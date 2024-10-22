#include <list>
#include <ui.h>
#include <string>
#include <WiFi.h>
#include <HTTPClient.h>

static void anim_ready_cb(lv_anim_t *anim);

class Page {
public:
    Page(const std::string& pageName, lv_obj_t* pagePtr)
        : _pageName(pageName), _pagePtr(pagePtr) {}

    const std::string& getPageName() const {
        return _pageName;
    }

    lv_obj_t* getPagePtr() const {
        return _pagePtr;
    }

private:
    std::string _pageName;
    lv_obj_t* _pagePtr;
};


class SysCore
{
private:

    // 页面相关
    std::list<Page*> Pages;
    Page* currentPage = nullptr;
    volatile bool isMenuShowing = true;
    // WIFI相关
    const char *ssid = "Hamamotolab-D410a";
    const char *password = "Hamamoto";
    // RTC相关
    const char *ntpServer = "pool.ntp.org";
    const long gmtOffset_sec = 9 * 3600;
    const int daylightOffset_sec = 0;
    struct tm timeinfo;
    // 位置信息
    const char *lat = "33.52";
    const char *lon = "130.48";
    // 任务相关
    TaskHandle_t uiTaskHandle;
    static void uiTask(void*);
    
    static TimerHandle_t lvgl_tick_timer;
    static void lv_tick_task(TimerHandle_t xTimer);

    // 设备
    lv_disp_t *disp;
    lv_indev_t *indev;
    static void InputdevON(lv_anim_t* ani);
    static void InputdevOff(lv_anim_t* ani);
public:
    SysCore(lv_disp_t *disp, lv_indev_t *indev);
    ~SysCore();
    void connectWifi();
    void updateRTC();
    void showNextPage();
    void showPrevPage();
    void blockInput(uint32_t ms);
};

