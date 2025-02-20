/// 一个监控任意函数运行时间的函数接口  ///

#include <chrono>
#include <iostream>
#include <type_traits>
#include <functional>

template<typename Func, typename... Args>
decltype(auto) measure_time(Func&& func, Args&&... args) {
    using Clock = std::chrono::high_resolution_clock;
    auto start = Clock::now();

    try {
        if constexpr (std::is_void_v<std::invoke_result_t<Func, Args...>>) {
            // 处理返回void的情况
            std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);
            auto end = Clock::now();
            auto duration = end - start;
            std::cout << "Duration: "
                      << std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count()
                      << " ns\n";
        } else {
            // 处理有返回值的情况
            decltype(auto) result = std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);
            auto end = Clock::now();
            auto duration = end - start;
            std::cout << "Duration: "
                      << std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count()
                      << " ns\n";
            return result; // 正确返回结果（保持引用/值语义）
        }
    } catch (...) {
        // 异常处理，记录时间后重新抛出
        auto end = Clock::now();
        auto duration = end - start;
        std::cout << "Duration (exception): "
                  << std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count()
                  << " ns\n";
        throw;
    }
}

//模拟一个长时间运行的无返回值函数
void long_running_void(int delay) {
    std::this_thread::sleep_for(std::chrono::seconds(delay));
}

//模拟一个返回a+b的有返回值函数
int long_running_int(int a, int b) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return a + b;
}

int main() {
    // 测试void函数
    measure_time(long_running_void, 1);

    // 测试非void函数
    int result = measure_time(long_running_int, 2, 3);
    std::cout << "Result: " << result << std::endl;

    return 0;
}
