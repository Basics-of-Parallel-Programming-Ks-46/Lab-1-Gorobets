// main.cpp

#include "lib.hpp"
#include <thread>
#include <iostream>

int main() {
    int tobaccoSmokedCount = 0;
    int paperSmokedCount = 0;
    int matchSmokedCount = 0;

    // Запуск потоков для курильщиков
    std::thread smoker1(smoker, TOBACCO, std::ref(tobaccoSmokedCount));
    std::thread smoker2(smoker, PAPER, std::ref(paperSmokedCount));
    std::thread smoker3(smoker, MATCH, std::ref(matchSmokedCount));

    // Запуск посредника
    std::thread agentThread(agent, 10);

    smoker1.join();
    smoker2.join();
    smoker3.join();
    agentThread.join();

    std::cout << "result:\n";
    std::cout << "A smoker with tobacco smoked: " << tobaccoSmokedCount << " cigarettes\n";
    std::cout << "A smoker with paper smoked: " << paperSmokedCount << " cigarettes\n";
    std::cout << "A smoker with match smoked: " << matchSmokedCount << " cigarettes\n";

    return 0;
}

