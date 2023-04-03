#include "INode.h"

/*==============================class Inode===================================*/
/*	预读块的块号，对普通文件这是预读块所在的物理块号。对硬盘而言，这是当前物理块（扇区）的下一个物理块（扇区）*/
int Inode::rablock = 0;

/* 内存打开 i节点*/
Inode::Inode()
{
    /* 清空Inode对象中的数据 */
    // this->Clean();
    /* 去除this->Clean();的理由：
     * Inode::Clean()特定用于IAlloc()中清空新分配DiskInode的原有数据，
     * 即旧文件信息。Clean()函数中不应当清除i_dev, i_number, i_flag, i_count,
     * 这是属于内存Inode而非DiskInode包含的旧文件信息，而Inode类构造函数需要
     * 将其初始化为无效值。
     */

    /* 将Inode对象的成员变量初始化为无效值 */
    pthread_mutex_init(&this->mutex, NULL);
    this->i_flag = 0;
    this->i_mode = 0;
    this->i_count = 0;
    this->i_nlink = 0;
    this->i_number = -1;
    this->i_uid = -1;
    this->i_gid = -1;
    this->i_size = 0;
    this->i_lastr = -1;
    for (int i = 0; i < 10; i++)
    {
        this->i_addr[i] = 0;
    }
}

Inode::~Inode()
{
    pthread_mutex_destroy(&this->mutex);
    // nothing to do here
}

void Inode::NFrele()
{
    /* 解锁pipe或Inode,并且唤醒相应进程 */
    this->i_flag &= ~Inode::ILOCK;
    // printf("[NFrel] 解锁pInode: i_number=%d\n", this->i_number);
    pthread_mutex_unlock(&this->mutex);
}

void Inode::NFlock()
{
    // printf("[NFlock] 上锁pInode: inumber=%d\n", this->i_number);
    pthread_mutex_lock(&this->mutex);
    this->i_flag |= Inode::ILOCK;
}

void Inode::Clean()
{
    /*
     * Inode::Clean()特定用于IAlloc()中清空新分配DiskInode的原有数据，
     * 即旧文件信息。Clean()函数中不应当清除i_dev, i_number, i_flag, i_count,
     * 这是属于内存Inode而非DiskInode包含的旧文件信息，而Inode类构造函数需要
     * 将其初始化为无效值。
     */
    this->i_mode = 0;
    this->i_nlink = 0;
    this->i_uid = -1;
    this->i_gid = -1;
    this->i_size = 0;
    this->i_lastr = -1;
    for (int i = 0; i < 10; i++)
    {
        this->i_addr[i] = 0;
    }
}

/*============================class DiskInode=================================*/

DiskInode::DiskInode()
{
    /*
     * 如果DiskInode没有构造函数，会发生如下较难察觉的错误：
     * DiskInode作为局部变量占据函数Stack Frame中的内存空间，但是
     * 这段空间没有被正确初始化，仍旧保留着先前栈内容，由于并不是
     * DiskInode所有字段都会被更新，将DiskInode写回到磁盘上时，可能
     * 将先前栈内容一同写回，导致写回结果出现莫名其妙的数据。
     */
    this->d_mode = 0;
    this->d_nlink = 0;
    this->d_uid = -1;
    this->d_gid = -1;
    this->d_size = 0;
    for (int i = 0; i < 10; i++)
    {
        this->d_addr[i] = 0;
    }
    this->d_atime = 0;
    this->d_mtime = 0;
}

DiskInode::~DiskInode()
{
    // nothing to do here
}
