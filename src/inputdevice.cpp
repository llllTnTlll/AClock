#include <inputdevice.hpp>

Button::Button(uint16_t pinIndex)
{
    this->pinIndex = pinIndex;
    pinMode(this->pinIndex, INPUT_PULLUP);
}

ButtonStatus Button::Check()
{
    ButtonStatus status = NONE;

    int reading = digitalRead(pinIndex);

    // 检测是否存在按键状态变化
    if (reading != lastButtonState)
    {
        lastDebounceTime = millis();
    }

    // 只有在消抖延时之后，按键状态才会被确认改变
    if ((millis() - lastDebounceTime) > debounceDelay)
    {
        if (reading != buttonState)
        {
            buttonState = reading;

            // 检测按键从未按下变为按下的状态变化
            if (buttonState == LOW)
            {
                pressedTime = millis(); // 记录按下时间
                isLongPressing = false; // 重置长按标志
            }

            // 检测按键松开的状态变化
            else if (buttonState == HIGH)
            {
                unsigned long releasedTime = millis();
                unsigned long pressDuration = releasedTime - pressedTime;

                if (pressDuration < 500)
                {
                    Serial.println("短按");
                    status = PRESSED;
                }
                isLongPressing = false; // 重置长按标志
            }
        }
    }

    // 检测按键保持按下状态
    if (buttonState == LOW && (millis() - pressedTime) >= 500 && !isLongPressing)
    {
        status = LONG_PRESSED;
        isLongPressing = true; // 标记为长按
    }
    else if (buttonState == LOW && isLongPressing)
    {
        status = LONG_PRESSING;
    }

    // 更新上一次按键状态
    lastButtonState = reading;
    return status;
}
