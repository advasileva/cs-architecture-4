# Архитектура вычислительных систем ИДЗ 3

0. [**Задание**](#задание)
0. [**Отчёт**](#отчёт)
    + [*4 балла*](#4-балла)
    + [*5 баллов*](#5-баллов)
    + [*6 баллов*](#6-баллов)
    + [*7 баллов*](#7-баллов)
    + [*8 баллов*](#8-баллов)

## Задание

*Изучить работу с потоками. Научиться разбивать задачу на части, для последующего их выполнения различными потоками.*

*Вариант 7. Задача о курильщиках. Есть три процесса-курильщика и один процесспосредник. Курильщик непрерывно скручивает сигареты и курит их. Чтобы скрутить сигарету, нужны табак, бумага и спички. У одного процессакурильщика есть табак, у второго – бумага, а у третьего – спички. Посредник кладет на стол по два разных случайных компонента. Тот процесс-курильщик, у которого есть третий компонент, забирает компоненты со стола, скручивает сигарету и курит. Посредник дожидается, пока курильщик закончит, затем процесс повторяется. Создать многопоточное приложение, моделирующее поведение курильщиков и посредника. При решении задачи использовать семафоры.*

## Отчёт

**Делала на оценку 8 баллов**

Для удобства проверки структурировала отчёт по критериям

### 4 балла

+ *Приведено условие задачи.*

    Условие задачи приведено выше

+ *Описана модель параллельных вычислений, используемая при разработке многопоточной программы.*



+ *Описаны входные данные программы, включающие вариативные диапазоны, возможные при многократных запусках.*

    

+ *Реализовано консольное приложение, решающее поставленную задачу с использованием одного варианта синхропримитивов.*



+ *Ввод данных в приложение реализован с консоли.*



+ *Результаты работы приведены в отчете*



### 5 баллов

+ *В программу добавлены комментарии, поясняющие выполняемые действия и описание используемых переменных.*



+ *В отчете должен быть приведен сценарий, описывающий одновременное поведение представленных в условии задания сущностей в терминах предметной области. То есть, описано поведение объектов разрабатываемой программы как взаимодействующих субъектов, а не то, как это будет реализовано в программе.*

    

### 6 баллов

+ *В отчете подробно описан обобщенный алгоритм, используемый при реализации программы исходного словесного сценария. В котором показано, как на программу отображается каждый из субъектов предметной области.*

    

+ *Реализован ввод данных из командной строки.*

    

+ *Результаты изменений отражены в отчете.*

    

### 7 баллов

+ *В программу добавлены ввод данных из файла и вывод результатов в файл.*

    

+ *Приведены входные и выходные файлы с различными результатами выполнения программы.*

    

+ *Результаты работы программы должны выводиться на экран и записываться в файл.*

    

+ *Ввод данных из командной строки расширен с учетом введенных изменений.*

    

+ *Результаты изменений отражены в отчете.*

    

### 8 баллов

+ *В программу добавлена генерация случайных данных в допустимых диапазонах.*

    

+ *Приведены входные и выходные файлы с различными результатами выполнения программы.*

    

+ *Ввод данных из командной строки расширен с учетом введенных изменений.*

    

+ *Результаты изменений отражены в отчете.*

    