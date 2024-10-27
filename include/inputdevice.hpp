#ifndef INPUTDEVICE_HPP
#define INPUTDEVICE_HPP

#include <Arduino.h>
#include <vector>

typedef enum{
    NONE,
    PRESSED,
    LONG_PRESSED,
    LONG_PRESSING,
}ButtonStatus;

class Button
{
private:
    uint16_t pinIndex;
    const int debounceDelay = 10;       // 软件消抖延时

    int lastButtonState = HIGH;         // 上一次的按键状态
    int buttonState = HIGH;             // 当前按键状态

    unsigned long lastDebounceTime = 0; // 上次抖动时间
    unsigned long pressedTime = 0;      // 按键按下的时间
    bool isLongPressing = false;        // 用于标记是否正在长按

    static unsigned long lastBtnResponse;

public:
    Button(uint16_t pinIndex);
    ButtonStatus Check();

    static unsigned long getLastPressedTime()
    {
        return Button::lastBtnResponse;
    }
};


#endif
