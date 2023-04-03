#pragma once
// 模拟磁盘
#ifndef MACHINE_H
#define MACHINE_H
#include "FileSystem.h"

#include "common.h"
// #include "Kernel.h"
//  #include "BufferManager.h"
class Machine
{
public:
    Machine();
    ~Machine();
    void Initialize();

private:
    void init_spb(SuperBlock &sb);
    void init_db(char *data);
    void init_img(int fd);

private:
    const char *img_path = "c.img";
    int img_fd; // devpath的fd，文件系统打开时 open，关闭时 close.
    // BufferManager *m_BufferManager; /* FileSystem类需要缓存管理模块(BufferManager)提供的接口 */
};
#endif