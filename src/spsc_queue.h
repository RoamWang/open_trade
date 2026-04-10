#pragma once
#include <type_traits>
#include <atomic>
#include "ctp_event.h"

//无锁单生产者单消费者 (SPSC) 队列
template <typename T, size_t Size>
class spsc_queue
{
public:
#if defined(__GNUC__) && __GNUC__ < 5
	static_assert(__has_trivial_copy(T), "T must be trivially copyable");
#else
	static_assert(std::is_trivially_copyable<T>::value, "T must be trivially copyable");
#endif
	static_assert((Size& (Size - 1)) == 0, "Size must be power of 2");
	spsc_queue()
		: head_(0)
		, head_cached_(0)
		, tail_(0)
		, tail_cached_(0)
	{

	}
	~spsc_queue()
	{

	}
	spsc_queue(const spsc_queue&) = delete;
	spsc_queue& operator= (const spsc_queue&) = delete;

	bool push(const T& val)
	{
		//安全读取
		size_t head = head_.load(std::memory_order_relaxed);
		//先检查本地缓存的tail, 减少原子读操作
		if ((head - tail_cached_) >= Size)
		{
			//tail 仅需 relaxed，不依赖消费者其他写入, 但用 acquire 以获得更强的可见性
			tail_cached_ = tail_.load(std::memory_order_acquire);
			if ((head - tail_cached_) >= Size)
			{
				//队列已满
				return false;
			}
		}

		buffer_[head & (Size - 1)] = val;
		//发布
		head_.store(head + 1, std::memory_order_release);
		return true;
	}

	bool pop(T& val)
	{
		//tail 仅由消费者更新，relaxed 读取即可
		size_t tail = tail_.load(std::memory_order_relaxed);
		// 如果本地缓存的 head 已经跑到了 tail 后面，说明 buffer 里还有数据没读完
		if (tail == head_cached_)
		{
			// 只有本地缓存用光了，才去执行昂贵的 acquire 原子读
			//acquire：同步生产者写入 buffer_ 的数据
			head_cached_ = head_.load(std::memory_order_acquire);
			if (tail == head_cached_)
			{
				//队列已空
				return false;
			}
		}

		val = buffer_[tail & (Size - 1)];
		//确保 val 被读走后，才更新 tail_，让生产者知道这个槽位空出来了
		tail_.store(tail + 1, std::memory_order_release);
		return true;
	}

private:
	// alignas(64) 防止伪共享
	alignas(64) std::atomic<size_t> head_;
	//消费者私有,无需原子属性
	size_t head_cached_;
	alignas(64) std::atomic<size_t> tail_;
	//生产者私有,无需原子属性
	size_t tail_cached_;
	alignas(64) T buffer_[Size];
};

//更多合约使用8192
typedef spsc_queue<ctp_task, 4096> optimized_queue;
