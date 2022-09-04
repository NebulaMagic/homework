AMD 3800X 3.90 GHz 8核16线程  RAM 16GB   Windows 10 MSVC2019

使用的tbb版本从2020U1标签克隆，使用build文件夹自带vs工程构建出

原程序：
```
fill: 1.60957s
fill: 1.60354s
saxpy: 0.644247s
sqrtdot: 0.683698s
5165.4
minvalue: 0.288241s
-1.11803
magicfilter: 3.78971s
55924034
scanner: 0.479334s
5.28566e+07
```

使用TBB后：
```
fill: 0.162371s         10x
fill: 0.160031s         
saxpy: 0.082478s        1.7x
sqrtdot: 0.0559893s     10x
5792.63
minvalue: 0.0338596s    10x
-1.11803
magicfilter: 0.450959s  9x
67108864
scanner: 0.131877s      4x
5.28721e+07
```




```
fill和saxpy过程是赋值，用parallel_for。

sqrtdot行为是缩并，使用parallel_reduce。

minvalue把分成多个区间，每个区间内串行求出最小值给到线程安全vector，最后再串行找最小，似乎可以用递归写法多分几次区间，没有写出来。

magicfilter对应课堂并行筛选的部分，这里一开始试的使用单个锁配合多个局部数组的方式一直都是负优化很奇怪，然后再试老师的原子加操作记录每个局部数组在最终数组的位置尺寸方法才有优化。

scanner行为是扫描，使用parallel_scan。

```