#include "lib.hpp"
#include <thread>
#include <iostream>

int main() {
    int tobaccoSmokedCount = 0;
    int paperSmokedCount = 0;
    int matchSmokedCount = 0;
    int iterationcount = 0;
    std::cin >> iterationcount;

    std::thread smoker1(smokerWithTobacco, std::ref(tobaccoSmokedCount));
    std::thread smoker2(smokerWithPaper, std::ref(paperSmokedCount));
    std::thread smoker3(smokerWithMatch, std::ref(matchSmokedCount));
    std::thread agentThread(agent, iterationcount);

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

