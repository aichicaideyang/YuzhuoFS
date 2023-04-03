#include "FileSystem.h"

/*==============================class SuperBlock===================================*/
/* 系统全局超级块SuperBlock对象 */
SuperBlock g_spb;

SuperBlock::SuperBlock()
{
    pthread_mutex_init(&s_ilock, NULL);
    pthread_mutex_init(&s_flock, NULL);
    // nothing to do here
}

SuperBlock::~SuperBlock()
{
    pthread_mutex_destroy(&s_ilock);
    pthread_mutex_destroy(&s_flock);
    // nothing to do here
}

/*==============================class FileSystem===================================*/
FileSystem::FileSystem()
{
    // nothing to do here
}

FileSystem::~FileSystem()
{
    // nothing to do here
}

void FileSystem::Initialize()
{
    // this->m_BufferManager = &Kernel::Instance().GetBufferManager();
    this->updlock = 0;
}

SuperBlock *FileSystem::GetFS()
{
    return &g_spb;
}

bool FileSystem::BadBlock(SuperBlock *spb, int blkno)
{
    return 0;
}
