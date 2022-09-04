// 小彭老师作业05：假装是多线程 HTTP 服务器 - 富连网大厂面试官觉得很赞
#include <functional>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <thread>
#include <map>
#include <chrono>
#include <mutex>
#include <shared_mutex>
#include <vector>
#include <queue>

// using namespace std::literals;
// using double_seconds = std::chrono::duration<double>;
// using m_time_duration = decltype(std::chrono::duration_cast<double_seconds>(std::declval<m_time_point>() - std::declval<m_time_point>()));
using m_time_point = decltype(std::chrono::steady_clock::now());
using m_time_duration = std::chrono::duration<double>;


struct User {
    std::string password;
    std::string school;
    std::string phone;
};

std::map<std::string, User> users;
std::map<std::string, m_time_point> has_login;
std::shared_mutex register_mutex;
std::shared_mutex login_mutex;



std::string do_register(std::string username, std::string password, std::string school, std::string phone) {
    std::cout<<"注册： "<<username<<std::endl;
    User user = {password, school, phone};
    {
        std::shared_lock<std::shared_mutex> readlock(register_mutex);
        if (users.find(username) != users.end())
            return "用户名已被注册";
    }
  
    std::lock_guard<std::shared_mutex> writelock(register_mutex);
    if (users.emplace(username, user).second)
        return "注册成功";
    else
        return "注册失败";
}

std::string do_login(std::string username, std::string password) {
    std::cout<<"登录： "<<username<<std::endl;
    {
        std::shared_lock<std::shared_mutex> readlock(login_mutex);
        if (has_login.find(username) != has_login.end())
        {
            auto now = std::chrono::steady_clock::now();
             m_time_duration login_duration = now - has_login.at(username);
            return std::to_string(login_duration.count()) + "秒内登录过";
        }         
    }

    {
        std::lock_guard<std::shared_mutex> writelock(login_mutex);
        {
            std:: shared_lock<std::shared_mutex> readlock(register_mutex);
            if (users.find(username) == users.end())
                return "用户名错误";
            
            if (users.at(username).password != password)
                return "密码错误";   
        }
        has_login[username] = std::chrono::steady_clock::now();       
    } 
    return "登录成功";     
}

std::string do_queryuser(std::string username) {
    std::cout<<"查询： "<<username<<std::endl;
    User user;
    {        
        std::shared_lock<std::shared_mutex> readlock(register_mutex);
        try{
            user = users.at(username);
        }catch(const std::out_of_range &e){
            return username + "未注册";
        }   
    }
    std::stringstream ss;
    ss << "用户名: " << username << std::endl;
    ss << "学校:" << user.school << std::endl;
    ss << "电话: " << user.phone << std::endl;
    return ss.str();
}


struct ThreadPool {
    void create(std::function<void()> start) {
        std::lock_guard<std::mutex> lk(queue_lock);
        tasks_queue.push(std::move(start));
    }

    ThreadPool():pool_run_flag(false)
    {         
        // auto const thread_count = std::thread::hardware_concurrency() - 1;
        auto const thread_count = 2;
        threads_run.reserve(thread_count);
        for(size_t i=0;i<thread_count;i++)
        {
            threads_run.push_back(std::thread(&ThreadPool::worker,this));
        }
        pool_run_flag = true;
    }

    void worker()
    {
        std::function<void()> task;
        while(pool_run_flag)
        {           
            std::unique_lock<std::mutex> lk(queue_lock,std::defer_lock);
            if(lk.try_lock() && tasks_queue.size()!=0)
            {
                task = tasks_queue.front();
                tasks_queue.pop();
                lk.unlock();
                task();
            }
            else
            {
                std::this_thread::yield();
            }         
        }
    }

    void pool_stop()
    {
        while (true)
        {
            if(tasks_queue.size()==0)
            {
                pool_run_flag = false;
                break;
            }
        }
        
        for(auto& t : threads_run)
        {
            if(t.joinable())
            {
                t.join();
            }
        }      
    }

    std::vector<std::thread> threads_run;
    std::queue<std::function<void()>> tasks_queue;
    std::mutex queue_lock;
    bool pool_run_flag; 
};




namespace test {  // 测试用例？出水用力！
std::string username[] = {"张心欣", "王鑫磊", "彭于斌", "胡原名"};
std::string password[] = {"hellojob", "anti-job42", "cihou233", "reCihou_!"};
std::string school[] = {"九百八十五大鞋", "浙江大鞋", "剑桥大鞋", "麻绳理工鞋院"};
std::string phone[] = {"110", "119", "120", "12315"};
}

int main() {
    ThreadPool tpool;

    for (int i = 0; i < 262144; i++) {
        tpool.create([&] {
            std::cout << do_register(test::username[rand() % 4], test::password[rand() % 4], test::school[rand() % 4], test::phone[rand() % 4]) << std::endl;
        });
        tpool.create([&] {
            std::cout << do_login(test::username[rand() % 4], test::password[rand() % 4]) << std::endl;
        });
        tpool.create([&] {
            std::cout << do_queryuser(test::username[rand() % 4]) << std::endl;
        });
    }

    tpool.pool_stop(); 
    return 0;
}
