// lib.cpp

#include "lib.h"
#include <iostream>
#include <thread>
#include <random>
#include <mutex>
#include <condition_variable>

std::mutex tableMutex;                 // Мьютекс для синхронизации доступа к столу
std::condition_variable tableReady;    // Условная переменная для уведомления о готовности компонентов на столе
bool componentsOnTable = false;        // Флаг наличия компонентов на столе
bool finished = false;                 // Флаг завершения работы
int rounds = 0;                       // Счётчик раздач

// Тип компонента: табак, бумага или спички
enum Component { TOBACCO, PAPER, MATCH };
Component table[2];                    // Массив для хранения компонентов на столе

// Посредник кладет два случайных компонента на стол
void agent(int maxRounds) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 2);

    while (rounds < maxRounds) {
        std::unique_lock<std::mutex> lock(tableMutex);

        // Ожидание, пока курильщик не заберет компоненты
        while (componentsOnTable) {
            tableReady.wait(lock);
        }

        // Выбор случайных компонентов
        int firstComponent = dist(gen);
        int secondComponent = (firstComponent + 1 + dist(gen) % 2) % 3;

        table[0] = static_cast<Component>(firstComponent);
        table[1] = static_cast<Component>(secondComponent);
        componentsOnTable = true;
        rounds++;

        std::cout << "The agent put the components: "
            << (firstComponent == 0 ? "tobacco" : (firstComponent == 1 ? "paper" : "matches"))
            << " and "
            << (secondComponent == 0 ? "tobacco" : (secondComponent == 1 ? "paper" : "matches"))
            << std::endl;

        tableReady.notify_all();
    }
    // Устанавливаем флаг завершения и оповещаем курильщиков
    finished = true;
    tableReady.notify_all();

}

// Курильщик с табаком
void smokerWithTobacco(int& tobaccoSmokedCount) {
    while (true) {
        std::unique_lock<std::mutex> lock(tableMutex);
        while (!componentsOnTable || (table[0] == TOBACCO || table[1] == TOBACCO)) {
            tableReady.wait(lock);
            if (finished) return;  // Завершение, если работа окончена
        }

        // Забирает компоненты
        std::cout << "A smoker with tobacco is smoking." << std::endl;
        tobaccoSmokedCount++;
        componentsOnTable = false;
        tableReady.notify_all();
    }
}

// Курильщик с бумагой
void smokerWithPaper(int& paperSmokedCount) {
    while (true) {
        std::unique_lock<std::mutex> lock(tableMutex);
        while (!componentsOnTable || (table[0] == PAPER || table[1] == PAPER)) {
            tableReady.wait(lock);
            if (finished) return;  // Завершение, если работа окончена
        }

        // Забирает компоненты
        std::cout << "A smoker with paper is smoking." << std::endl;
        paperSmokedCount++;
        componentsOnTable = false;
        tableReady.notify_all();
    }
}

// Курильщик со спичками
void smokerWithMatch(int& matchSmokedCount) {
    while (true) {
        std::unique_lock<std::mutex> lock(tableMutex);
        while (!componentsOnTable || (table[0] == MATCH || table[1] == MATCH)) {
            tableReady.wait(lock);
            if (finished) return;  // Завершение, если работа окончена
        }

        // Забирает компоненты
        std::cout << "A smoker with matches is smoking." << std::endl;
        matchSmokedCount++;
        componentsOnTable = false;
        tableReady.notify_all();
    }
}
