# Cocos2d-x
Game colored squares

Тестовое задание для С++ разработчика в Dankolab
Формулировка задания
	Необходимо реализовать следующую игровую механику. Игроку дается прямоугольное поле состоящее из разноцветных квадратных блоков. Необходимо удалить как можно больше блоков с игрового поля. При нажатии на любой блок нужно найти все соприкасающиеся блоки одного цвета в области нажатия пользователем. Если количество блоков больше или равно трем, то они уничтожаются. После удаления блоков, оставшиеся сверху блоки падают вниз, заполняя образовавшиеся пустые клетки. Интерфейс должен содержать кнопку запуска игры. 


Обязательно реализовать:
- Игра должна корректно запускаться на десктопе с разрешением игрового экрана 1280x1024.
- При запуске сразу начинается игра с настройками: ширина = 16, высота = 10, цвета = 3.
- Игровое поле центрируется и масштабируется на весь экран, оставляя небольшой отступ по сторонам.
- При запуске игры игровое поле полностью заполняется блоками со случайно выбранным цветом.
- Подсчет и удаление всех соседних блоков одного цвета: соседними считаются блоки, соприкасающиеся одной стороной.
- Падение оставшихся сверху блоков, заполняющее пустоты после удаления.
- Перезапуск игры по нажатию кнопки “Старт”.
  
По желанию можно добавить:
- Ввод ширины, высоты и количества цветов в интерфейсе игры;
- Проверку корректности введенных данных;
- Подсчет очков за удаленные блоки;
- Окно завершения игры, когда у игрока не осталось больше ходов;
- Анимацию удаления и падения блоков;
- Звуковые эффекты;
- Любую графику на Ваш выбор.

