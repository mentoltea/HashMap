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

    for (int i = 0; i < vec.size(); i++) {
        table[vec[i]] = i;
    }

    cout << table.maxChainLen() << endl;
    cout << table.getLoadFactor()  << endl<< endl;

    for (int i = 0; i < vec.size(); i++) {
        if (table[vec[i]] != i) cout << vec[i] << " problem" << endl;
    }
    return 0;
}