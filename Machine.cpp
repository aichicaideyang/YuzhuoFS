#include "Machine.h"
Machine::Machine()
{
}
Machine::~Machine()
{
}
void Machine::Initialize()
{
    int fd = open(img_path, O_RDWR);

    if (fd == -1)
    {
        // 文件不存在，那么就创建
        fd = open(img_path, O_RDWR | O_CREAT, 0666);
        if (fd == -1)
        {
            sys_log("Machine 初始化失败!");
            exit(-1);
        }
        // 对磁盘进行初始化
        this->init_img(fd);
    }
    this->img_fd = fd;
}

void Machine::init_img(int fd)
{
    SuperBlock spb;
    init_spb(spb);
    DiskInode *di_table = new DiskInode[FileSystem::INODE_ZONE_SIZE * FileSystem::INODE_NUMBER_PER_SECTOR];
    time_t init_time;
    time(&init_time);
    time_t cur_time; // 用于记录一下创建时间
    char *datablock = new char[FileSystem::DATA_ZONE_SIZE * 512];
    memset(datablock, 0, FileSystem::DATA_ZONE_SIZE * 512);
    init_db(datablock);
    // 下面开始创建dev, home, bin, usr
    // 这里由于文件系统内部还有很多细节没有实现
    // 所以我这里直接分配，后面实现之后再做修改

    // 在目录文件下首先需要创建 . .. 文件, 这两个不需要分配inode,就是0号inode
    // 设置 rootDiskInode 的初始值
    di_table[0].d_mode = Inode::IFDIR;  // 文件类型为目录文件
    di_table[0].d_mode |= Inode::IEXEC; // 文件的执行权限
    time(&cur_time);
    di_table[0].d_atime = cur_time - init_time;
    di_table[0].d_mtime = cur_time - init_time;
    di_table[0].d_size = 0; // 空目录文件

    int file_inode_idx = 0;   // 创建的文件的inode号
    char file_name[28] = "."; // 创建的文件的名称
    // 将 . 文件写入
    memcpy(datablock, (char *)&file_inode_idx, sizeof(int));       // 先写入inode号
    memcpy(datablock + sizeof(int), file_name, sizeof(file_name)); // 再写入文件名称
    strcpy(file_name, "..");
    // 将 ..文件写入
    memcpy(datablock + 32, (char *)&file_inode_idx, sizeof(int));       // 先写入inode号
    memcpy(datablock + sizeof(int) + 32, file_name, sizeof(file_name)); // 再写入文件名称

    auto dev_inode_idx = spb.s_inode[--spb.s_ninode];
    // 当然这里的分配其实是非常简单的，按理来说还需要考虑如果没有的情况
    auto dev_inode = di_table[dev_inode_idx];
    time(&cur_time);
    dev_inode.d_mtime = cur_time - init_time;
    dev_inode.d_atime = cur_time - init_time;
    dev_inode.d_mode = Inode::IFDIR;  // 文件类型为目录文件
    dev_inode.d_mode |= Inode::IEXEC; // 文件的执行权限
    // cout << spb.s_ninode << "\n";
    auto dev_dataBlock_idx = spb.s_free[--spb.s_fsize];
    // 当然这里的分配其实是非常简单的，按理来说还需要考虑如果没有的情况
    dev_inode.d_addr[di_table[0].d_size] = dev_dataBlock_idx;
    di_table[0].d_size += 1;
    strcpy(file_name, "dev");
    // 将dev文件写入
    memcpy(datablock + (di_table[0].d_size + 1) * 32, (char *)&dev_inode_idx, sizeof(int));        // 先写入inode号
    memcpy(datablock + (di_table[0].d_size + 1) * 32 + sizeof(int), file_name, sizeof(file_name)); // 再写入文件名称
    // cout << spb.s_ninode << "\n";

    auto home_inode_idx = spb.s_inode[--spb.s_ninode];
    // 当然这里的分配其实是非常简单的，按理来说还需要考虑如果没有的情况
    auto home_inode = di_table[home_inode_idx];
    time(&cur_time);
    home_inode.d_mtime = cur_time - init_time;
    home_inode.d_atime = cur_time - init_time;
    home_inode.d_mode = Inode::IFDIR;  // 文件类型为目录文件
    home_inode.d_mode |= Inode::IEXEC; // 文件的执行权限
    // cout << "debug1"   << "\n";
    auto home_dataBlock_idx = spb.s_free[-spb.s_fsize];
    // 当然这里的分配其实是非常简单的，按理来说还需要考虑如果没有的情况
    home_inode.d_addr[di_table[0].d_size] = home_dataBlock_idx;
    di_table[0].d_size += 1;
    strcpy(file_name, "home");
    // 将home文件写入
    memcpy(datablock + (di_table[0].d_size + 1) * 32, (char *)&home_inode_idx, sizeof(int));       // 先写入inode号
    memcpy(datablock + (di_table[0].d_size + 1) * 32 + sizeof(int), file_name, sizeof(file_name)); // 再写入文件名称

    auto bin_inode_idx = spb.s_inode[--spb.s_ninode];
    // 当然这里的分配其实是非常简单的，按理来说还需要考虑如果没有的情况
    auto bin_inode = di_table[bin_inode_idx];
    time(&cur_time);
    bin_inode.d_mtime = cur_time - init_time;
    bin_inode.d_atime = cur_time - init_time;
    bin_inode.d_mode = Inode::IFDIR;  // 文件类型为目录文件
    bin_inode.d_mode |= Inode::IEXEC; // 文件的执行权限
    // cout << "debug2" << "\n";
    auto bin_dataBlock_idx = spb.s_free[--spb.s_fsize];
    // 当然这里的分配其实是非常简单的，按理来说还需要考虑如果没有的情况
    bin_inode.d_addr[di_table[0].d_size] = bin_dataBlock_idx;
    di_table[0].d_size += 1;
    strcpy(file_name, "bin");
    // 将bin文件写入
    memcpy(datablock + (di_table[0].d_size + 1) * 32, (char *)&bin_inode_idx, sizeof(int));        // 先写入inode号
    memcpy(datablock + (di_table[0].d_size + 1) * 32 + sizeof(int), file_name, sizeof(file_name)); // 再写入文件名称

    auto usr_inode_idx = spb.s_inode[--spb.s_ninode];
    // 当然这里的分配其实是非常简单的，按理来说还需要考虑如果没有的情况
    auto usr_inode = di_table[usr_inode_idx];
    time(&cur_time);
    usr_inode.d_mtime = cur_time - init_time;
    usr_inode.d_atime = cur_time - init_time;
    // 这里还需要修改一下root文件夹修改时间
    di_table[0].d_mtime = cur_time;
    usr_inode.d_mode = Inode::IFDIR;  // 文件类型为目录文件
    usr_inode.d_mode |= Inode::IEXEC; // 文件的执行权限
    // cout << "debug3" << "\n";
    auto usr_dataBlock_idx = spb.s_free[--spb.s_fsize];
    // 当然这里的分配其实是非常简单的，按理来说还需要考虑如果没有的情况
    usr_inode.d_addr[di_table[0].d_size] = usr_dataBlock_idx;
    di_table[0].d_size += 1;
    strcpy(file_name, "usr");
    // 将usr文件写入
    memcpy(datablock + (di_table[0].d_size + 1) * 32, (char *)&usr_inode_idx, sizeof(int));        // 先写入inode号
    memcpy(datablock + (di_table[0].d_size + 1) * 32 + sizeof(int), file_name, sizeof(file_name)); // 再写入文件名称

    // 写入文件
    write(fd, &spb, sizeof(SuperBlock));
    write(fd, di_table, FileSystem::INODE_ZONE_SIZE * FileSystem::INODE_NUMBER_PER_SECTOR * sizeof(DiskInode));
    write(fd, datablock, FileSystem::DATA_ZONE_SIZE * 512);

    sys_log("Disk img initialized complete!");
    close(fd);
    sys_log("Now let's see whether we're succeed!");

    // 打印文件信息
    // 这里一共需要打印
    // .
    // ..
    // dev
    // bin
    // home
    // usr
    // 我们只需要沿着root的数据区找即可

    auto root_data_addr = FileSystem::DATA_ZONE_START_SECTOR * 512;
    fstream file;
    file.open(img_path, ios::in);
    if (!file.is_open())
    {
        sys_log("open file failed!");
        exit(-1);
    }
    file.seekg(root_data_addr);
    for (int i = 0; i <= di_table[0].d_size + 1; i++)
    {
        // 每32个字节是一个inode信息
        char p[32];
        file.read(p, sizeof(p));
        // inode 号
        int cur_indoe_idx;
        memcpy(&cur_indoe_idx, p, sizeof(int));
        char cur_file_name[28];
        memcpy(cur_file_name, p + sizeof(int), sizeof(cur_file_name));
        // 打印一下inode结点信息
        cout << "file inode idx: " << cur_indoe_idx << " "
             << "create time: " << di_table[cur_indoe_idx].d_atime << " file_name: " << cur_file_name << "\n";
    }
}
void Machine::init_spb(SuperBlock &sb)
{
    sb.s_isize = FileSystem::INODE_ZONE_SIZE;
    sb.s_fsize = FileSystem::DATA_ZONE_END_SECTOR + 1;
    sb.s_nfree = (FileSystem::DATA_ZONE_SIZE - 99) % 100;

    // 找到最后一个盘块组的第一个盘块
    int start_last_datablk = FileSystem::DATA_ZONE_START_SECTOR;
    while (true)
    {
        if ((start_last_datablk + 100 - 1) < FileSystem::DATA_ZONE_END_SECTOR)
            start_last_datablk += 100;
        else
            break;
    }
    start_last_datablk--;
    // 将最后一个盘块组的盘块号填入
    for (int i = 0; i < sb.s_nfree; ++i)
        sb.s_free[i] = start_last_datablk + i;

    sb.s_ninode = 100;
    for (int i = 0; i < sb.s_ninode; ++i)
        sb.s_inode[i] = i;

    sb.s_fmod = 0;
    sb.s_ronly = 0;
}

void Machine::init_db(char *data)
{
    struct
    {
        int nfree;     // 本组空闲的个数
        int free[100]; // 本组空闲的索引表
    } tmp_table;

    int last_datablk_num = FileSystem::DATA_ZONE_SIZE; // 未加入索引的盘块的数量
    // 初始化组长盘块
    for (int i = 0;; i++)
    {
        if (last_datablk_num >= 100)
            tmp_table.nfree = 100;
        else
            tmp_table.nfree = last_datablk_num;
        last_datablk_num -= tmp_table.nfree;

        for (int j = 0; j < tmp_table.nfree; j++)
        {
            if (i == 0 && j == 0)
                tmp_table.free[j] = 0;
            else
            {
                tmp_table.free[j] = 100 * i + j + FileSystem::DATA_ZONE_START_SECTOR - 1;
            }
        }
        memcpy(&data[99 * 512 + i * 100 * 512], (void *)&tmp_table, sizeof(tmp_table));
        if (last_datablk_num == 0)
            break;
    }
}
