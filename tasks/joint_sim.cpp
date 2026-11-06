#include <iostream>
#include <numbers>
#include <vector>
#include <fstream>


// Convert an angle from degrees to radians.
double deg2rad(double deg) {
    return deg * std::numbers::pi / 180;
}

// Saturate the control input u to the range [lo, hi]
// (models the actuator's torque limit; defaults to +/-50).
double clamp(double u, double lo = -50.0, double hi = 50.0) {
    if (u < lo) {
        return lo;          // below the lower limit -> cap at lo
    } else if (u > hi) {
        return hi;          // above the upper limit -> cap at hi
    } else {
        return u;           // within limits -> pass through unchanged
    }
}

// Time series recorded during the simulation, one entry per time step.
struct SimResult
{
    std::vector<double> q_log, dq_log, u_log;   // joint angle, joint velocity, control input
};

// Simulate a single joint driven by a PD controller toward a target angle.
//   N_step : number of simulation steps
//   dt     : time step length [s]
//   theta  : desired joint angle [deg]
SimResult run_simulation(int N_step, double dt, double theta) {

    SimResult res;

    // Allocate memory up front so push_back never has to reallocate.
    res.q_log.reserve(N_step);
    res.dq_log.reserve(N_step);
    res.u_log.reserve(N_step);

    double q_des = deg2rad(theta);          // desired joint angle [rad]
    double q = 0.0, dq = 0.0, u = 0.0;      // state: angle, velocity, control input

    for (int i = 0; i < N_step; i++) {
        double K_P = 0.15, K_D = 0.1, I = 0.05;     // P gain, D gain, joint inertia
        double err = q_des - q;                     // position error

        u   = clamp(K_P * err - K_D * dq);          // PD control law, then saturate
        dq += u / I * dt;                           // acceleration = u / I -> integrate to velocity
        q  += dq * dt;                              // integrate velocity to position

        // Record this step's values.
        res.q_log.push_back(q);
        res.dq_log.push_back(dq);
        res.u_log.push_back(u);
    }

    return res;

}

// Write the simulation log to a CSV file with columns t, q, dq, u.
void WriteCsv(const SimResult& result, double dt, const std::string& file_name = "joint_log.csv") {
    std::ofstream file(file_name);                  // open (creates/overwrites)
    if (!file) {
        std::cerr << "Could not open file\n";
        return;
    }

    file << "t,q,dq,u\n";                           // header
    for (std::size_t i = 0; i < result.q_log.size(); ++i) {
        file << i * dt << ","                       // time = step index * dt
             << result.q_log[i] << ","
             << result.dq_log[i] << ","
             << result.u_log[i] << "\n";
    }
}


int main() {

    double theta = 45.0;                            // target joint angle [deg]
    constexpr double dt = 0.001;                    // time step [s]
    const int n_step = 25000;                       // 25000 * 0.001 s = 25 s of simulated time
    std::string file_name = "joint_log.csv";        // output file

    SimResult result;
    result = run_simulation(n_step, dt, theta);     // run the simulation

    WriteCsv(result, dt, file_name);                // save the results for plotting

    return 0;
}