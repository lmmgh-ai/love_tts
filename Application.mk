#开启异常控制
APP_CPPFLAGS := -std=c++17

#编译选项 APP_CFLAGS := -O2 -DNDEBUG 

# 启用RTTI 
APP_CPPFLAGS += -frtti
 
# 启用异常处理 
APP_CPPFLAGS += -fexceptions

#关联可执行文件和共享库时要传递的标记。
APP_LDFLAGS := -latomic
# 指定目标ABI
APP_ABI := arm64-v8a armeabi-v7a  #x86 x86_64 

# 指定平台版本 
APP_PLATFORM := android-16

# 构建所有匹配的模块
APP_BUILD_SCRIPT := $(call my-dir)/Android.mk 
 
# 指定模块列表（可选）
#APP_MODULES := mylib otherlib
 

# 构建模式 
APP_OPTIM := release
# 可选值：release（默认）或 debug

# 指定STL类型
APP_STL := c++_static
# 可选值：
# system          - 默认，最小C++运行时 
# gabi++_static   - GAbi++静态库 
# gabi++_shared   - GAbi++共享库
# stlport_static  - STLport静态库
# stlport_shared  - STLport共享库
# c++_static      - LLVM libc++静态库
# c++_shared      - LLVM libc++共享库