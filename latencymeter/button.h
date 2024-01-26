#include "include/AbstractEventHandler.h"

// Тайминги работы кнопки в миллисекундах
#ifndef Button_deBounce
#define Button_deBounce 25
#endif
#ifndef Button_hold
#define Button_hold 500
#endif
#ifndef Button_long
#define Button_long 1500
#endif

class Button
{

public:
  TEvent<> onClick;
  TEvent<> onClickHold;
  TEvent<> onClickLong;
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

    if (btnState && !_flag && !_flagHold && !_flagLong)	// Нажатие кнопки
    {
      onKeyDown();
      _flag = true;
      _timer = millis();
    }

    if(btnState && _flag)	// Удерживание кнопки
    {
      uint32_t time = millis() - _timer;

      if (time > Button_hold && !_flagHold && !_flagLong)
      {
        _flagHold = true;
      }
      if (time > Button_long && !_flagLong)
      {
        _flagHold = false;
        _flagLong = true;
      }
    }

    if (!btnState && _flag) // Кнопка отпущена
    {
      uint32_t time = millis() - _timer;

      if (time > Button_deBounce && !_flagHold && !_flagLong)  // Клик
      {
        onClick();
        _flag = false;
        return;
      }
      if (_flagHold) // Отпускание после долгого нажатия
      {
        onClickHold();
        _flag = false;
        _flagHold = false;
        return;
      }
      if (_flagLong) // Отпускание после долгого удержания
      {
        onClickLong();
        _flag = false;
        _flagLong = false;
        return;
      }
    }
  }

private:
  byte _pin;
  uint32_t _timer = 0;
  bool _flag = false;		// флаг Кнопка нажата
  bool _flagHold = false;	// флаг Произошло событие - удерживание кнопки
  bool _flagLong = false;	// флаг Произошло событие - долгое удерживание кнопки
};
