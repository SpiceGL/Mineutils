SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_PROCESSOR arm)

#SET(CMAKE_STAGING_PREFIX /home/devel/stage)   

SET (CMAKE_C_COMPILER "/usr/local/gcc/bin/gcc")   #设置gcc编译器路径
SET (CMAKE_CXX_COMPILER "/usr/local/gcc/bin/g++")   #设置g++编译器路径

#对FIND_PROGRAM()起作用，有三种取值，NEVER，ONLY，BOTH
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
 
#下面的三个选项表示只在交叉环境中查找库和头文件
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

#SET(CMAKE_C_FLAGS "-march=armv7ve -mfpu=neon  -mfloat-abi=hard -mcpu=cortex-a7 --sysroot=/home/spicegl/forlinx/sysroots/cortexa7hf-neon-poky-linux-gnueabi")
#SET(CMAKE_CXX_FLAGS "-march=armv7ve -mfpu=neon  -mfloat-abi=hard -mcpu=cortex-a7 --sysroot=/home/spicegl/forlinx/sysroots/cortexa7hf-neon-poky-linux-gnueabi")