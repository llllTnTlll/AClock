#include <sys_core.hpp>

TimerHandle_t SysCore::lvgl_tick_timer = nullptr;
volatile bool SysCore::isMenuShowing = true;

TaskHandle_t SysCore::syncTimeTaskHandle = NULL;
TaskHandle_t SysCore::uiTaskHandle = NULL;
TaskHandle_t SysCore::wifiTaskHandle = NULL;

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

    // 逻辑相关使用freeRTOS 任务和定时器
    lvgl_tick_timer = xTimerCreate("lv_tick_timer", pdMS_TO_TICKS(1), pdTRUE, NULL, lv_tick_task); // 更新lvgl心跳计时器
    if (lvgl_tick_timer != NULL)
    {
        xTimerStart(lvgl_tick_timer, 0);
    }

    xTaskCreate(uiTask, "uiTask", 6000, NULL, 4, &SysCore::uiTaskHandle);                   // 创建刷新ui的Task
    xTaskCreate(wifiTask, "wifiTask", 6000, NULL, 3, &SysCore::wifiTaskHandle);             // 连接WIFI
    xTaskCreate(syncTimeTask, "timeSyncTask", 6600, NULL, 3, &SysCore::syncTimeTaskHandle); // 更新系统RTC

    // UI相关使用lvgl定时器保证线程安全
    updateTimeLabelTimer = lv_timer_create(updateElementsData, 500, NULL); // 每500ms更新一次时间信息
    updateSecArcTimer = lv_timer_create(updateTimeSecArc, 1000, NULL);     // 每1s更新一次时钟Arc
    updateMenuStatusTimer = lv_timer_create(sideMenuShowHide, 500, NULL);  // 每500ms确认一次菜单栏状态

    printStackUsageTimer = lv_timer_create(printStackUsage, 1000, NULL);
}

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
    if (!SysCore::isMenuShowing)
    {
        menuShow_Animation(ui_sideMenu, 0);
        SysCore::isMenuShowing = true;
        return;
    }
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
                blockInput(300);
                currentPage = target;
                return;
            }
        }
    }
    return;
}

void SysCore::showPrevPage()
{
    if (!SysCore::isMenuShowing)
    {
        menuShow_Animation(ui_sideMenu, 0);
        SysCore::isMenuShowing = true;
        return;
    }
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
                --it;
                Page *target = *it;
                lv_scr_load_anim(target->getPagePtr(), LV_SCR_LOAD_ANIM_OVER_BOTTOM, 300, 0, false);
                menuUpAnim_Animation(ui_menuContent, 0);
                blockInput(300);
                currentPage = target;
                return;
            }
        }
    }
    return;
}

void SysCore::uiTask(void *)
{
    TickType_t xLastWakeTime;
    const TickType_t xPeriod = pdMS_TO_TICKS(15);

    xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        lv_task_handler();
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}

void SysCore::wifiTask(void *pvParameters)
{
    while (1)
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            WiFi.begin("Hamamotolab-D410a", "Hamamoto");
        }
        vTaskDelay(5000); // 每五秒确认一次WIFI连接状态
    }
}

void SysCore::syncTimeTask(void *pvParameters)
{
    while (1)
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            configTime(9 * 3600, 0, "pool.ntp.org");
            vTaskDelay(pdMS_TO_TICKS(60000)); // 若成功更新则下次更新时间在一分钟之后
        }
        else
        {
            vTaskDelay(pdMS_TO_TICKS(1000)); // 若更新不成功则等待一分钟候重试
        }
    }
}

void SysCore::printStackUsage(lv_timer_t *timer)
{
    UBaseType_t highWaterMark1 = uxTaskGetStackHighWaterMark(SysCore::syncTimeTaskHandle);
    UBaseType_t highWaterMark2 = uxTaskGetStackHighWaterMark(SysCore::uiTaskHandle);
    UBaseType_t highWaterMark3 = uxTaskGetStackHighWaterMark(SysCore::wifiTaskHandle);
    Serial.print("Task syncTime Stack High Water Mark: ");
    Serial.println(highWaterMark1 * sizeof(StackType_t));

    Serial.print("Task uiTask Stack High Water Mark: ");
    Serial.println(highWaterMark2 * sizeof(StackType_t));

    Serial.print("Task wifiTask Stack High Water Mark: ");
    Serial.println(highWaterMark3 * sizeof(StackType_t));
}

void SysCore::updateElementsData(lv_timer_t *timer)
{
    time_t now = time(NULL);
    if (now > 100000)
    {
        struct tm timeinfo;
        if (getLocalTime(&timeinfo))
        {
            char currentTime[6];
            snprintf(currentTime, sizeof(currentTime), "%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);
            lv_label_set_text(ui_timeLabel1, currentTime);
            lv_label_set_text(ui_timeLabel2, currentTime);
        }
    }
}

void SysCore::updateTimeSecArc(lv_timer_t *timer)
{
    time_t now = time(NULL);
    if (now > 100000)
    {
        struct tm timeinfo;
        if (getLocalTime(&timeinfo))
        {
            lv_arc_set_value(ui_timeSecArc, timeinfo.tm_sec * 6);
        }
    }
}

void SysCore::sideMenuShowHide(lv_timer_t *timer)
{
    if (millis() - Button::getLastPressedTime() >= 3000 && isMenuShowing == true)
    {
        isMenuShowing = false;
        menuHide_Animation(ui_sideMenu, 0);
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
    lv_anim_set_ready_cb(&a, InputdevON);  // 动画结束时的回调
    lv_anim_start(&a);
}

void SysCore::InputdevON(lv_anim_t *ani)
{
    SysCore *instance = static_cast<SysCore *>(ani->user_data);
    lv_indev_enable(instance->indev, true);
}

void SysCore::InputdevOff(lv_anim_t *ani)
{
    SysCore *instance = static_cast<SysCore *>(ani->user_data);
    lv_indev_enable(instance->indev, false);
}
