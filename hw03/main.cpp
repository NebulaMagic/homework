#include <iostream>
#include <vector>
#include <variant>

template<typename T>
std::ostream &operator<<(std::ostream &os, std::vector<T> const &a) {
    os << "{";
    for (size_t i = 0; i < a.size(); i++) {
        os << a[i];
        if (i != a.size() - 1)
            os << ", ";
    }
    os << "}";
    return os;
}


template <class T1, class T2,
typename Tr = decltype(std::declval<T1>()+std::declval<T2>())>
std::vector<Tr> operator+(std::vector<T1> const &a, std::vector<T2> const &b) {
    std::vector<Tr> res(std::min(a.size(),b.size()));
    for(size_t i =0;i<res.size();i++)
    {
        res[i] = a[i] + b[i];
    }
    return res;
}


template<class T1,class T2,class Tx,
typename = std::enable_if_t<std::is_same_v<T1,Tx> || std::is_same_v<T2,Tx>>>
std::variant<T1,T2> operator+(std::variant<T1,T2> const& a,Tx const& b)
{
    std::variant<T1,T2> res;
    std::visit([&](auto const& t1){
                res = t1 + b;
            },a);
    return res;
}

template<class T1,class T2,class Tx,
typename = std::enable_if_t<std::is_same_v<T1,Tx> || std::is_same_v<T2,Tx>>>
std::variant<T1,T2> operator+(Tx const& a,std::variant<T1,T2> const& b)
{
    return b+a;
}


template <class T1, class T2,
typename Tr = decltype(std::declval<T1>()+std::declval<T2>()),
typename = std::enable_if_t<std::is_same_v<T1,Tr> || std::is_same_v<T2,Tr>>>
std::variant<T1, T2> operator+(std::variant<T1, T2> const &a, std::variant<T1, T2> const &b) {
    std::variant<T1,T2> res;
    std::visit([&](auto const& t1,auto const& t2){
                res = t1 + t2;
            },a,b);
    return res;
}


template <class T1,class... T2>
std::ostream &operator<<(std::ostream &os, std::variant<T1,T2...> const &a) {
    std::visit([&](auto const& t){
               os<<t<<std::endl; 
            },a);
    return os;
}

int main() {
    std::vector<int> a = {1, 4, 2, 8, 5, 7};
    std::cout << a << std::endl;
    std::vector<double> b = {3.14, 2.718, 0.618};
    std::cout << b << std::endl;
    auto c = a + b;

    // 应该输出 1
    std::cout << std::is_same_v<decltype(c), std::vector<double>> << std::endl;

    // 应该输出 {4.14, 6.718, 2.618}
    std::cout << c << std::endl;

    std::variant<std::vector<int>, std::vector<double>> d = c;
    std::variant<std::vector<int>, std::vector<double>> e = a;
    d = d + c + e;

    // 应该输出 {9.28, 17.436, 7.236}
    std::cout << d << std::endl;

    return 0;
}
