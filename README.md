# Latency Meter Camera

Устройство для измерения раздержки различных камер и FPV-систем. Оно построено на плате Ардуино Нано и 4х разрядном 7ми-сегментном индиаторе LED 5461AS-1.

**Кнопка:** Длинное нажатие на кнопку запускает процесс измерения сначала. Короткое - переключает показания индикатора между real-time, min, max, sma.

**Внимание!** Программный код поддерживает ТРИ разных взаимных расположения индкатора LED 5461AS и Arduino Nano. Выберите для себя нужную компоновку и задайте правильное значение константы LOCATION_LED5461AS в файле latencymeter.ino. Там же читайте подробности в комментариях перед определением константы LOCATION_LED5461AS.

Видео по сборке для компоновки LOCATION_LED5461AS 0: [https://www.youtube.com/watch?v=69uGeqPZ3CI](https://www.youtube.com/watch?v=69uGeqPZ3CI)

![1701351332899](images/README/1701351332899.png)![1701351013962](images/README/1701351013962.png)

22.01.2024
Добавлены 3Д модели (папка 3D_Model_OutSensors) корпуса устройства для варианта сборки с выносными фоторезистором и светодиодом. Этот вариант удобнее для измерения задержек FPV-систем от курсовой камеры на квадрокоптере до шлема или очков.
