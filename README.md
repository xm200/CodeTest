**Необходимые зависимости**
---
    cmake, gcc, g++, make, git

***Специфичные зависимости***

    Windows: ninja, choco
    Macos: brew
    
**Если в системе каких-то зависимостей нет, они будут установлены автоматически.**

**Установка**
---
***Linux/Macos***

    git clone https://github.com/xm200/CodeTest
    cd CodeTest
    sudo ./installer_mac_linux.sh

***Windows***

    В консоли администратора запустить:
    git clone https://github.com/xm200/CodeTest
    cd CodeTest
    .\install.ps1

**Использование**:
---
***Linux/Macos***

    codetest [attributes] path_to_file

***Windows***

    .\codetest.exe [attributes] path_to_file

***Примеры***

---
    Windows:
        .\codetest.exe -c Testing/test1_2.py
        cat Testing/output.txt

---
    Linux/Macos:
        codetest --cfg-only Testing/test3.py
        cat Testing/output.txt

---

**Основные аргументы**:
---
    --help, to see this help
    Graph extracting algorithm
        -B / --bfs: use BFS (default using DFS algo)
        -D / --dfs: use DFS
    -v / --verbose: print additional info
    -o path / --output path: path to output file 
        default - output.txt in dir with testing file
        if you write -o -- / --output --, generated data will be written to stdout
    -c / --cfg: print CFG tree
    -C / --cfg-only: print CFG tree without generating data
