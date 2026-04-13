#!/bin/sh

CURDIR=`pwd`
ndk-build NDK_PROJECT_PATH=${CURDIR} APP_BUILD_SCRIPT=${CURDIR}/Android.mk NDK_APPLICATION_MK=${CURDIR}/Application.mk

MY_SRC_PATH=${CURDIR}/obj/local/*
MY_DEST_PATH=${NAUT_ROOTDIR}/public/android/

cp -rf ${MY_SRC_PATH} ${MY_DEST_PATH}

# remove objs
list="arm64-v8a armeabi-v7a x86 x86_64"
for i in $list;
do

echo "enter " ${MY_DEST_PATH}$i ;
cd ${MY_DEST_PATH}$i
rm -rf objs
cd ..

done

echo "build android finish"