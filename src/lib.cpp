// lib.cpp

#include "lib.hpp"
#include <iostream>
#include <thread>
#include <random>
#include <mutex>
#include <condition_variable>

std::mutex tableMutex;                 // Мьютекс для синхронизации доступа к столу
std::condition_variable tableReady;    // Условная переменная для уведомления о готовности компонентов на столе
bool componentsOnTable = false;        // Флаг наличия компонентов на столе
bool finished = false;                 // Флаг завершения работы
int rounds = 0;                        // Счётчик раздач

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

        std::cout << "Посредник положил компоненты: " 
                  << (firstComponent == 0 ? "табак" : (firstComponent == 1 ? "бумага" : "спички")) 
                  << " и " 
                  << (secondComponent == 0 ? "табак" : (secondComponent == 1 ? "бумага" : "спички")) 
                  << std::endl;

        tableReady.notify_all();
    }

    // Устанавливаем флаг завершения и оповещаем курильщиков
    finished = true;
    tableReady.notify_all();
}

// Универсальная функция для курильщика
void smoker(Component ownedComponent, int& smokedCount) {
    while (true) {
        std::unique_lock<std::mutex> lock(tableMutex);

        // Ожидание компонентов на столе или завершения работы
        while (!componentsOnTable || 
               (table[0] == ownedComponent || table[1] == ownedComponent)) {
            tableReady.wait(lock);
            if (finished) return;  // Завершение, если работа окончена
        }

        // Забирает компоненты и "курит"
        std::cout << "Курильщик с компонентом " 
                  << (ownedComponent == TOBACCO ? "табак" : (ownedComponent == PAPER ? "бумага" : "спички")) 
                  << " скрутил сигарету и курит." << std::endl;
        smokedCount++;
        componentsOnTable = false;

        tableReady.notify_all();
    }
}
