# Protocol for *myfind* Implementation

---

#### by Sonja Lechner and Niklas Brandtner

---

#### Overview

The *myfind* program is a command-line tool written in C that searches for files in a specified directory, either recursively (with the `-R` flag) or non-recursively. It supports searching for multiple files simultaneously by forking child processes, and allows case-insensitive searches with the `-i` flag.

------

### Parallelization and Synchronization

#### **1. Forking Child Processes**

For each file to be searched, the program forks a new child process using `fork()`. Each child independently searches for its target file in the specified search path. This enables parallel execution, improving search efficiency when looking for multiple files.

- **Process Identification**: Each child process has a unique process ID (PID), which is included in the output for clarity.

#### **2. Child Process Termination**

The parent process monitors its children using `waitpid()`, ensuring that no zombie processes are left behind when the children terminate. A custom `SIGUSR1` signal handler is used to track when a child finishes, preventing unnecessary waits.

#### **3. Output Synchronization**

Multiple processes printing to `stdout` concurrently can cause jumbled output. To avoid this, `flock()` is used to acquire an exclusive lock on `stdout` before each child prints, ensuring clean, non-overlapping output.

------

### Argument Parsing and Options

The program uses `getopt()` to handle command-line options. Users can enable recursive search with `-R` and case-insensitive search with `-i`. These options can appear anywhere in the argument list.

- **Error Handling**: The program validates input arguments and prints an error message if an invalid path or option is provided.

------

### Recursive Search and Case Insensitivity

With the `-R` flag, the program searches through directories and subdirectories recursively using `readdir()` and `opendir()`. If the `-i` flag is set, filenames are compared in a case-insensitive manner by converting strings to lowercase before comparing.
