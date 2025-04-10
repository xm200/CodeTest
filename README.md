**Необходимо предустановить**
---

    cmake, ninja, gcc, g++

**Установка**
---

    git clone https://github.com/xm200/CodeTest && cd CodeTest && cmake -G "Ninja" . && cmake --build .

**Использование**:
---
***Linux/Macos***

    ./CodeTest [attributes] path_to_file

***Windows***

    .\CodeTest.exe [attributes] path_to_file

***Main attributes***:

    --help, to see this help
    -B / --bfs: use BFS (default using DFS algo)
    -D / --dfs: use DFS

