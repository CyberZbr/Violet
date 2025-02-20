/// һ��������⺯������ʱ��ĺ����ӿ�  ///

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
            // ������void�����
            std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);
            auto end = Clock::now();
            auto duration = end - start;
            std::cout << "Duration: "
                      << std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count()
                      << " ns\n";
        } else {
            // �����з���ֵ�����
            decltype(auto) result = std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);
            auto end = Clock::now();
            auto duration = end - start;
            std::cout << "Duration: "
                      << std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count()
                      << " ns\n";
            return result; // ��ȷ���ؽ������������/ֵ���壩
        }
    } catch (...) {
        // �쳣������¼ʱ��������׳�
        auto end = Clock::now();
        auto duration = end - start;
        std::cout << "Duration (exception): "
                  << std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count()
                  << " ns\n";
        throw;
    }
}

//ģ��һ����ʱ�����е��޷���ֵ����
void long_running_void(int delay) {
    std::this_thread::sleep_for(std::chrono::seconds(delay));
}

//ģ��һ������a+b���з���ֵ����
int long_running_int(int a, int b) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return a + b;
}

int main() {
    // ����void����
    measure_time(long_running_void, 1);

    // ���Է�void����
    int result = measure_time(long_running_int, 2, 3);
    std::cout << "Result: " << result << std::endl;

    return 0;
}
