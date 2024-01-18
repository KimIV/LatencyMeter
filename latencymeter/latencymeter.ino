#define F_CPU 16000000UL  // частота работы микроконтроллера
#define PIN_BUTTON 10     // Кнопка

// LOCATION_LED5461AS определяет взаимное расположение Led 5461AS и Arduino Nano
// при двухэтажной компоновке:
//  0 - оригинальная компоновка автора проекта https://github.com/githubniko/LatencyMeter
//      Смотри схему Schematic_Latencycam_2023-11-22.pdf
//  1 - Led 5461AS сдвинут на два контакта в сторону от miniUSB относительно оригинала.
//      Смотри схему Schematic_LatencyMeter_Loc1_2024-01-18.pdf
//  2 - Led 5461AS перевёрнут на 180 градусов относительно оригинала.
//      Смотри схему Schematic_LatencyMeter_Loc2_2024-01-18.pdf
#define LOCATION_LED5461AS 0

#if (LOCATION_LED5461AS == 0)
#include "Led_5461AS.h"
#elif (LOCATION_LED5461AS == 1)
#include "Led_5461AS_2.h"
#else
#include "Led_5461AS_180.h"
#endif
#include "Button.h"
#include "LatencyMeter.h"

Led_5461AS *led;
LatencyMeter *latencyMeter;
Button button1(PIN_BUTTON);

uint8_t flagTypeDataOut = 3; // 0 - задержка рела-тайм, 1 - минимальная, 2 - максимальная, 3 - медиана, 4 - кол-во измерений

class EventHandler
{
public:
  void OnUpdateData()
  {
    char buf[33];
    char str[10];

    switch (flagTypeDataOut)
    {
    case 1:
      dtostrf(latencyMeter->minTime, 3, 0, buf);
      strcpy(str, "L");
      break;
    case 2:
      dtostrf(latencyMeter->maxTime, 3, 0, buf);
      strcpy(str, "H");
      break;
    case 3:
      dtostrf(latencyMeter->medianTime, 3, 0, buf);
      strcpy(str, "^");
      break;
    case 4:
      itoa(latencyMeter->count, buf, 10);
      strcpy(str, "n");
      break;
    case 0:
    default:
      dtostrf(latencyMeter->valueTime, 4, 0, buf);
      strcpy(str, "");
      break;
    }
    strcat(str, buf);
    led->Set(str); // вывод на экран
  }
  void OnBtnClick()
  {
    flagTypeDataOut++;
    if (flagTypeDataOut > 4)
      flagTypeDataOut = 0;
    
    OnUpdateData();
  }
  void OnBtnReset()
  {
    latencyMeter->Start();
    // asm volatile("jmp 0x00"); // Перезагрузка
  }
};
EventHandler eventHandler;

void setup()
{
  Serial.begin(115200);

  led = new Led_5461AS();
  latencyMeter = new LatencyMeter();
  latencyMeter->onUpdate += METHOD_HANDLER(eventHandler, EventHandler::OnUpdateData);

  button1.onClick += METHOD_HANDLER(eventHandler, EventHandler::OnBtnClick);
  button1.onClickLong += METHOD_HANDLER(eventHandler, EventHandler::OnBtnReset);

  //latencyMeter->Start();
}

void loop()
{
  button1.Execute();
  latencyMeter->Execute();
}
