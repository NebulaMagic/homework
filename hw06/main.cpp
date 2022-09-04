#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include "ticktock.h"
#include <tbb/tbb.h>
#include <tbb/concurrent_vector.h>
#include <mutex>
#include <tbb/spin_mutex.h>
#include <atomic>

#define use_tbb
// TODO: 并行化所有这些 for 循环

template <class T, class Func>
std::vector<T> fill(std::vector<T> &arr, Func const &func) {
    TICK(fill);

    #ifndef use_tbb

    for (size_t i = 0; i < arr.size(); i++) {
        arr[i] = func(i);
    }

    #else

    tbb::parallel_for((size_t)0,arr.size(),
    [&](size_t i){
        arr[i] = func(i);
    });

    #endif

    TOCK(fill);

    return arr;
}

template <class T>
void saxpy(T a, std::vector<T> &x, std::vector<T> const &y) {
    TICK(saxpy);

    #ifndef use_tbb

    for (size_t i = 0; i < x.size(); i++) {
       x[i] = a * x[i] + y[i];
    }

    #else

    tbb::parallel_for(tbb::blocked_range<size_t>(0,x.size()),
    [&](tbb::blocked_range<size_t> r){
        for (size_t i = r.begin(); i < r.end(); i++) {
            x[i] = a * x[i] + y[i];
        }
    });

    #endif

    TOCK(saxpy);
}

template <class T>
T sqrtdot(std::vector<T> const &x, std::vector<T> const &y) {
    TICK(sqrtdot);

    #ifndef use_tbb

    T ret = 0;
    for (size_t i = 0; i < std::min(x.size(), y.size()); i++) {
        ret += x[i] * y[i];
    }
    ret = std::sqrt(ret);  

    #else   

    T ret  = tbb::parallel_reduce(tbb::blocked_range<size_t>(0,std::min(x.size(), y.size())),T{0},
    [&](tbb::blocked_range<size_t>r, T local_res){
        for(size_t i = r.begin(); i < r.end(); i++){
            local_res += x[i] * y[i];
        }
        return local_res;
    },
    [](T x, T y){
        return x + y;
    });
    ret = std::sqrt(ret);  

    #endif

    TOCK(sqrtdot);
    return ret;
}

template <class T>
T minvalue(std::vector<T> const &x) {
    TICK(minvalue);

    #ifndef use_tbb

    T ret = x[0];
    for (size_t i = 1; i < x.size(); i++) {
        if (x[i] < ret)
            ret = x[i];
    }

    #else 

    tbb::concurrent_vector<T> temp_res;
    tbb::parallel_for(tbb::blocked_range<size_t>(0,x.size()),
    [&](tbb::blocked_range<size_t> r){
        T temp_min = x[r.begin()];
        for(size_t i = r.begin(); i < r.end(); i++)
        {
            if (x[i] < temp_min)
                temp_min = x[i];
        }
        temp_res.push_back(temp_min);
    });
    
    T ret = temp_res[0];
    for (size_t i = 1; i < temp_res.size(); i++) {
        if (temp_res[i] < ret)
            ret = temp_res[i];
    }
    
    #endif
    TOCK(minvalue);
    return ret;
}

template <class T>
std::vector<T> magicfilter(std::vector<T> const &x, std::vector<T> const &y) {
    TICK(magicfilter);
    #ifndef use_tbb

    std::vector<T> res;
    for (size_t i = 0; i < std::min(x.size(), y.size()); i++) {
        if (x[i] > y[i]) {
            res.push_back(x[i]);
        } else if (y[i] > x[i] && y[i] > 0.5f) {
            res.push_back(y[i]);
            res.push_back(x[i] * y[i]);
        }
    }

    #else

    std::vector<T> res;
    std::mutex lk;
    tbb::spin_mutex slk;
    std::atomic<size_t> a_size = 0;

    // res.reserve(std::min(x.size(), y.size()) * 2 / 3);
    // tbb::task_arena ta(4);
    // ta.execute([&]{
    //     tbb::parallel_for(tbb::blocked_range<size_t>(0,std::min(x.size(), y.size())),
    //     [&](tbb::blocked_range<size_t>r){
    //         std::vector<T> local_temp;
    //         local_temp.reserve(r.size());
    //         for(size_t i = r.begin(); i < r.end(); i++)
    //         {
    //             if (x[i] > y[i]) {
    //                 local_temp.push_back(x[i]);
    //             } else if (y[i] > x[i] && y[i] > 0.5f) {
    //                 local_temp.push_back(y[i]);
    //                 local_temp.push_back(x[i] * y[i]);
    //             }           
    //         }

    //         std::lock_guard lck(slk);
    //         std::copy(local_temp.begin(),local_temp.end(),std::back_inserter(res));
    //     },tbb::static_partitioner{});
    // });

    res.resize(std::min(x.size(), y.size()));
    tbb::task_arena ta(4);
    ta.execute([&]{
        tbb::parallel_for(tbb::blocked_range<size_t>(0,std::min(x.size(), y.size())),
        [&](tbb::blocked_range<size_t>r){
            std::vector<T> local_temp(r.size());
            size_t lasize = 0;
            for(size_t i = r.begin(); i < r.end(); i++)
            {
                if (x[i] > y[i]) {
                    local_temp[lasize++] = x[i];
                } else if (y[i] > x[i] && y[i] > 0.5f) {
                    local_temp[lasize++] = y[i];
                    local_temp[lasize++] = x[i] * y[i];
                }           
            }

            size_t base = a_size.fetch_add(lasize);
            for(size_t i = 0; i < lasize; i++)
                res[base + i] = local_temp[i];

        },tbb::static_partitioner{});
    });

    #endif
    TOCK(magicfilter);
    return res;
}

template <class T>
T scanner(std::vector<T> &x) {
    TICK(scanner);

    #ifndef use_tbb
    T ret = 0;
    for (size_t i = 0; i < x.size(); i++) {
        ret += x[i];
        x[i] = ret;
    }

    #else
    
    T ret = tbb::parallel_scan(tbb::blocked_range<size_t>(0,x.size()),T{0},
    [&](tbb::blocked_range<size_t>r, T local_res, auto is_final){
        for(size_t i = r.begin(); i < r.end(); i++){
            local_res += x[i];
            if(is_final){
                x[i] = local_res;
            }
        }
        return local_res;
    },
    [](T x, T y){
        return x + y;
    });

    #endif

    TOCK(scanner);
    return ret;
}

int main() {
    size_t n = 1<<26;
    std::vector<float> x(n);
    std::vector<float> y(n);

    fill(x, [&] (size_t i) { return std::sin(i); });
    fill(y, [&] (size_t i) { return std::cos(i); });

    saxpy(0.5f, x, y);

    std::cout << sqrtdot(x, y) << std::endl;
    std::cout << minvalue(x) << std::endl;

    auto arr = magicfilter(x, y);
    std::cout << arr.size() << std::endl;

    scanner(x);
    std::cout << std::reduce(x.begin(), x.end()) << std::endl;

    return 0;
}
