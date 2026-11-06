#include <iostream>   // like: import (for printing)
#include <string>
#include <cmath>      // std::sqrt, std::sin
#include <numbers>    // std::numbers::pi (C++20)

int main() {                       // program starts here
    // ---- Types: must be declared, can't change later ----
    int    n = 10;                 // integer
    double x = 3.14;               // 64-bit float (use this, not float)
    float  f = 2.5f;               // 32-bit float (note the 'f')
    bool   ok = true;              // true / false (lowercase)
    char   c = 'A';                // single char: single quotes
    std::string name = "robot";    // text: double quotes

    auto y = 2.0;                  // compiler deduces double
    auto k = 2;                    // compiler deduces int  (!)

    const double g = 9.81;         // cannot be modified
    constexpr int N = 100;         // compile-time constant (for sizes, etc.)

    // ---- Pitfall 1: integer division ----
    std::cout << 7 / 2   << "\n";  // 3     (int / int -> int)
    std::cout << 7 / 2.0 << "\n";  // 3.5
    std::cout << static_cast<double>(n) / 4 << "\n";  // 2.5 (explicit cast)

    // ---- Pitfall 2: uninitialized variables = garbage ----
    int bad;                       // value is random! always initialize
    int good = 0;

    // ---- Operators ----
    n += 5;  n++;                  // n = 16
    int r = n % 3;                 // remainder
    bool both = ok && (x > 3.0);   // && and, || or, ! not

    // ---- Control flow ----
    if (x > 3.0) {
        std::cout << "big\n";
    } else if (x > 1.0) {
        std::cout << "medium\n";
    } else {
        std::cout << "small\n";
    }

    for (int i = 0; i < 3; ++i) {  // like: for i in range(3)
        std::cout << "i = " << i << "\n";
    }

    int count = 0;
    while (count < 3) { count++; }

    // ---- Scope: variables live inside their { } ----
    {
        int inner = 5;
    }
    // inner does not exist here

    std::cout << name << " " << c << " " << good << " " << r << " " << both << "\n";
    std::cout << "sqrt(2) = " << std::sqrt(2.0) << ", pi = " << std::numbers::pi << "\n";
    return 0;                      // 0 = success
}