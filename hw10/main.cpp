#include <cstdio>
#include <vector>
#include <limits>
#include <cstdlib>
#include <cstring>
#include "ticktock.h"
#include <unordered_map>
#include <memory>

template <class T>
struct PlaceData
{
    static constexpr bool isPlace = true;

    T m_value;

    T ret_value()
    {
        return m_value;
    }

    T& ret_reference()
    {
        return m_value;
    }
};


template <int Bshift, class Node>
struct DenseBlock
{
    static constexpr bool isPlace = false;
    static constexpr bool bitShift = Bshift;
    static constexpr int B = 1 << Bshift;
    static constexpr int Bmask = B - 1;

    Node m_data[B][B];

    Node  ret_value(int x, int y)
    {
        return m_data[x & Bmask][y & Bmask];
    }

    Node& ret_reference(int x, int y)
    {
        return m_data[x & Bmask][y & Bmask];
    }
};

template <int Bshift, class Node>
struct PointerBlock
{
    static constexpr bool isPlace = false;
    static constexpr bool bitShift = Bshift;
    static constexpr int B = 1 << Bshift;
    static constexpr int Bmask = B - 1;

    static Node empty_node;

    std::unique_ptr<Node> m_data[B][B];

    Node  ret_value(int x, int y)
    {
        auto& block_ptr = m_data[x & Bmask][y & Bmask];
        if(!block_ptr)
        {
            return empty_node;
        }
        return *block_ptr;
    }

    Node& ret_reference(int x, int y)
    {
        auto& block_ptr = m_data[x & Bmask][y & Bmask];
        if(!block_ptr)
        {
            block_ptr=std::make_unique<Node>();
        }
        return *block_ptr;
    }
};


template <class Node>
struct HashBlock
{
    static constexpr bool isPlace = false;
    static constexpr bool bitShift = 0;

    static Node empty_node;

    struct MyHash {
        std::size_t operator()(std::tuple<int, int> const &key) const {
            auto const &[x, y] = key;
            return (x * 114525) ^ (y * 314125);
        }
    };

    std::unordered_map<std::tuple<int, int>, Node, MyHash> m_data;

    Node ret_value(int x,int y)
    {
        auto it = m_data.find(std::make_tuple(x,y));
        if(it == m_data.end())
        {
            return empty_node;
        }
        return it->second;
    }
    
    Node& ret_reference(int x,int y)
    {
        auto it = m_data.find(std::make_tuple(x,y));
        if(it == m_data.end())
        {
            auto ret = m_data.emplace(std::tuple(x,y),Node{});
            return (ret.first)->second;
        }
        return it->second;         
    }

};

template <class T,class Layout>
struct RootGrid
{
    Layout m_root;

    template <class Node>
    static T _ret_value(Node& node, int x, int y)
    {
        if constexpr (node.isPlace)
        {
            return node.ret_value();
        }
        else
        {
            auto& child = node.ret_reference(x >> node.bitShift, y >> node.bitShift);
            return _ret_value(child,x,y);
        }       
    }

    T ret_value(int x, int y)
    {
        return _ret_value(m_root x, y);
    }


    template <class Node>
    static T& _ret_reference(Node& node, int x, int y)
    {
        if constexpr (node.isPlace)
        {
            return node.ret_reference();
        }
        else
        {
            auto& child = node.ret_reference(x >> node.bitShift, y >> node.bitShift);
            return _ret_reference(child,x,y);
        }       
    }

    T& ret_reference(int x, int y)
    {
        return _ret_reference(m_root, x, y);
    }

};

template <int Width,class T,class Layout>
struct Grid
{
    RootGrid<T,Layout> m_grid;

    T& Grid::operator[](int i)
    {
        return m_grid.ret_reference(i / N, i % N);
    }
};

constexpr int N = 1<<12;
Grid<N,char,HashBlock<DenseBlock<3,PlaceData<char>>>> cells; //????????????
Grid<N,char,HashBlock<DenseBlock<3,PlaceData<char>>>> outcells;
// Grid<N,char,PointerBlock<6,DenseBlock<2,PlaceData<char>>>> cells;  //????????????
// Grid<N,char,PointerBlock<6,DenseBlock<2,PlaceData<char>>>> outcells;

// ??????: ?????????????????????????????????????????????
// std::vector<bool> cells(N * N);
// std::vector<bool> outcells(N * N);

