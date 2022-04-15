cmake_minimum_required(VERSION 3.4.1)   #最低cmake版本
#SET(CMAKE_TOOLCHAIN_FILE ../arm_linux_setup.cmake)   #设置交叉编译环境

project(cmakeist_test)   #设置项目名称
SET(CMAKE_CXX_STANDARD 17)   #设置C/C++标准

#SET(CMAKE_BUILD_TYPE Debug)   #设置编译模式

#SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")   #debug模式参数
#SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")   #release模式参数

###设置编译结果目录
if(NOT DEFINED build_dir)
	SET(build_dir ${CMAKE_CURRENT_SOURCE_DIR}/build)
endif()

SET(tpl_header_dir "E:\\opencv3416\\build_gcc930\\install\\include")   #设置第三方库的头文件目录
SET(proj_header_dir ${CMAKE_CURRENT_SOURCE_DIR}/../../../MineLibs)   #设置项目内部的头文件目录

SET(tpl_lib_dir "E:\\opencv3416\\build_gcc930\\install\\x64\\mingw\\lib")   #设置第三方库的库目录
SET(proj_lib_dir ${CMAKE_CURRENT_SOURCE_DIR}/lib)   #设置项目内部的库目录

SET(tpl_libs opencv_world3416)   #添加系统环境中的库文件
SET(proj_libs "")   #添加项目内部的库文件

#################依赖设置完毕######################

SET(EXECUTABLE_OUTPUT_PATH ${build_dir}/make_res)   #可执行文件输出目录
SET(LIBRARY_OUTPUT_PATH ${build_dir}/make_res)   #库文件输出目录

INCLUDE_DIRECTORIES(${tpl_header_dir} ${proj_header_dir})   #包含目录
LINK_DIRECTORIES(${tpl_lib_dir} ${proj_lib_dir})   #库目录

add_executable(main CVTest.cpp)   #生成可执行文件
target_link_libraries(main ${tpl_libs} ${proj_libs})   #将可执行文件链接库

file(COPY ${proj_header_dir} DESTINATION ${build_dir})   #将项目内部的头文件放到build中

###设置make install命令的安装目录
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX ${build_dir}/install CACHE PATH "install dir" FORCE)
endif()

install(TARGETS main ${proj_libs}   #将被执行安装的文件
        LIBRARY DESTINATION bin   # 动态库安装路径
        ARCHIVE DESTINATION lib   # 静态库安装路径
        RUNTIME DESTINATION bin   # 可执行文件安装路径
        )
install(DIRECTORY ${proj_header_dir} DESTINATION .)   #安装项目头文件到install目录
