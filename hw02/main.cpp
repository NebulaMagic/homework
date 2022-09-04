/* 基于智能指针实现双向链表 */
#include <cstdio>
#include <memory>
#include <iostream>

template<typename T>
struct Node {
    
    std::unique_ptr<Node> next;
    // std::unique_ptr<Node> prev;
    Node * prev;
    
    T value;

    Node(T val) :value(val){}

    void insert(int val) {
        auto node = std::make_unique<Node>(val);

        node->prev = prev;
        prev = node.get();

        node->next = std::move(node->prev->next);  
        node->prev->next = std::move(node);
    }

    void erase() {
        if (next)
        {
            next->prev = prev;
        }
        if (prev)
        {
            prev->next = std::move(next);
        }
    }

    ~Node() {
        printf("~Node()\n"); 
    }
};

template<typename T>
struct node_traits
{
    using node_value = Node<T>;
    using node_pointer= Node<T>*;
    using node_reference = Node<T>&;
};

template<typename T>
struct list_iterator  : node_traits<T> {
    node_pointer node_ptr;

    list_iterator(node_pointer x):node_ptr(x){}

    list_iterator& operator++()
    {
        node_ptr = node_ptr->next.get();
        return *this;
    }

    list_iterator& operator--()
    {
        node_ptr = node_ptr->prev;
        return *this;
    }

    bool operator!=(list_iterator& rhs) const {return node_ptr != rhs.node_ptr;}
    bool operator==(list_iterator& rhs) const {return node_ptr == rhs.node_ptr;}
    node_reference operator*() {return *node_ptr;}
};


template<typename T>
struct List :node_traits<T>{
    using iterator = list_iterator<T>;

    std::unique_ptr<node_value> head;
    node_pointer tail;

    List() :head(nullptr),tail(nullptr){}

    List(List const &other) :head(nullptr),tail(nullptr){
        printf("List 被拷贝！\n");   

        auto _end = other.end(),  _begin = other.begin();
        for(; _begin!= _end; --_end)
        {
            push_front((*_end).value); 
        }
        if(_end==_begin)
        {
            push_front((*_end).value);    
        }
    }

    List &operator=(List const &) = delete;

    List(List &&) = default;
    List &operator=(List &&) = default;

    node_pointer front() const {
        return head.get();
    }

    T pop_front() {
        T ret = head->value;
        head = head->next;
        return ret;
    }

    void push_front(T value) {
        auto node = std::make_unique<node_value>(value);
        if(tail==nullptr)
        {   
            head = std::move(node);
            tail = head.get();   
            return;        
        }  
        head->prev = node.get();
        node->next = std::move(head);
        head = std::move(node);
    }

    node_pointer at(size_t index) const {
        auto curr = front();
        for (size_t i = 0; i < index; i++) {
            curr = curr->next.get();
        }
        return curr;
    }

    iterator  begin() const
    {
        return front();
    }

    iterator  end() const
    {
        return tail;
    }

    ~List()
    {
        while (head->next!=nullptr)
        {
            head = std::move(head->next);
        }
        
    }
};

template<typename T>
void print(List<T>& lst) { 
    printf("[");
    for (auto curr = lst.front(); curr; curr = curr->next.get()) {
        printf(" %d", curr->value);
    }
    printf(" ]\n");
}

int main() {
    List<int> a;

    a.push_front(7);
    a.push_front(5);
    a.push_front(8);
    a.push_front(2);
    a.push_front(9);
    a.push_front(4);
    a.push_front(1);

    print(a);   // [ 1 4 9 2 8 5 7 ]

    a.at(2)->erase();

    print(a);   // [ 1 4 2 8 5 7 ]

    List b = a;

    a.at(3)->erase();

    print(a);   // [ 1 4 2 5 7 ]
    print(b);   // [ 1 4 2 8 5 7 ]

    b.at(3)->insert(6); // [ 1 4 2 6 8 5 7 ]
    print(b);

    b = {};
    a = {};

    return 0;
}
