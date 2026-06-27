#ifndef HASH_MAP_H
#define HASH_MAP_H

#include "DynamicArray.h"
#include "LinkedList.h"
#include <string>
#include <stdexcept>
#include <functional>

template <typename K, typename V>
class HashMap {
private:
    struct KeyValuePair {
        K key;
        V value;
        KeyValuePair(const K& k, const V& v) : key(k), value(v) {}
    };

    DynamicArray<LinkedList<KeyValuePair>> buckets;
    int current_size;
    
    // Hash function
    int hash(const K& key) const {
        std::hash<K> hasher;
        // Basic modulo hashing
        return hasher(key) % buckets.capacity();
    }

public:
    HashMap(int capacity = 16) : buckets(capacity), current_size(0) {
        // Initialize empty buckets
        for (int i = 0; i < capacity; ++i) {
            buckets.append(LinkedList<KeyValuePair>());
        }
    }

    [[nodiscard]] int size() const { return current_size; }
    [[nodiscard]] bool isEmpty() const { return current_size == 0; }

    void put(const K& key, const V& value) {
        int index = hash(key);
        LinkedList<KeyValuePair>& bucket = buckets.get(index);
        
        // FIXED: Check if key exists using our LinkedList iterators!
        for (auto& pair : bucket) {
            if (pair.key == key) {
                pair.value = value; // Update existing value
                return;
            }
        }
        
        // If we reach here, it's a new key
        bucket.append(KeyValuePair(key, value));
        current_size++;
    }
};

#endif // HASH_MAP_H
