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
    _filled = 0;
    table = tabletype(size);
}

HashTable::~HashTable() = default;


void HashTable::insert(const KeyType &key, const ValueType &value) {
    if (getLoadFactor() >= 0.75) {
        std::cout << "Increasing!" << std::endl;
        tabletype temp = table;
        _capacity *= 2;
        table = tabletype(_capacity);
        for (auto vit = temp.begin(); vit!=temp.end(); vit++) {
            for (auto lit = (*vit).begin(); lit != (*vit).end(); lit++) {
                insert(lit->first, lit->second);
            }
        }
    }
    size_t hash = hash_function(key);
    size_t index = index_from_hash(hash); // maybe some other way to convert hash to index
    
    for (auto lit = table[index].begin(); lit != table[index].end(); lit++) {
        if (lit->first == key) {
            lit->second = value; return;
        }
    }
    if (table[index].empty()) _filled++;
    table[index].push_back(std::make_pair(key, value));
}

bool HashTable::find(const KeyType &key, ValueType &value) const {
    size_t hash = hash_function(key);
    size_t index = index_from_hash(hash);
    for (auto lit = table[index].begin(); lit != table[index].end(); lit++) {
        if (lit->first == key && lit->second == value) {
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
            _filled--;
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
    //std::memset(&value, 0, sizeof(ValueType));
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

size_t HashTable::hash_function(const KeyType &key) const {
    size_t hash;
    //std::hash<KeyType> hasher;
    //hash = hasher(key);
    
    hash = 0;
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
    }
    else {
        std::memcpy(&hash, ptr, size);
    }
    //std::cout << key << " " << hash << std::endl;
    return hash;
}


size_t HashTable::maxChainLen() const {
    size_t max = 0;
    for (auto vit = table.begin(); vit!=table.end(); vit++) {
        if ((*vit).size() > max && !(*vit).empty()) max = (*vit).size();
    }
    return max;
}