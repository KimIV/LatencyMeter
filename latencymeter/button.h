#include "include/AbstractEventHandler.h"

// Тайминги работы кнопки в миллисекундах
#ifndef Button_deBounce
#define Button_deBounce 15
#endif
#ifndef Button_hold
#define Button_hold 500
#endif
//#ifndef Button_long
//#define Button_long 5000
//#endif
//#ifndef Button_idle
//#define Button_idle 10000
//#endif

class Button
{

public:
  TEvent<> onClick;
  TEvent<> onClickLong;
  TEvent<> onClickLongDown;
  TEvent<> onClickLongPulse;
  TEvent<> onKeyDown;
  TEvent<> onKeyUp;

public:
  Button(byte pin)
  {
    _pin = pin;
    pinMode(_pin, INPUT_PULLUP);
  }
  /// @brief Основной цикл класса
  void Execute()
  {
    bool btnState = !digitalRead(_pin);

    if (!btnState && _flag) // Кнопка отпущена
    {
      uint32_t time = millis() - _timer;

      if (_flagEvent) // Отпускание после долгого нажатия
      {
        onKeyUp();
        onClickLongDown();
        _flag = false;
        _flagEvent = false;
        return;
      }
      if (time > Button_deBounce && !_flagEvent)	// Клик
      {
        onKeyUp();
        onClick();
        _flag = false;
        _flagEvent = false;
        return;
      }
    }

    if(btnState && _flag)	// Удерживание кнопки
    {
      uint32_t time = millis() - _timer;
      if (time > Button_hold)
      {
        if(_flagEvent)
          onClickLongPulse();
        else
          onClickLong();

        _flagEvent = true;
        return;
      }
    }

    if (btnState && !_flag && !_flagEvent)	// Нажатие кнопки
    {
      onKeyDown();
      _flag = true;
      _timer = millis();
    }
  }

private:
  byte _pin;
  uint32_t _timer = 0;
  bool _flag = false;		// флаг Кнопка нажата
  bool _flagEvent = false;	// флаг Произошло событие, ждать исходное состояние кнопки
};
