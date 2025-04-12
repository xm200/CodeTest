**Необходимые зависимости**
---
    cmake, gcc, g++, make

***Специфичные зависимости***

    Windows: ninja, choco
    Macos: brew
Если в системе каких-то зависимостей нет, 
они будут установлены автоматически.

**Установка**
---
***Linux/Macos***

    git clone https://github.com/xm200/CodeTest
    cd CodeTest
    ./configure.sh
    make
    sudo make install

***Windows***

    В консоли администратора запустить:
    git clone https://github.com/xm200/CodeTest
    cd CodeTest
    .\install.ps1

**Основные аргументы**:
---
    --help, to see this help
    Graph extracting algorithm
        -B / --bfs: use BFS (default using DFS algo)
        -D / --dfs: use DFS
    -v / --verbose: print additional info
    -o path / --output path: path to output file (default - output.txt in dir testing file)
    -C / --cfg: print CFG tree

**Использование**:
---
***Linux/Macos***

    codetest [attributes] path_to_file

***Windows***

    codetest.exe [attributes] path_to_file
