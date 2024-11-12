
#ifndef LIB_HPP
#define LIB_HPP

#include <mutex>
#include <condition_variable>

// Функции для курильщиков
void smokerWithTobacco(int& tobaccoSmokedCount);
void smokerWithPaper(int& paperSmokedCount);
void smokerWithMatch(int& matchSmokedCount);

// Функция для посредника
void agent(int maxRounds);

#endif // LIB_HPP
