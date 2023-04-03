#include "Machine.h"
// 主程序
int main()
{
    sys_log("welcome to yuzhuo unix!");
    //  这里仅创建一个Machine,后续其实需要使用Kernel实例，当然这里只是演示格式化
    Machine yMachine;
    //  进行初始化操作
    yMachine.Initialize();

    sys_log("Good Bye!");
    return 0;
}