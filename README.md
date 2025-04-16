**Что такое CodeTest?**
---

---
**Это программа для выработки тестов с высоким покрытием на основе исходного кода на python версии 3 и старше.**

---
**Целевая аудитория - небольшие команды разработчиков, которые хотят протестировать свой код на неверную обработку данных.**

---

**Необходимые зависимости**
---
    cmake, gcc, g++, git

***Специфичные зависимости***

    Windows: ninja, choco
    Macos: brew, make
    Linux: make
    
**Если в системе каких-то зависимостей нет, они будут установлены автоматически. Может потребоваться пароль администратора**

**Установка**
---
***Linux/Macos***

    git clone https://github.com/xm200/codetest
    cd codetest
    bash configure.sh
    make
    sudo make install

***Windows***

    В консоли администратора запустить:
        git clone https://github.com/xm200/codetest
        cd codetest
        .\install.ps1

**Основные аргументы**:
---
    --help, увидеть эту справочную информацию
    Для алгоритма извлечения графа
        -B / --bfs: использовать BFS (по умолчанию используется алгоритм DFS)
        -D / --dfs: использовать DFS
    -v / --verbose: повысить подробность
    -o [path] / --output [path]: путь, по которому создать файл с тестами 
        по умолчанию output.txt в той же папке, в которой находится тестируемый файл
        Для выведения в стандартный поток вывода: -o -- / --output --
    -c / --cfg: Вывести CFG и выработать данные
    -C / --cfg-only: Вывести CFG и не вырабатывать данные


**Использование**:
---
***Linux/Macos***

    codetest [attributes] path_to_file

***Windows***

    .\codetest.exe [attributes] path_to_file

***Примеры***

---
    Windows:
        .\codetest.exe -v -c -o -- test.py

---
    Linux/Macos:
        codetest -o -- --cfg-only -v test.py
