# 未完成


# lscpu

    Thread(s) per core:              1
    Core(s) per socket:              2
    L1d cache:                       64 KiB
    L1i cache:                       64 KiB
    L2 cache:                        8 MiB
    L3 cache:                        35.8 MiB

# 改进前

```
matrix_randomize: 0.00703398s
matrix_randomize: 0.0102356s

```

# 改进后

```
matrix_randomize: 0.00388176s
matrix_randomize: 0.00386299s

```

# 加速比

```
matrix_randomize: 2~3x
```
> 如果记录了多种优化方法，可以做表格比较

# 优化方法

下面这些函数你是如何优化的？是什么思路？用了老师上课的哪个知识点？

> matrix_randomize

```
ndarray是以YX序，将原XY序循环改为YX序循环，空间连续性对缓存友好。
```

> matrix_transpose

请回答。

> matrix_multiply

请回答。

> matrix_RtAR

请回答。

# 我的创新点

如果有，请说明。
