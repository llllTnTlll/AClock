#ifndef SYS_CORE_HPP
#define SYS_CORE_HPP

#include <list>
#include <ui.h>
#include <string>
#include <inputdevice.hpp>
#include "weather.hpp"

class Page
{
public:
    Page(const std::string &pageName, lv_obj_t *pagePtr)
        : _pageName(pageName), _pagePtr(pagePtr) {}

    const std::string &getPageName() const
    {
        return _pageName;
    }

    lv_obj_t *getPagePtr() const
    {
        return _pagePtr;
    }

private:
    std::string _pageName;
    lv_obj_t *_pagePtr;
};

class SysCore
{
private:
    // 页面相关
    std::list<Page *> Pages;
    Page *currentPage = nullptr;
    static volatile bool isMenuShowing;

    // RTC相关
    struct tm timeinfo;
    // freeRTOS任务
    static TaskHandle_t uiTaskHandle;
    static TaskHandle_t wifiTaskHandle;
    static TaskHandle_t syncTimeTaskHandle;
    static void uiTask(void *pvParameters);
    static void wifiTask(void *pvParameters);
    static void syncTimeTask(void *pvParameters);
    // freeRTOS定时器
    static TimerHandle_t lvgl_tick_timer;
    static void lv_tick_task(TimerHandle_t xTimer);
    // LVGL定时器
    lv_timer_t *updateTimeLabelTimer;
    lv_timer_t *updateSecArcTimer;
    lv_timer_t *updateMenuStatusTimer;
    lv_timer_t *printStackUsageTimer;
    static void updateElementsData(lv_timer_t *timer);
    static void updateTimeSecArc(lv_timer_t *timer);
    static void sideMenuShowHide(lv_timer_t *timer);
    static void printStackUsage(lv_timer_t *timer);
    // 设备
    lv_disp_t *disp;
    lv_indev_t *indev;
    static void InputdevON(lv_anim_t *ani);
    static void InputdevOff(lv_anim_t *ani);

public:
    SysCore(lv_disp_t *disp, lv_indev_t *indev);
    ~SysCore();
    void showNextPage();
    void showPrevPage();
    void blockInput(uint32_t ms);
};

#endif