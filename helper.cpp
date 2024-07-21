#include "helper.h"

void CpuWatcher::updateCpuProperty()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 100);
    setCurrentCpuUsage(dis(gen));
}
