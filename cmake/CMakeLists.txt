cmake_minimum_required(VERSION 3.4.1)   #最低cmake版本
#SET(CMAKE_TOOLCHAIN_FILE /home/spicegl/MySpace/toolchain.cmakes/okt507.cmake)
project(demo)   #设置项目名称
SET(CMAKE_CXX_STANDARD 11)   #设置C/C++标准

#SET(CMAKE_C_FLAGS "-Wl,-rpath=./lib")
#SET(CMAKE_CXX_FLAGS "-Wl,-rpath=./lib")

SET(build_dir ${CMAKE_CURRENT_SOURCE_DIR}/build)   #设置编译结果目录

SET(tpl_header_dir "/usr/local/include" "/usr/local/ncnn/include" "/usr/local/protobuf/include/google/protobuf")   #设置第三方库的头文件目录
SET(proj_header_dir ${CMAKE_CURRENT_SOURCE_DIR}/../../MineLibs)   #设置项目内部的头文件目录

SET(tpl_lib_dir "/usr/local/lib" "/usr/local/ncnn/lib" "/usr/local/protobuf/lib")   #设置第三方库的库目录
SET(proj_lib_dir ./lib)   #设置项目内部的库目录

SET(tpl_libs opencv_world ncnn pthread gomp)   #添加系统环境中的库文件
SET(proj_libs "")   #添加项目内部的库文件

#################依赖设置完毕######################

SET(EXECUTABLE_OUTPUT_PATH ${build_dir}/make_res)   #可执行文件输出目录
SET(LIBRARY_OUTPUT_PATH ${build_dir}/make_res)   #库文件输出目录

INCLUDE_DIRECTORIES(${tpl_header_dir} ${proj_header_dir})   #包含目录
LINK_DIRECTORIES(${tpl_lib_dir} ${proj_lib_dir})   #库目录

##add_library(run SHARED run.cpp)   #
add_executable(demo run.cpp)   #生成可执行文件
target_link_libraries(demo ${tpl_libs} ${proj_libs})   #将可执行文件链接库

SET(CMAKE_INSTALL_PREFIX install)   #设置make install命令的安装目录

install(TARGETS demo #${proj_libs} #将被执行安装的文件
        LIBRARY DESTINATION lib  # 动态库安装路径
        ARCHIVE DESTINATION lib  # 静态库安装路径
        RUNTIME DESTINATION bin  # 可执行文件安装路径
        )
install(DIRECTORY ${proj_header_dir} DESTINATION .)   #安装项目头文件到install目录