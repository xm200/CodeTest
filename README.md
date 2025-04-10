*CodeTest* is used for *Code Testing*

**Requirements:**

    cmake, make, git

**Building:**

    git clone https://github.com/CodeTest 
    cd CodeTest 
    cmake .
    make
    sudo make install

**Usage:**

*Linux/Macos*

    ./CodeTest [attributes] path_to_file

*Windows*

    .\CodeTest.exe [attributes] path_to_file

**Main attributes**:

    --help, to see this help
    -B / --bfs: use BFS (default using DFS algo)
    -D / --dfs: use DFS
    -v / --verbose: print additional info
    -o path / --output path: path to output file 
            (default using output.txt in dir with file for debug)