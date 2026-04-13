SRCS = logger.cpp 
       
OBJS = logger.o

CXXFLAGS = -fPIC -Wall -std=c++0x -O3

INCLUDE = -I $(NAUT_ROOTDIR)/thirdparty/boost_1_65_0/
PHONY : all 
all : $(OBJS) 
	ar r liblog.a $(OBJS)
	g++ -shared -o liblog.so $(OBJS)
	@mkdir -p $(NAUT_ROOTDIR)/server/public/lib
	@mkdir -p $(NAUT_ROOTDIR)/server/public/liblog
	-cp -p liblog.a $(NAUT_ROOTDIR)/server/public/lib
#	-cp -p liblog.so ../../public/lib
	-cp -p *.h $(NAUT_ROOTDIR)/server/public/liblog
	-rm -rf $(OBJS)
$(OBJS):	
	g++ -c $(CXXFLAGS) $(INCLUDE) $(SRCS)

.PHONY : clean
clean:
	rm -f *.a *.o *.so


