// Arduino-TFT_eSPI board-template main routine. There's a TFT_eSPI create+flush driver already in LVGL-9.1 but we create our own here for more control (like e.g. 16-bit color swap).

#include <lvgl.h>
#include <TFT_eSPI.h>
#include <ui.h>
#include <inputdevice.hpp>
#include <sys_core.hpp>

/*Don't forget to set Sketchbook location in File/Preferences to the path of your UI project (the parent foder of this INO file)*/

/*Change to your screen resolution*/
static const uint16_t screenWidth = 240;
static const uint16_t screenHeight = 320;

static const uint16_t lvglWidth = 320;
static const uint16_t lvglHeight = 240;

enum
{
  SCREENBUFFER_SIZE_PIXELS = lvglWidth * lvglHeight
};

// 显示缓存区
LV_ATTRIBUTE_MEM_ALIGN
static uint8_t *buf_3_1;
LV_ATTRIBUTE_MEM_ALIGN
static uint8_t *buf_3_2;
LV_ATTRIBUTE_MEM_ALIGN
static uint8_t *buf_rot;

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */
SysCore *core;

Button btn1 = Button(4);
Button btn2 = Button(5);
Button btn3 = Button(6);

#if LV_USE_LOG != 0
/* Serial debugging */
void my_print(const char *buf)
{
  Serial.printf(buf);
  Serial.flush();
}
#endif

/* Display flushing */
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *pixelmap)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  // size_t size = sizeof(pixelmap) / sizeof(pixelmap[0]);
  lv_draw_sw_rotate(pixelmap, buf_rot, w, h, w * 2, h * 2, LV_DISPLAY_ROTATION_90, LV_COLOR_FORMAT_RGB565);

  if (LV_COLOR_16_SWAP)
  {
    size_t len = lv_area_get_size(area);
    lv_draw_sw_rgb565_swap(buf_rot, len);
  }

  tft.startWrite();
  // tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.setAddrWindow(0, 0, screenWidth, screenHeight);
  tft.pushColors((uint16_t *)buf_rot, w * h, true);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}

void my_keyboard_read(lv_indev_t *indev, lv_indev_data_t *data)
{
  ButtonStatus btn1Status = btn1.Check();
  if (btn1Status != NONE)
  {
    core->showNextPage();
  }

  ButtonStatus btn2Status = btn2.Check();
  if (btn2Status != NONE)
  {
    core->showPrevPage();
  }
}

// /*Set tick routine needed for LVGL internal timings*/

void setup()
{
  Serial.begin(115200); /* prepare for possible serial debug */
  lv_init();

#if LV_USE_LOG != 0
  lv_log_register_print_cb(my_print); /* register print function for debugging */
#endif

  tft.begin();        /* TFT init */
  tft.setRotation(0); /* Landscape orientation, flipped */

  buf_3_1 = (uint8_t *)heap_caps_malloc(SCREENBUFFER_SIZE_PIXELS * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
  buf_3_2 = (uint8_t *)heap_caps_malloc(SCREENBUFFER_SIZE_PIXELS * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
  buf_rot = (uint8_t *)heap_caps_malloc(SCREENBUFFER_SIZE_PIXELS * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);

  static lv_disp_t *disp;
  disp = lv_display_create(lvglWidth, lvglHeight);
  lv_display_set_buffers(disp, buf_3_1, buf_3_2, SCREENBUFFER_SIZE_PIXELS * sizeof(lv_color_t), LV_DISPLAY_RENDER_MODE_FULL);
  lv_display_set_flush_cb(disp, my_disp_flush);

  static lv_indev_t *indev;
  indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_KEYPAD);
  lv_indev_set_read_cb(indev, my_keyboard_read);

  ui_init();

  // 初始化系统
  core = new SysCore(disp, indev);
}

void loop()
{
  
}
