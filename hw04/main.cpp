#include <cstdio>
#include <cstdlib>
#include <vector>
#include <chrono>
#include <cmath>
#include <array>

using std::array;

static const std::size_t data_size = 48;

float frand() {
    return (float)rand() / RAND_MAX * 2 - 1;
}


array<float,data_size> px;
array<float,data_size> py;
array<float,data_size> pz;
array<float,data_size> vx;
array<float,data_size> vy;
array<float,data_size> vz;
array<float,data_size> mass;


void init() {
    for (int i = 0; i < data_size; i++) {
        px[i]= frand();
        py[i]= frand();
        pz[i]= frand();
        vx[i]= frand();
        vy[i]= frand();
        vz[i]= frand();
        mass[i]= frand()+1;
    }
}

constexpr float G = 0.001;
constexpr float eps = 0.001;
constexpr float dt = 0.01;

constexpr float eps_square  = eps * eps;


void step() {
    #pragma omp simd 
    for (std::size_t i=0; i<data_size; i++) {
        float temp_px = px[i];
        float temp_py = py[i];
        float temp_pz = pz[i];

        for (std::size_t j=0; j<data_size; j++) {
            float dx = px[j] - temp_px;
            float dy = py[j] - temp_py;
            float dz = pz[j] - temp_pz;

            float d2 = dx * dx + dy * dy + dz * dz + eps_square;
            d2 *= std::sqrt(d2);

            float temp_m = mass[j] * G * dt / d2;

            vx[i] += dx * temp_m;
            vy[i] += dy * temp_m;
            vz[i] += dz * temp_m;
        }
    }
    
    #pragma omp simd 
    for (std::size_t i=0;i<data_size;i++) {
        
        px[i]+= vx[i] * dt;
        py[i]+= vy[i] * dt;
        pz[i]+= vz[i] * dt;
    }
}

float calc() {
    float energy = 0;
    for (std::size_t i=0;i<data_size;i++) {
        float v2 = vx[i] * vx[i] + vy[i] * vy[i] + vz[i] * vz[i];
        energy += mass[i] * v2 / 2;
        for (std::size_t j=0;j<data_size;j++) {
            float dx = px[j] - px[i];
            float dy = py[j] - py[i];
            float dz = pz[j] - pz[i];
            float d2 = dx * dx + dy * dy + dz * dz + eps * eps;
            energy -= mass[j] * mass[i] * G / sqrt(d2) / 2;
        }
    }
    return energy;
}

template <class Func>
long benchmark(Func const &func) {
    auto t0 = std::chrono::steady_clock::now();
    func();
    auto t1 = std::chrono::steady_clock::now();
    auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);
    return dt.count();
}

int main() {
    init();
    printf("Initial energy: %f\n", calc());
    auto dt = benchmark([&] {
        for (int i = 0; i < 100000; i++)
            step();
    });
    printf("Final energy: %f\n", calc());
    printf("Time elapsed: %ld ms\n", dt);
    return 0;
}
