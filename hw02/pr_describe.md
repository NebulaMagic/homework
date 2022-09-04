
1.

// 这两个指针会造成什么问题？请修复
std::shared_ptr<Node> next;
std::shared_ptr<Node> prev;

```
list中的head节点和第二个节点组成了循环引用，head不会调用析构函数，从而整个list都无法析构。
题目提示改为使用unique_ptr,这里我没有想到怎么用两个unique_ptr写出双向链表，这里改为next方向使用unique_ptr，prev方向使用原始指针，
另外需要修改list析构函数，因为让原本unique_ptr从头自动析构时会递归调用next的成员next析构，当list比较大时容易栈溢出，这里改手动循环移动head到末尾,
添加了迭代器形式，不过暂不太标准和细节不太好。

```

2.

// 这个构造函数有什么可以改进的？
Node(int val) {
    value = val;
}

Node(int val) ：value(val){
}

```
改为使用初始列表对性能好，直接在成员位置调用构造。
```

3.

~Node() {
    printf("~Node()\n");   // 应输出多少次？为什么少了？
}

```
因为 1 的意思，在原题目list退出作用域时没有一个节点能自动析构，没有输出，修改后list有多少个节点就输出多少次。
```

4.

List &operator=(List const &) = delete;  // 为什么删除拷贝赋值函数也不出错？

```
在使用=时变成了调用构造函数的形式，如果在构造函数加了explicit就会报错。
```

5.

void print(List lst) {  // 有什么值得改进的？
……

```
改为void print(List const& lst)  只读和避免额外拷贝构造
```
