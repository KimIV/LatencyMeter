#include "include/AbstractEventHandler.h"

// Тайминги работы кнопки в миллисекундах
#ifndef Button_deBounce
#define Button_deBounce 25  // Защита от дребезга контактов
#endif
#ifndef Button_hold
#define Button_hold 300     // Время короткого удержания
#endif
#ifndef Button_long
#define Button_long 1500    // Время долгого удержания
#endif

class Button
{
private:
  byte _pin;
  uint32_t _timer = 0;
  bool _flag = false;    // флаг Кнопка нажата
  bool _flagHold = false; // флаг Произошло событие - удерживание кнопки
  bool _flagLong = false; // флаг Произошло событие - долгое удерживание кнопки

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
    uint32_t times = millis() - _timer;

    if (btnState && !_flag && times > Button_deBounce && !_flagHold && !_flagLong)  // Нажатие кнопки
    {
      onKeyDown();
      _flag = true;
      _timer = millis();
    }

    if(btnState && _flag) // Удерживание кнопки
    {
      times = millis() - _timer;

      if (times > Button_hold && !_flagHold && !_flagLong)
      {
        _flagHold = true;
        _flagClear = true;
      }
      if (times > Button_long && !_flagLong)
      {
        _flagHold = false;
        _flagLong = true;
        _flagClear = false;
      }
    }

    if (!btnState && _flag) // Кнопка отпущена
    {
      times = millis() - _timer;
      _timer = millis();

      if (times > Button_deBounce && !_flagHold && !_flagLong)  // Клик
      {
        onClick();
        _flag = false;
        return;
      }
      if (_flagHold) // Отпускание после короткого удержания
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
};
