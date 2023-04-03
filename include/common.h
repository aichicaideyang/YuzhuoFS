#pragma once
#ifndef COMMON_H
#define COMMON_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <functional>
#include <pthread.h>
#include <cstdlib>
#include <time.h>
#include <string.h>
#include <chrono>
#include <algorithm>
#include <map>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
using namespace std;
typedef int ErrorCode;
#define NOERROR 0
void sys_log(string info);
#endif
