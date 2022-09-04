1.

// fill_sin 改成“网格跨步循环”以后，这里三重尖括号里的参数如何调整？10 分
fill_sin<<<n / 1024, 1024>>>(arr.data(), n);

```
按照老师的意思可以随意指定板块和线程数，这里也填个比较小的值。
```

2.

// 这里的“边角料法”对于不是 1024 整数倍的 n 会出错，为什么？请修复：10 分
filter_positive<<<n / 1024, 1024>>>(counter.data(), res.data(), arr.data(), n);

```
因为默认除法是向下取整，这样忽略掉的余数部分就索引不到了，这里也改为网格跨步循环。
函数内改为直接使用原子加函数，编译器自动优化。
```



3.

// 这里 CPU 访问数据前漏了一步什么操作？请补上：10 分

```
使用universal_vectoruni或者CudaAllocator(cudaMallocManaged)让CPU和GPU使用统一的地址空间需要主动使用cudaDeviceSynchronize同步数据，使用cudaMemcpy方式时可以省略主动同步。
```



------------

```
其他记录：MSVC2019 CUDA11.6.1 

1.使用std::vector<int, CudaAllocator<int>>会报错，搜索了很多没有找到什么意思问题

    no suitable user-defined conversion fro to "CudaAllocator<std::_Container_proxy>

2.使用universal_vector的data()类型与int* 不直接匹配
```