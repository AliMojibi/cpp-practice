# How to Run a C++ Program

C++ is a **compiled** language. Running a program takes two steps:

1. **Compile:** the compiler (`g++`) turns your `.cpp` source file into an `.exe` program.
2. **Run:** you run that `.exe`.

The steps below are for **Windows (PowerShell)**. On Ubuntu, see [On Ubuntu (Linux)](#on-ubuntu-linux).

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

## On Ubuntu (Linux)

It works the same way. Only the setup, the file name and the run command change.

**Install the compiler (once):**

```bash
sudo apt update
sudo apt install build-essential
g++ --version        # check it worked
```

**Compile:** Linux programs don't need an extension, so there is no `.exe`:

```bash
g++ -std=c++20 -Wall -Wextra hello.cpp -o hello
```

**Run:** use `./` (forward slash):

```bash
./hello
```

**Both steps in one line:** `&&` runs the program only if compiling succeeded:

```bash
g++ -std=c++20 -Wall -Wextra hello.cpp -o hello && ./hello
```

| | Windows (PowerShell) | Ubuntu (bash) |
|---|---|---|
| Install | MSYS2 → `g++` | `sudo apt install build-essential` |
| Output file | `hello.exe` | `hello` |
| Run | `.\hello.exe` | `./hello` |
| Path separator | `samples\hello.cpp` | `samples/hello.cpp` |
| Chain commands | `; if ($?) { .\hello.exe }` | `&& ./hello` |

> **Tip:** Linux programs have no extension, so `.gitignore` won't catch them like it does `*.exe`.
> Put them in the `build/` folder (already ignored): `mkdir -p build`, then
> `g++ ... hello.cpp -o build/hello && ./build/hello`.

## Easier option: VS Code

Install the **C/C++** extension (by Microsoft), open a `.cpp` file, and click the ▶ button
(**Run C/C++ File**) in the top-right corner. It compiles and runs in one click, and lets you
debug with breakpoints.
