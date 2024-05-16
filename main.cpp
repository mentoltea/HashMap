#include <iostream>
#include "HashTable.h"

using namespace std;
int main() {
    HashTable table(1000);
    int num = 1000;
    for (int i=0; i<num; i++) {
        table[i] = i;
    }
    cout << table.getLoadFactor() << endl;
    cout << table.maxChainLen() << endl << endl;
    
    for (int i=0; i<num; i++) {
        if (table[i] != i) cout << i << " " << table[i] << endl;
    }
    
    

    return 0;
}