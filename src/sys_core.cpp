#include <sys_core.hpp>

TimerHandle_t SysCore::lvgl_tick_timer = nullptr;

SysCore::SysCore(lv_disp_t *disp, lv_indev_t *indev)
{
    this->disp = disp;
    this->indev = indev;

    // 将slideMenu置于layer_top
    lv_obj_t *layer_top = lv_layer_top();
    lv_obj_set_parent(ui_sideMenu, layer_top);

    // 页面管理初始化
    Page *airQualityScreen = new Page("airQualityScreen", ui_airQualityScreen);
    Page *clockScreen = new Page("clockScreen", ui_clockScreen);
    Page *weatherScreen = new Page("weatherScreen", ui_weatherScreen);

    Pages.push_back(airQualityScreen);
    Pages.push_back(clockScreen);
    Pages.push_back(weatherScreen);

    currentPage = clockScreen;

    // 连接WIFI 更新系统时间
    // connectWifi();
    // while (WiFi.status() != WL_CONNECTED)
    // {
    //     delay(500);
    // }

    // updateRTC();
    // while (!time(nullptr))
    // {
    //     delay(100);
    // }

    // 启动心跳定时器
    lvgl_tick_timer = xTimerCreate("lv_tick_timer", pdMS_TO_TICKS(1), pdTRUE, NULL, lv_tick_task);

    if (lvgl_tick_timer != NULL)
    {
        xTimerStart(lvgl_tick_timer, 0);
    }

    // 创建刷新ui的Task
    xTaskCreate(uiTask, "uiTask", 8000, NULL, 1, &uiTaskHandle);
}

// bool SysCore::updateAllTimeLabel()
// {
//     if (currentPage->getPageName() == "clockScreen")
//     {
//         struct tm timeinfo;
//         if (!getLocalTime(&timeinfo))
//             return false;
//         int hour = timeinfo.tm_hour;
//         std::string currentTime = std::to_string(timeinfo.tm_hour) + ':' + std::to_string(timeinfo.tm_min);
//         // lv_label_set_text(ui_Label2, currentTime.c_str());
//         return true;
//     }
// }

SysCore::~SysCore()
{
    for (Page *page : Pages)
    {
        delete page;
    }
    Pages.clear();
}

void SysCore::showNextPage()
{
    // 遍历页面列表，找到当前页面
    for (auto it = Pages.begin(); it != Pages.end(); ++it)
    {
        if ((*it)->getPageName() == currentPage->getPageName())
        {
            // 使用 std::next 获取下一个迭代器
            auto next_it = std::next(it);
            if (next_it == Pages.end())
            {
                // 已经是最后一页，播放到底动画并返回 false
                menuReachBottom_Animation(ui_menuContent, 0);
                blockInput(300);
                return;
            }
            else
            {
                // 切换到下一页
                Page *target = *next_it;
                lv_scr_load_anim(target->getPagePtr(), LV_SCR_LOAD_ANIM_OVER_TOP, 300, 0, false);
                menuDownAnim_Animation(ui_menuContent, 0);
                blockInput(500);
                currentPage = target;
                return;
            }
        }
    }
    return;
}

void SysCore::showPrevPage()
{
    // 找到当前页迭代器
    for (auto it = Pages.begin(); it != Pages.end(); ++it)
    {
        if ((*it)->getPageName() == currentPage->getPageName())
        {
            if (it == Pages.begin())
            {
                // 当前页面已经是第一页，无法再向上移动
                menuReachTop_Animation(ui_menuContent, 0);
                blockInput(300);
                return;
            }
            else
            {
                // 安全地获取前一个迭代器
                --it;
                Page *target = *it;
                lv_scr_load_anim(target->getPagePtr(), LV_SCR_LOAD_ANIM_OVER_BOTTOM, 300, 0, false);
                menuUpAnim_Animation(ui_menuContent, 0);
                blockInput(500);
                currentPage = target;
                return;
            }
        }
    }
    return;
}

void SysCore::connectWifi()
{
    WiFi.begin(ssid, password);
}

void SysCore::updateRTC()
{
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void SysCore::uiTask(void *)
{
    while (1)
    {
        lv_task_handler();             // 处理LVGL任务
        vTaskDelay(pdMS_TO_TICKS(10)); // 每10ms调用一次
    }
}

void SysCore::lv_tick_task(TimerHandle_t xTimer)
{
    lv_tick_inc(1); // 每1ms增加LVGL的时间计数
}

void SysCore::blockInput(uint32_t ms)
{
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_time(&a, ms);
    a.user_data = this;
    lv_anim_set_start_cb(&a, InputdevOff); // 动画开始时的回调
    lv_anim_set_ready_cb(&a, InputdevON);   // 动画结束时的回调
    lv_anim_start(&a);
}

void SysCore::InputdevON(lv_anim_t* ani)
{
    SysCore* instance = static_cast<SysCore*>(ani->user_data);
    lv_indev_enable(instance->indev, true);
}

void SysCore::InputdevOff(lv_anim_t* ani)
{
    SysCore* instance = static_cast<SysCore*>(ani->user_data);
    lv_indev_enable(instance->indev, false);
}
