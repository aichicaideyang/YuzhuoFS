CC = g++ # 编译器。

CFLAGS := -Wall # 显示所有报错信息。
CFLAGS += -std=c++17 # 启用C++17，使用 filesystem 等库。
CFLAGS += -fexec-charset=GB18030
CFLAGS += -g # 写入 debug 信息。

CFLAGS := $(strip $(CFLAGS)) # 删除多余空格。