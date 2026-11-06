#include <iostream>
#include<vector>
#include<fstream>

class Table {
public:
    Table(double init_width, double init_height) {
        if (init_height < 0 || init_width < 0)
            abort();

        width = init_width;
        height = init_height;
    }

    bool contains_ball(double x, double y); 
    void reflect(double& x, double& y, double& vx, double& vy);
    double get_width()  { return width;  }
    double get_height() { return height; }

private:
    double width;
    double height;

};

bool Table::contains_ball(double x, double y) { 
    return x >= 0 && x < width && y >= 0 && y < height;

}

void Table::reflect(double& x, double& y, double& vx, double& vy) {
    if (x < 0) {
        x  = -x;
        vx = -vx;
    }
    if (x >= width) {
        x  = 2 * width - x;
        vx = -vx;
    }
    if (y < 0) {
        y  = -y;
        vy = -vy;
    }
    if (y >= height) {
        y  = 2 * height - y;
        vy = -vy;
    }
}

class Ball {
public:
    Ball(double _x, double _y, double _vx, double _vy, Table* t){

        if (_x < 0 || _y < 0 || _x > t->get_width() || _y > t->get_height())
            abort();

        x     =  _x;
        y     =  _y;
        vx    = _vx;
        vy    = _vy;
        table =   t;

    }

    void move(double dt);
    double get_x()  { return x; }
    double get_y()  { return y; }
    double get_vx() { return vx; }
    double get_vy() { return vy; }

private:
    double x;
    double y;
    double vx;
    double vy;
    Table* table;
};

void Ball::move(double dt){
    x += vx * dt;
    y += vy * dt;
    
    while (!table->contains_ball(x, y)) {
        table->reflect(x, y, vx, vy);
    }

}

struct SimResult {
    std::vector<double> x_log, y_log, vx_log, vy_log;
};

SimResult run_simulation(Ball& b, int& steps, double& dt) {

    SimResult res;

    for (int i = 0; i <= steps; ++i) {
        b.move(dt);
        res.x_log.push_back(b.get_x());
        res.y_log.push_back(b.get_y());
        res.vx_log.push_back(b.get_vx());
        res.vy_log.push_back(b.get_vy());
        }    
    
    return res;
}

void write_csv(SimResult& result, double dt, std::string file_name = "results.csv") {
    std::ofstream file(file_name);                  // open (creates/overwrites)
    if (!file) {
        std::cerr << "Could not open file\n";
        return;
    }

    file << "t,x,y,vx,vy\n";                           // header
    for (std::size_t i = 0; i < result.x_log.size(); ++i) {
        file << i * dt << "," 
             << result.x_log[i]<< ","                     // time = step index * dt
             << result.y_log[i] << ","
             << result.vx_log[i] << ","
             << result.vy_log[i] << "\n";
    }


}

int main(){

    int n_steps = 5000;
    double dt = 0.1;

    Table my_table(100, 50);
    Ball my_ball(5, 20, 0.5, 0.5, &my_table);
    
    SimResult res = run_simulation(my_ball, n_steps, dt);
    write_csv(res, dt);

    
    return 0;
}