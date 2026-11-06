// references_demo.cpp

#include <iostream>   // std::cout (printing)
#include <vector>     // std::vector (dynamic array, like a Python list)
#include <string>     // std::string (text)

// ============================================================================
// SYNTAX NOTES
//   T&        -> "reference to T": another name (alias) for an existing variable
//   const T&  -> "read-only reference to T": alias that cannot modify the original
//   std::     -> "::" means "look inside"; std:: = inside the standard library
//   { a, b }  -> brace list: initializes a container with values
//   :         -> in a range-based for loop, means "in" (like Python's "for e in data")
// ============================================================================

// ---- 3 ways to pass arguments ----

// By value: x is a NEW COPY of the argument. Changing x does not affect the caller.
void byValue(double x) { x = 0.0; }

// By reference: "double&" means x IS the caller's variable (same memory).
// Changing x changes the original.
void byRef(double& x) { x = 0.0; }

// By const reference: v is an alias to the caller's vector (no copy is made),
// but "const" makes it read-only inside this function.
void byConstRef(const std::vector<double>& v) {
    std::cout << "size = " << v.size() << "\n";   // .size() = number of elements, like len(v)
    // v.push_back(1.0);   // ❌ compile error: v is const, cannot be modified
}

// ---- Returning results through reference parameters ----
// v is read-only input; mn and mx are OUTPUTS written into the caller's variables.
void minMax(const std::vector<double>& v, double& mn, double& mx) {
    mn = v[0];                 // v[0] = first element (indexing like Python)
    mx = v[0];

    // Range-based for loop:  for (TYPE NAME : CONTAINER)
    //   read as: "for each element e IN v"
    //   "double e" -> e is a COPY of each element (fine for small types like double)
    for (double e : v) {
        if (e < mn) mn = e;
        if (e > mx) mx = e;
    }
}

// ---- ❌ Danger: never return a reference to a local variable ----
// tmp is destroyed when the function ends, so the returned reference would
// point to memory that no longer exists ("dangling reference").
// double& bad() { double tmp = 1.0; return tmp; }

int main() {
    // ---- A reference is another name for an existing variable ----
    double q = 1.0;
    double& r = q;      // r is an alias of q: they share the same memory.
                        // A reference MUST be initialized and can never be re-bound.
    r = 5.0;            // writing to r writes to q
    std::cout << q << "\n";             // 5    ("\n" = new line)

    double a = 3.0;
    byValue(a);    std::cout << a << "\n";   // 3   (function changed only its copy)
    byRef(a);      std::cout << a << "\n";   // 0   (function changed the original)

    // Brace initialization: creates a vector with these 4 values
    std::vector<double> data = {3.0, -1.0, 7.5, 2.0};
    byConstRef(data);                   // passes an alias, no copy of the vector

    double lo = 0.0, hi = 0.0;
    minMax(data, lo, hi);               // lo and hi are filled in by the function
    std::cout << lo << " " << hi << "\n";    // -1 7.5

    // ---- Range-based for loops with references ----
    // "double& e : data" -> e is an ALIAS to each element IN data,
    // so "e *= 2.0" doubles the actual elements inside the vector.
    for (double& e : data) e *= 2.0;    // data is now {6, -2, 15, 4}

    // "const auto& e : data" -> e is a READ-ONLY alias to each element IN data.
    //   auto   = let the compiler deduce the type (here: double)
    //   const& = no copy, and no modification allowed
    // (A one-line loop body doesn't need { }.)
    for (const auto& e : data) std::cout << e << " ";
    std::cout << "\n";                  // 6 -2 15 4

    // A const reference can bind to a temporary value.
    // std::string("robot") is a temporary object; binding it to a const&
    // keeps it alive as long as s exists.
    const std::string& s = std::string("robot");
    std::cout << s << "\n";

    return 0;   // 0 = program finished successfully
}