# C++ Session Notes

Compile single files:
```bash
g++ -std=c++17 -Wall -Wextra main.cpp -o main && ./main
```
Fix the **first** compiler error first. Later errors are often caused by it.

---

## 1. Basic Syntax and Types

```cpp
int n = 10;  double x = 3.14;  bool ok = true;  char c = 'A';
std::string name = "robot";
auto k = 2;              // int (!)   auto y = 2.0;  // double
const double g = 9.81;   // cannot change
constexpr int N = 100;   // compile-time constant
```
- Every statement ends with `;`. Blocks use `{ }`.
- **Integer division:** `7 / 2 == 3`, `7 / 2.0 == 3.5`, `static_cast<double>(n) / 4`.
- **Always initialize** variables. Uninitialized = garbage.
- Variables live only inside their `{ }` (scope).

### Initializing multiple variables
```cpp
double q = 0.0, dq = 0.0, u = 0.0;   // ✅ each gets a value
double q, dq, u = 0.0;               // ❌ only u is initialized
double q{0.0}, dq{0.0};              // brace init: blocks narrowing (int n{3.7} → error)
```

---

## 2. Functions

```cpp
double deg2rad(double deg);                                   // declaration (prototype)
double deg2rad(double deg) { return deg * M_PI / 180.0; }     // definition
void printValue(const std::string& label, double v);          // void = no return
double clamp(double v, double lo = -1.0, double hi = 1.0);    // defaults must be last
int square(int a);  double square(double a);                  // overloading
```
- Declare a function **before** using it (define it above `main`, or write a prototype).
- You **can't define a function inside another function**.
- Arguments are **copied** by default.
- No keyword arguments; defaults can only be left off from the right.

### Returning multiple values
```cpp
return q_log, dq_log, u_log;   // ❌ comma operator → returns only u_log
```
Use a **struct**:
```cpp
struct SimResult { std::vector<double> q, dq, u; };
SimResult runSimulation(...) { SimResult res; ...; return res; }  // no copy (RVO)
```
(Alternative: `std::tuple` + `auto [a, b, c] = f();`)

---

## 3. Arrays: `std::vector` and `std::array`

```cpp
std::vector<double> q_log;          // ✅ type, then name (not std::vector<double>::q_log)
q_log.reserve(N);                   // allocate once, before the loop (real-time habit)
q_log.push_back(q);                 // like append()
q_log.size();  q_log[i];  q_log.back();   // len, index, last element
std::vector<double> v(N, 0.0);      // like np.zeros(N)

std::array<double, N> a{};          // fixed size; N must be constexpr; stack, no allocation
```
- `::` = scope operator ("look inside"), e.g. `std::vector`.
- Use `std::size_t i` when comparing with `.size()` (avoids a sign warning).
- Avoid C-style arrays (`double a[200];`).

---

## 4. Plotting: C++ → CSV → Python

```cpp
#include <fstream>
void writeCsv(const SimResult& res, double dt,
              const std::string& file_name = "joint_log.csv") {
    std::ofstream file(file_name);
    if (!file) { std::cerr << "Could not open " << file_name << "\n"; return; }
    file << "t,q,dq,u\n";
    for (std::size_t i = 0; i < res.q.size(); ++i)
        file << i * dt << "," << res.q[i] << "," << res.dq[i] << "," << res.u[i] << "\n";
}   // file closes automatically
```
Then plot with pandas + matplotlib in Python.

---

## 5. Simulation Loop: Review Lessons

```cpp
double q = 0.0, dq = 0.0;                     // state declared ONCE, before the loop
for (int i = 0; i < n_steps; ++i) {
    q_log.push_back(q);  dq_log.push_back(dq);           // log BEFORE update → t = i*dt
    const double u = clamp(kp * (q_des - q) - kd * dq,   // D term is MINUS (damping)
                           -tau_max, tau_max);           // explicit limits
    u_log.push_back(u);
    dq += u / inertia * dt;                    // semi-implicit (symplectic) Euler:
    q  += dq * dt;                             // update dq first, then q with the new dq
}
```
- ❌ `+kd*dq` → negative damping → unstable.
- ❌ State declared inside the loop → reset to 0 every step.
- Keep variables in the smallest scope (`const double err` inside the loop).
- Pick one naming style: `camelCase` or `snake_case`.

---

## 6. References and Const References

```cpp
double q = 1.0;
double& r = q;          // r IS q (alias); must be initialized; can't be re-bound
r = 5.0;                // q == 5

void byValue(double x);                       // copy
void byRef(double& x);                        // modifies the original
void byConstRef(const std::vector<double>& v); // no copy, read-only
```
| Type | Read only | Modify |
|---|---|---|
| Small (`int`, `double`) | `double x` | `double& x` |
| Large (vector, string, Eigen, struct) | `const T& x` | `T& x` |

- Never return a reference to a local variable (dangling reference).
- In Python, everything is a reference. In C++, **copy is the default**.
- Pointers (`double* p = &q;`) can be null or re-pointed. Seen later as smart pointers in ROS 2.

### Range-based for loop: `:` means "in"
```cpp
for (double e : data)          // copy of each element
for (double& e : data)         // alias → modifies the elements
for (const auto& e : data)     // read-only alias, no copy (large elements)
for (std::size_t i = 0; i < data.size(); ++i)   // use when you need the index
```

---

## 7. Classes

```cpp
class Joint {
public:
    Joint(const std::string& name, double inertia)
        : name_(name), inertia_(inertia), q_(0.0), dq_(0.0) {}   // initializer list
    void step(double tau, double dt) { dq_ += tau / inertia_ * dt; q_ += dq_ * dt; }
    double q() const { return q_; }            // const = doesn't modify the object
private:
    std::string name_;
    double inertia_, q_, dq_;                  // trailing _ = member variable
};                                             // ← semicolon!

struct Gains { double kp = 50.0; double kd = 2.0; };   // struct = public by default
```
| Python | C++ |
|---|---|
| `__init__` | constructor + initializer list |
| `self.q` | `q_` (or `this->q_`) |
| `_q` (convention) | `private:` (enforced) |
| `j = Joint("hip", 0.05)` | `Joint j("hip", 0.05);` (stack, no `new`) |

- Keep data `private` and expose functions.
- Mark non-modifying functions `const`. Only `const` functions can be called on a `const Joint&`.
- `struct` for plain data, `class` for behavior + hidden state.

---

## Open Tasks
- [ ] `joint_sim.cpp`: PD joint simulation + CSV + plot, final angle in degrees, error < 0.5°
- [ ] Quick check 1 (types/functions): `5/2`, `tryToChange`, prototype, `auto t = 0`
- [ ] Refactor into classes: `PDGains`, `PDController` (`mutable` question), `Joint`, `SimResult`, `runSimulation`, `writeCsv`; simulate hip + knee
- [ ] Quick check 2 (references/classes): 5 questions

## Next
Header vs. source files → Eigen → `KalmanFilter` class
