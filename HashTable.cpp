#include "HashTable.h"

size_t pow(int base, size_t power) {
    size_t p = 1;
    for (int i=0; i<power; i++) {
        p *= base;
    }
    return p;
}

HashTable::HashTable(size_t size) noexcept {
    if (!size) size=100;
    _capacity = size;
    _filled = 0; _size = 0;
    table = tabletype(size);
}

HashTable::~HashTable() = default;


void HashTable::insert(const KeyType &key, const ValueType &value) {
    size_t hash, index;
    if (getLoadFactor() >= 0.75) {
        std::cout << "Increasing!" << std::endl;
        tabletype temp = table;
        _capacity = _capacity*2;
        _filled = 0; _size = 0;
        table = tabletype(_capacity);
        for (auto vit = temp.begin(); vit!=temp.end(); vit++) {
            for (auto lit = vit->begin(); lit != vit->end(); lit++) {
                insert(lit->first, lit->second);
            }
        }
    }
    
    hash = hash_function(key);
    index = index_from_hash(hash); // maybe some other way to convert hash to index
    
    for (auto lit = table[index].begin(); lit != table[index].end(); lit++) {
        if (lit->first == key) {
            lit->second = value; return;
        }
    }
    if (table[index].empty()) _filled++;
    _size++;
    table[index].push_back(std::make_pair(key, value));
}

bool HashTable::find(const KeyType &key, ValueType &value) const {
    size_t hash = hash_function(key);
    size_t index = index_from_hash(hash);
    for (auto lit = table[index].begin(); lit != table[index].end(); lit++) {
        if (lit->first == key) {
            value = lit->second;
            return true;
        }
    }
    return false;
}

bool HashTable::exist(const KeyType &key) const {
    size_t hash = hash_function(key);
    size_t index = index_from_hash(hash);
    for (auto lit = table[index].begin(); lit != table[index].end(); lit++) {
        if (lit->first == key) {
            return true;
        }
    }
    return false;
}

void HashTable::remove(const KeyType &key) {
    size_t hash = hash_function(key);
    size_t index = index_from_hash(hash);
    for (auto lit = table[index].begin(); lit != table[index].end(); lit++) {
        if (lit->first == key) {
            table[index].erase(lit);
            if (table[index].empty()) _filled--;
            _size--;
            return;
        }
    }
    //maybe warn that no such element
}

ValueType& HashTable::operator[](const KeyType &key) {
    size_t hash = hash_function(key);
    size_t index = index_from_hash(hash);
    for (auto lit = table[index].begin(); lit != table[index].end(); lit++) {
        if (lit->first == key) {
            return lit->second;
        }
    }
    //maybe warn that no such element
    ValueType value;
    insert(key, value);
    index = index_from_hash(hash);
    return table[index].back().second;
}

double HashTable::getLoadFactor() const {
    return (double)_filled/_capacity;
}

size_t HashTable::index_from_hash(size_t hash) const {
    return hash%_capacity;
}

size_t HashTable::hash_function(const KeyType &key) {
    size_t hash;
    hash = 0;
    
    #if (KeyType==string)
        for (int i=0; i<key.size(); i++) {
            hash += ((unsigned char)(key[i]));
            i%2 ?  hash >> 3 : hash << 8;
            hash -= i%3 * (unsigned char)(key[i]);
        }
    #else
        unsigned char* ptr = (unsigned char*)&key;
        size_t size = sizeof(key);
        size_t delta = 0;
        size_t temp;
        if (size>sizeof(size_t)){
            while (size > 0) {
                if (delta!=sizeof(size_t)) {
                    switch (delta%3)
                    {
                    case 0:
                        hash<<3;
                        break;
                    case 1:
                        hash>>1;
                        hash++;
                        break;
                    case 2:
                        temp = hash;
                        hash<<5;
                        temp /= delta;
                        hash *= (delta+2);
                        temp<<2; hash>>2;
                        hash = temp+hash-1;
                    default:
                        break;
                    }
                } else {
                    temp = hash;
                    temp << 3;
                    temp /= (delta-1);
                    temp >> 2;
                    hash >> 3;
                    hash *= (delta*delta);
                    hash--;
                    hash = temp+hash;
                }
                hash += (*ptr)*(hash ? hash : 1) ;
                size--; delta++;
                ptr++;
            }
        } else {
            std::memcpy(&hash, ptr, size);
        }
    #endif

    return hash;
}


size_t HashTable::maxChainLen() const {
    size_t max = 0;
    for (auto vit = table.begin(); vit!=table.end(); vit++) {
        if ((*vit).size() > max && !(*vit).empty()) max = (*vit).size();
    }
    return max;
}

void HashTable::print() const {
    for (int i =0; i<_capacity; i++) {
        if (table[i].empty()) continue;
        std::cout << "at index " << i << std::endl;
        for (auto lit = table[i].begin(); lit != table[i].end(); lit++) {
            std::cout << "\t{" << lit->first << "," << lit->second << "}" << std::endl;
        }
    }
}