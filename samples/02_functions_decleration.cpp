#include <iostream>
#include <cmath>
#include <numbers>

// ---- Declaration (prototype): tells the compiler the function exists ----
double deg2rad(double deg);

// ---- Definition: return type, name, typed parameters ----
double deg2rad(double deg) {
    double M_PI = std::numbers::pi; 
    return deg * M_PI / 180.0;
}

// ---- void: returns nothing ----
void printValue(const std::string& label, double v) {
    std::cout << label << " = " << v << "\n";
}

// ---- Default arguments (must be at the end) ----
double clamp(double v, double lo = -1.0, double hi = 1.0) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

// ---- Overloading: same name, different parameter types ----
int    square(int a)    { return a * a; }
double square(double a) { return a * a; }

// ---- Pass by value: the function gets a COPY ----
void tryToChange(int a) { a = 100; }

// ---- Pass by reference (preview of next topic): modifies the original ----
void reallyChange(int& a) { a = 100; } // By default, arguments are copied. & passes the original. We’ll go deeper in the next topic.

int main() {
    printValue("90 deg in rad", deg2rad(90.0));
    printValue("clamp(5)", clamp(5.0));             // uses defaults -> 1
    printValue("clamp(5, 0, 10)", clamp(5.0, 0.0, 10.0));

    std::cout << square(3) << " " << square(1.5) << "\n";  // 9 2.25

    int m = 1;
    tryToChange(m);   std::cout << m << "\n";   // 1   (copy changed)
    reallyChange(m);  std::cout << m << "\n";   // 100 (original changed)
    return 0;
}