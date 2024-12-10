
// lib.hpp

#ifndef LIB_HPP
#define LIB_HPP

#include <mutex>
#include <condition_variable>

// Тип компонента: табак, бумага или спички
enum Component { TOBACCO, PAPER, MATCH };

// Универсальная функция для курильщика
void smoker(Component ownedComponent, int& smokedCount);

// Функция для посредника
void agent(int maxRounds);

#endif // LIB_HPP
