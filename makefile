# 安静执行，不打印不必要的信息
MAKEFLAGS	+=	-s
# NDK根目录
NDK		?=	/home/eeds/Android/android-ndk-r8e
# 服务器体系
HOST_ARCH	?=	linux-x86
# NDK提供的头文件和库文件路径,必须包含这个
SYSROOT		?=	$(NDK)/platforms/android-14/arch-arm
# NDK编译器
CROSS_CC	?=	$(NDK)/toolchains/arm-linux-androideabi-4.7/prebuilt/$(HOST_ARCH)/bin/arm-linux-androideabi-g++ \
				--sysroot=$(SYSROOT)
# 编译选项
CFLAGS_INC 	?=	-I$(SYSROOT)/usr/include \
			-I$(NDK)/sources/cxx-stl/gnu-libstdc++/4.7/include \
        	  	-I$(NDK)/sources/cxx-stl/gnu-libstdc++/4.7/libs/armeabi-v7a/include
			 
CFLAGS		?=	-std=gnu++11 \
			-g \
			-Wall \
			-O2

# 链接选项
LDFLAGS		?=	-lc \
			-ljpeg \
			-lsupc++ \
			-ldl \
			-lgnustl_shared \
			-Wl,-dynamic-linker,/system/bin/linker \
			-L$(NDK)/sources/cxx-stl/gnu-libstdc++/4.7/libs/armeabi-v7a \
			-L$(SYSROOT)/usr/lib

# 当前目录下的源文件: .cpp
SRCS		:=	$(wildcard *.cpp)
# 当前目录下对应的obj文件
OBJS		:=	$(patsubst %.cpp,%.o,$(SRCS)) 
# obj存放目录
OBJDIR 		:=	objs
# obj文件生成位置
OBJS 		:=	$(addprefix $(OBJDIR)/, $(OBJS))
# 可执行文件
EXE 		:=	test
# EXE存放目录
EXEDIR 		:=	bin
EXE 		:=	$(addprefix $(EXEDIR)/, $(EXE))

.python:all clean newdir
all: newdir $(EXE) 
newdir:
	mkdir -p $(EXEDIR)
	mkdir -p $(OBJDIR)
$(EXE):$(OBJS)
	$(CROSS_CC) $(LDFLAGS) $(CFLAGS_INC) $(CFLAGS) -o $@ $^
$(OBJDIR)/%.o:%.cpp
	$(CROSS_CC) $(LDFLAGS) $(CFLAGS_INC) $(CFLAGS) -o $@ -c $^
clean:
	rm -rf $(OBJDIR) $(EXEDIR)

