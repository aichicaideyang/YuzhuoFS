.DEFAULT_GOAL := all
include ./makefile.inc

TARGET = ./objs
INC_PATH = ./include/
SOURCE = $(wildcard *.cpp) # 定位所有cpp文件。
OBJS = $(patsubst %.cpp, $(TARGET)/%.o, $(SOURCE)) # 获取目标文件集。

$(TARGET)/%.o: %.cpp
	$(CC) -I$(INC_PATH) $(CFLAGS) -c $< -o $@

.PHONY: all
all: $(OBJS)
	$(CC) $(CFLAGS) -o ./fileSystem.o $(OBJS)	

.PHONY: clean
clean:
	rm -f $(OBJS) 