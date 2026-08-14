#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Hash {
    // Number of buckets
    int bucketCount;
    vector<vector<int>> table;

public:
    // Constructor to initialize the hash 
    // table with given bucket count
    Hash(int buckets) {
        bucketCount = buckets;
        table.resize(bucketCount);
    }

    // Function to insert a key into hash table
    void insert(int key) {
        // Get the hash index for the key
        int index = getHashIndex(key);

        // Insert the key into the corresponding bucket
        table[index].push_back(key);
    }

    // Function to delete a key from hash table
    void remove(int key) {
        // Get the hash index for the key
        int index = getHashIndex(key);

        // Find and remove the key from the bucket
        auto it = find(table[index].begin(), table[index].end(), key);

        // Erase the key if found
        if (it != table[index].end()) {
            table[index].erase(it);
        }
    }

    // Function to display the hash table
    void display() {
        for (int i = 0; i < bucketCount; i++) {
            cout << i;

            // Print all keys in current bucket
            for (int key : table[i]) {
                cout << " --> " << key;
            }

            cout << endl;
        }
    }

private:
    // Simple hash function to map key to index
    int getHashIndex(int key) {
        return key % bucketCount;
    }
};

int main() {
    vector<int> keys = {7, 18, 12, 25};

    Hash hashTable(7);

    for (int key : keys) {
        hashTable.insert(key);
    }

    hashTable.remove(12);

    hashTable.display();

    return 0;
}