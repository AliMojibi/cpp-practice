# How to Run a C++ Program

C++ is a **compiled** language. Running a program takes two steps:

1. **Compile:** the compiler (`g++`) turns your `.cpp` source file into an `.exe` program.
2. **Run:** you run that `.exe`.

## Step 0: Write a program

Save this as `hello.cpp`:

```cpp
#include <iostream>

int main() {
    std::cout << "Hello, C++!\n";
    return 0;
}
```

## Step 1: Compile

Open a terminal (PowerShell) in the folder that holds your file and run:

```powershell
g++ -std=c++20 -Wall -Wextra hello.cpp -o hello.exe
```

| Part              | Meaning                                       |
|-------------------|-----------------------------------------------|
| `g++`             | the C++ compiler                              |
| `-std=c++20`      | use the C++20 standard                        |
| `-Wall -Wextra`   | show warnings (they help you catch mistakes)  |
| `hello.cpp`       | your source file                              |
| `-o hello.exe`    | name of the program to create                 |

If nothing is printed, it worked, and `hello.exe` now exists.

## Step 2: Run

```powershell
.\hello.exe
```

Output:

```
Hello, C++!
```

> In PowerShell, you must put `.\` before the program name.

## Both steps in one line

```powershell
g++ -std=c++20 -Wall -Wextra hello.cpp -o hello.exe; if ($?) { .\hello.exe }
```

The program only runs if compiling succeeded.

## Common problems

- **Errors when compiling:** read the **first** error. It shows the file and line number
  (e.g. `hello.cpp:4:5: error: ...`). Later errors are often caused by the first one.
- **Changed the code but the output didn't change:** you must **compile again** after every change.
- **`hello.exe is not recognized`:** you forgot the `.\` → use `.\hello.exe`.
- **`g++ is not recognized`:** the compiler isn't on your PATH. Add `C:\msys64\ucrt64\bin` to PATH
  and reopen the terminal.

## Easier option: VS Code

Install the **C/C++** extension (by Microsoft), open a `.cpp` file, and click the ▶ button
(**Run C/C++ File**) in the top-right corner. It compiles and runs in one click, and lets you
debug with breakpoints.
