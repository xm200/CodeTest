**Необходимо предустановить**
---
    cmake, ninja, gcc, g++

**Установка**
---

    git clone https://github.com/xm200/CodeTest && cd CodeTest && cmake -G "Ninja" . && cmake --build .

***Основные аргументы***:
---
    --help, to see this help
    Graph extracting algorithm
        -B / --bfs: use BFS (default using DFS algo)
        -D / --dfs: use DFS
    -v / --verbose: print additional info
    -o path / --output path: path to output file
    (default using output.txt in dir with file for debug)

**Использование**:
---
***Linux/Macos***

    ./CodeTest [attributes] path_to_file

***Windows***

    .\CodeTest.exe [attributes] path_to_file
