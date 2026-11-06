#include <iostream>
#include <string>

class Joint {
public:                                   // interface: usable from outside
    // ---- Constructor with initializer list (preferred way to set members) ----
    Joint(const std::string& name, double inertia)
        : name_(name), inertia_(inertia), q_(0.0), dq_(0.0) {}

    // ---- Member function that MODIFIES the object ----
    void step(double tau, double dt) {
        dq_ += tau / inertia_ * dt;
        q_  += dq_ * dt;
    }

    // ---- const member functions: promise NOT to modify the object ----
    double q()  const { return q_; }      // getters
    double dq() const { return dq_; }
    const std::string& name() const { return name_; }  // returning a member by ref is OK

    void reset() { q_ = 0.0; dq_ = 0.0; }

private:                                  // hidden state: only the class can touch it
    std::string name_;
    double inertia_;
    double q_, dq_;                       // trailing _ = member variable (common style)
};

// ---- struct = class whose members are public by default ----
struct Gains {
    double kp = 50.0;                     // default member values
    double kd = 2.0;
};

// ---- Passing objects: const ref to read, ref to modify ----
void printJoint(const Joint& j) {         // can only call const functions on j
    std::cout << j.name() << ": q = " << j.q() << "\n";
    // j.step(1.0, 0.001);   // ❌ error: step() is not const
}

int main() {
    Joint hip("hip", 0.05);               // create an object (on the stack, no 'new')
    Gains g;                              // uses default values
    g.kp = 80.0;                          // struct: direct access

    for (int i = 0; i < 100; ++i) {
        const double tau = g.kp * (0.5 - hip.q()) - g.kd * hip.dq();
        hip.step(tau, 0.001);
    }
    printJoint(hip);

    // hip.q_ = 3.0;   // ❌ error: q_ is private
    return 0;
}