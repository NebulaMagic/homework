1. 主机 AMD 3800X 3.90 GHz 8核16线程   RAM 16GB  Windows 10 MSVC2019

原程序运行打印：

    Initial energy: -8.571527
    Final energy: -8.511734
    Time elapsed: 3270 ms

忧化后：

    Initial energy: -8.571527
    Final energy: -8.511688
    Time elapsed: 5362 ms

搞了好久无论怎么搞都是负优化，未知 （ﾉ´д｀）


2. 腾讯云小小服务器 Intel Platinum 8255C 2.50GHz  2核2线程   RAM 4GB  Linux VM-20-15-ubuntu 5.4.0   GCC 9.4.0

原程序运行打印：

    Initial energy: -13.414000
    Final energy: -13.356842
    Time elapsed: 1760 ms

优化后：

    Initial energy: -13.413999
    Final energy: -13.356841
    Time elapsed: 194 ms

```
当只加-ffast-math -march=native时优化程度约到1000ms
将数据结构AOS变SOA，计算的过程逻辑改成两层循环样式，此时加不加OpenMP和#pragma omp simd都已经能成功矢量化，
接下来将少部分的中间过程数据缩减成一个中间变量，但这个优化后基本没有多大变化。
```