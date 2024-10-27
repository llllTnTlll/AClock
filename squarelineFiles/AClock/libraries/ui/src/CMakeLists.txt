SET(SOURCES ui_dormancyScreen.c
    ui_clockScreen.c
    ui_weatherScreen.c
    ui_airQualityScreen.c
    ui_clockScreen1.c
    ui.c
    ui_comp_hook.c
    ui_helpers.c
    ui_img_sensor_png.c
    ui_img_clock_png.c
    ui_img_01_sunny_png.c
    ui_img_02_fewcloud_png.c)

add_library(ui ${SOURCES})
