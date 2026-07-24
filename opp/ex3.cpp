#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// HashTable class to handle insert, delete, 
// display, and rehash operations
class Hash {
    
    // Number of buckets
    int bucketCount; 
    
    // Number of elements
    int numOfElements; 
    
    // Vector of chains
    vector<vector<int>> table; 

public:

    // Constructor to initialize the hash table
    Hash(int buckets) {
        bucketCount = buckets;
        numOfElements = 0;
        table.resize(bucketCount);
    }

    // Insert a key into the hash table
    void insert(int key) {
        while (getLoadFactor() > 0.5) {
            rehash();
        }
        int index = getHashIndex(key);
        table[index].push_back(key);
        numOfElements++;
    }

    // Remove a key from the hash table
    void remove(int key) {
        int index = getHashIndex(key);
        auto it = find(table[index].begin(), 
                        table[index].end(), key);
        if (it != table[index].end()) {
            table[index].erase(it);
            numOfElements--;
        }
    }

    // Display the current hash table
    void display() {
        for (int i = 0; i < bucketCount; i++) {
            cout << i;
            for (int key : table[i]) {
                cout << " --> " << key;
            }
            cout << endl;
        }
    }

private:
    // Hash function to map key to index
    int getHashIndex(int key) {
        return key % bucketCount;
    }

    // Calculate load factor
    float getLoadFactor() {
        return (float)numOfElements / bucketCount;
    }

    // Rehash the table by doubling the size
    void rehash() {
        vector<vector<int>> oldTable = table;
        bucketCount *= 2;
        table.clear();
        table.resize(bucketCount);
        numOfElements = 0;
        for (const auto& bucket : oldTable) {
            for (int key : bucket) {
                insert(key);
            }
        }
    }
};

int main() {
    vector<int> keys = {15, 11, 27};

    Hash hashTable(5);

    for (int key : keys) {
        hashTable.insert(key);
    }

    // hashTable.remove(11);
    // hashTable.display();

    hashTable.insert(19);

    cout << "\nAfter rehashing:\n";
    hashTable.display();

    return 0;
}