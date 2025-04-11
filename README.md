**Необходимо предустановить**
---
    cmake, ninja, gcc, g++, make

***Специфичные зависимости***

    Windows: choco
    Macos: brew

**Установка**
---
***Общие шаги***

    git clone https://github.com/xm200/CodeTest
    cd CodeTest

***Linux/Macos***

    chmod +x install_unix.sh
    sudo install_unix.sh

***Windows***

    В консоли администратора запустить:
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

    ./CodeTest [attributes] path_to_file

***Windows***

    .\CodeTest.exe [attributes] path_to_file
