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
    chmod +x configure.sh
    ./configure.sh
    make
    sudo make install

***Windows***

    В консоли администратора запустить:
    git clone https://github.com/xm200/codetest
    cd codetest
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
        .\codetest.exe -c -v -o -- test.py

---
    Linux/Macos:
        codetest --cfg-only -v -o -- test.py

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
