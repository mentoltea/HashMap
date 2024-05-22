#pragma once
#include <vector>
#include <list>
#include <string>
#include <utility>
#include <stdint.h>
#include <iostream>
#include <exception>
#include <cstring>

using KeyType = std::string;
using ValueType = double;

typedef std::vector<std::list<std::pair<KeyType, ValueType> > > tabletype;

class HashTable 
{
public:
    //! Конструктор
    explicit HashTable(size_t size = 100) noexcept;
    //! Деструктор
    ~HashTable();

    //! Вставка элемента в хеш-таблицу
    //! Если при вставке обнаруживается, что LoadFactor таблицы превышает 0.75, то таблица расширяется в 2 раза.
    void insert(const KeyType &key, const ValueType &value);
    //! Поиск элемента в хеш таблице
    bool find(const KeyType &key, const ValueType &value) const;
    bool exist(const KeyType &key) const;
    //! Удаление элемента из хеш-таблицы
    void remove(const KeyType &key);
    //! Получение элемента по ключу
    ValueType& operator[](const KeyType &key);

    //! Загрузка хеш-таблицы. Считается как отношение количества занятых ячеек к вместимости хеш-таблицы.
    double getLoadFactor() const;

    size_t maxChainLen() const;

    void print() const;
private:
    int32_t _capacity;  //!< Вместимость хеш-таблицы
    int32_t _filled;    //!< Текущее количество занятых ячеек хеш-таблицы

    //! Структура, на которой основана таблица с методом цепочек для решения коллизий
    tabletype table;
    //! Хеш-функция

    static size_t hash_function(const KeyType &key); 
    size_t index_from_hash(size_t hash) const;
};