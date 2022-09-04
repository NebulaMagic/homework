1. 

// 请修复这个函数的定义：10 分
std::ostream &operator<<(std::ostream &os, std::vector<T> const &a) {
……

```
缺少template<typename T>
```

2.

// 请修复这个函数的定义：10 分
template <class T1, class T2>
std::vector<T0> operator+(std::vector<T1> const &a, std::vector<T2> const &b) {
    // 请实现列表的逐元素加法！10 分

```
使用decltype和devlval配合推导出两个模板参数相加后的类型，如果T1+T2是个新的Tr类型，返回的也是新Tr类型的vector。
```

3.

template <class T1, class T2>
std::variant<T1, T2> operator+(std::variant<T1, T2> const &a, std::variant<T1, T2> const &b) {
    // 请实现自动匹配容器中具体类型的加法！10 分
}

```
使用std::visit配合。根据题目中main函数判断应该是需要再另外写variant和vector相加的operator+，
这里用SFINAE来限定和variant相加的另一个类型必须是variant内的其中之一类型。
```


4.

template <class T1, class T2>
std::ostream &operator<<(std::ostream &os, std::variant<T1, T2> const &a) {
    // 请实现自动匹配容器中具体类型的打印！10 分
}

```
写为了variant的变长模板参数形式。
```