const char gundata[] = R"(125b3o$125bobo$125b3o6bo$125b3o5b3o$125b3o$125b3o$125bobo5b3o$125b3o$
133bobo$133bobo2$133b3o$111b2o6b2o$110bo2bo4bo2bo$110bo2bo4bo2bo11b3o$
110bo2bo4bo2bo12bo$111b2o6b2o2$137bo2b2o4b2o2bo$136bo3b3o2b3o3bo$137bo
2b2o4b2o2bo7$139bo$139b3o4bobo2bobo$138bob4obo2bo2bo2bob2o$120bobo23bo
bo2bobo$120b2o$121bo$139b3o$139b3o$44b3o93bo$45bo94bo$45bo94bo$44b3o
92bobo2$44b3o$44b3o92bobo$140bo$44b3o93bo$45bo94bo$45bo93b3o$44b3o92b
3o2$51b2o6b2o$49bo4bo2bo4bo$49bo4bo2bo4bo$49bo4bo2bo4bo4bo$51b2o6b2o5b
3o8bo$65bobobo6b3o$65bobobo5bobobo$66b3o6bobobo$67bo8b3o$77bo2$67bo$
66b3o8bo$17bo47bobobo6b3o$16b3o46bobobo5bobobo$15bobobo46b3o6bobobo$
15bobobo47bo8b3o$16b3o12bo45bo$17bo11b2o$30b2o2$17bo79b3o$16b3o80bo$
15bobobo59bo18bo$15bobobo59bobo$16b3o49bo10b2o$17bo49b2o$67bobo$78b3o$
2bo2bo4bo2bo64bo$3o2b6o2b3o63bo$2bo2bo4bo2bo2$81b2o$82b2o$81bo2$48bo$
46b2o$47b2o$58bobo2bobo$54b2obo2bo2bo2bob2o$58bobo2bobo2$22b3o52b3o2$
21bo3bo50bo3bo$21bo3bo50bo3bo2$22b3o52b3o$88b3o$88b3o$22b3o52b3o9bo$
89bo$21bo3bo50bo3bo8bo$21bo3bo17bobo30bo3bo7bobo$44b2o$22b3o19bo32b3o$
88bobo$89bo$8bo2bob2obo2bo69bo$8b4ob2ob4o69bo$8bo2bob2obo2bo68b3o$37b
2o6b2o41b3o$35bo4bo2bo4bo$35bo4bo2bo4bo$35bo4bo2bo4bo$37b2o6b2o2$51b3o
66b2o$52bo66b2o$52bo68bo$51b3o2$51b3o$38bobo10b3o$39b2o$39bo11b3o$52bo
$52bo$51b3o26$159bo$159b3o4bobo2bobo$158bob4obo2bo2bo2bob2o$166bobo2bo
bo3$159b3o$159b3o$160bo$160bo$81bo78bo$79bobo77bobo$80b2o2$159bobo$
160bo$160bo$160bo$159b3o$159b3o23$117b3o$119bo$118bo$100b2o6b2o$99bo2b
o4bo2bo$99bo2bo4bo2bo$99bo2bo4bo2bo$100b2o6b2o$114b3o2$113bo3bo$113bo
3bo2$114b3o3$114b3o2$113bo3bo$113bo3bo2$114b3o!)";

void init(int bx, int by) {
    int acc = 0;
    int x = bx;
    int y = by;
    for (int i = 0; i < sizeof(gundata); i++) {
        char c = gundata[i];
        if (!c || strchr(" \n\t!", c)) continue;
        if ('0' <= c && c <= '9') {
            acc *= 10;
            acc += c - '0';
            continue;
        }
        if (!acc) acc = 1;
        if (c == 'b') {
            for (int o = 0; o < acc; o++) {
                cells[x * N + y++] = 0;
            }
        }
        if (c == 'o') {
            for (int o = 0; o < acc; o++) {
                cells[x * N + y++] = 1;
            }
        }
        if (c == '$') {
            y = by;
            x += acc;
        }
        acc = 0;
    }
}

void step() {
// #pragma omp parallel for collapse(2)
    for (int y = 1; y < N-1; y++) {
        for (int x = 1; x < N-1; x++) {
            int neigh = 0;
            neigh += cells[x * N + (y + 1)];
            neigh += cells[x * N + (y - 1)];
            neigh += cells[(x + 1) * N + (y + 1)];
            neigh += cells[(x + 1) * N + y];
            neigh += cells[(x + 1) * N + (y - 1)];
            neigh += cells[(x - 1) * N + (y + 1)];
            neigh += cells[(x - 1) * N + y];
            neigh += cells[(x - 1) * N + (y - 1)];
            if (cells[x * N + y]) {
                if (neigh == 2 || neigh == 3) {
                    outcells[x * N + y] = 1;
                } else {
                    outcells[x * N + y] = 0;
                }
            } else {
                if (neigh == 3) {
                    outcells[x * N + y] = 1;
                } else {
                    outcells[x * N + y] = 0;
                }
            }
        }
    }
    std::swap(cells, outcells);
}

void showinfo() {
    int rightbound = std::numeric_limits<int>::min();
    int leftbound = std::numeric_limits<int>::max();
    int count = 0;
// #pragma omp parallel for collapse(2) reduction(max:rightbound) reduction(min:leftbound) reduction(+:count)
    for (int x = 0; x < N; x++) {
        for (int y = 0; y < N; y++) {
            if (cells[x * N + y]) {
                rightbound = std::max(rightbound, y);
                leftbound = std::min(leftbound, y);
                count++;
            }
        }
    }
    // ???????????????left=1048, right=3523, count=2910
    printf("left=%d, right=%d, count=%d\n", leftbound, rightbound, count);
}

int main() {
    TICK(main);

    init(N / 2, N / 2);
    init(N / 2 - 500, N / 2 - 500);
    init(N / 2 + 500, N / 2 + 500);
    init(N / 2 - 1000, N / 2 - 1000);
    init(N / 2 + 1000, N / 2 + 1000);
    for (int times = 0; times < 800; times++) {
        printf("step %d\n", times);
        if (times % 100 == 0)
            showinfo();
        step();
    }
    showinfo();

    TOCK(main);
    return 0;
}
