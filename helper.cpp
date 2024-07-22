#include "helper.h"

void CpuWatcher::updateCpuProperty()
{
    int usage = static_cast<int>(getCpuUsage());
    setCurrentCpuUsage(usage);
}

__int64 CpuWatcher::FileTimeToInt64(const FILETIME &ftime) {
    LARGE_INTEGER li;
    li.LowPart = ftime.dwLowDateTime;
    li.HighPart = ftime.dwHighDateTime;
    return li.QuadPart;
}

double CpuWatcher::getCpuUsage(){
    FILETIME IdleTime, KernelTime, UserTime;
    GetSystemTimes(&IdleTime, &KernelTime, &UserTime);

    __int64 idle = FileTimeToInt64(IdleTime) - FileTimeToInt64(preIdleTime);
    __int64 kernel = FileTimeToInt64(KernelTime) - FileTimeToInt64(preKernelTime);
    __int64 user = FileTimeToInt64(UserTime) - FileTimeToInt64(preUserTime);

    preIdleTime = IdleTime;
    preKernelTime = KernelTime;
    preUserTime = KernelTime;

    __int64 total = kernel + user;
    if (total == 0) return 0.0;
    return (total - idle) * 100.0 / total;
}
