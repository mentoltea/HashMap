#include <iostream>
#include "HashTable.h"

using namespace std;
int main() {
    HashTable table(5);

    vector<string> vec = {
        "three two one",
        "asdb",
        "0oerp",
        ".",
        "~~~`",
        "/]/equal",
        "sd",
        "/n\t",
        "  *space*  ",
        "one two three",
        "123091239 3"
    };
    cout << table.table.size() << endl;
    for (auto it=vec.begin(); it!=vec.end(); it++) {
        table.insert(*it, *it);
    }
    cout << table.table.size() << endl;

    cout << table.maxChainLen() << endl;
    cout << table.getLoadFactor()  << endl<< endl;

    for (auto it=vec.begin(); it!=vec.end(); it++) {
        if (!table.find(*it, *it)) cout << *it  << " = " << table[*it] << endl;
    }

    return 0;
}