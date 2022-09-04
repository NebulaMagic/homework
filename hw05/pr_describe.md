1.

把 `login`, `register` 等函数变成多线程安全的
能利用 `shared_mutex` 区分读和写
用 `lock_guard` 系列符合 RAII 思想

```
std::shared_mutex结合std::shared_lock和std::lock_guard来使用作为读写锁样式，使用{}划分代码块方便利用它们的RAII封装自动解锁，查找map是否有键值时共享读，要写入map时独占写。
```

2.

把 `login` 的登录计时器改成基于 chrono 的

```
使用using别名了一下好看点。
```

3.

让 ThreadPool::create 创建的线程保持后台运行不要退出
等待 tpool 中所有线程都结束后再退出

```
多任务队列线程池模型

用一个简陋的单任务队列线程池模型，从对应function类型的队列不断取出任务来执行，create函数循环向队列加入任务，取出和加入使用互斥锁保护，在main最后调用pool_stop()不断轮询判断队列是否空，空了就可以让线程池的循环关闭了，并且在析构之前全部线程join过。
```
