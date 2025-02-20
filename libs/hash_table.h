#ifndef HASH_TABLE_H_INCLUDED
#define HASH_TABLE_H_INCLUDED

#include "includes.h"

class HashTable {
private:
    class KeyValuePair {
    public:
        string key;
        string value;
        KeyValuePair* next;

        KeyValuePair(const string& k, const string& v) : key(k), value(v), next(nullptr) {}
    };

    KeyValuePair** table;
    size_t tableSize;

    size_t hashFunction(const string& key) const {
        size_t hash = 0;
        for (char c : key) {
            hash = hash * 31 + c;
        }
        return hash % tableSize;
    }

public:
    HashTable(size_t initialCapacity = 10) : tableSize(initialCapacity) {
        table = new KeyValuePair*[tableSize];
        for (size_t i = 0; i < tableSize; ++i) {
            table[i] = nullptr;
        }
    }

    ~HashTable() {
        for (size_t i = 0; i < tableSize; ++i) {
            KeyValuePair* current = table[i];
            while (current != nullptr) {
                KeyValuePair* next = current->next;
                delete current;
                current = next;
            }
        }
        delete[] table;
    }

    void push(const string& key, const string& value) {
        size_t hash = hashFunction(key);
        KeyValuePair* current = table[hash];
        while (current != nullptr) {
            if (current->key == key) {
                cout << "6:ERROR: Key already exists." << endl;
                return;
            }
            current = current->next;
        }
        KeyValuePair* newPair = new KeyValuePair(key, value);
        newPair->next = table[hash];
        table[hash] = newPair;
    }

    bool get(const string& key, string& result) const {
        size_t hash = hashFunction(key);
        KeyValuePair* current = table[hash];
        while (current != nullptr) {
            if (current->key == key) {
                result = current->value;
                return true;
            }
            current = current->next;
        }
        return false; // Ключ не найден
    }

    bool del(const string& key) {
        size_t hash = hashFunction(key);
        KeyValuePair* current = table[hash];
        KeyValuePair* prev = nullptr;
        while (current != nullptr) {
            if (current->key == key) {
                if (prev == nullptr) {
                    table[hash] = current->next;
                } else {
                    prev->next = current->next;
                }
                delete current;
                return true;
            }
            prev = current;
            current = current->next;
        }
        return false; // Ключ не найден
    }

    // Сохранение в текстовый файл
    void saveToFile(const string& filename) const {
        ofstream file(filename);
        for (size_t i = 0; i < tableSize; ++i) {
            KeyValuePair* current = table[i];
            while (current != nullptr) {
                file << current->key << ";" << current->value << endl;
                current = current->next;
            }
        }
        file.close();
    }

    // Загрузка из текстового файла
    void loadFromFile(const string& filename) {
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            size_t pos = line.find(';');
            if (pos != string::npos) {
                string key = line.substr(0, pos);
                string value = line.substr(pos + 1);
                push(key, value);
            }
        }
        file.close();
    }

    // Сохранение в бинарный файл
    void saveToBinaryFile(const string& filename) const {
        ofstream file(filename, ios::binary);
        for (size_t i = 0; i < tableSize; ++i) {
            KeyValuePair* current = table[i];
            while (current != nullptr) {
                size_t keySize = current->key.size();
                size_t valueSize = current->value.size();
                file.write(reinterpret_cast<char*>(&keySize), sizeof(keySize));
                file.write(current->key.c_str(), keySize);
                file.write(reinterpret_cast<char*>(&valueSize), sizeof(valueSize));
                file.write(current->value.c_str(), valueSize);
                current = current->next;
            }
        }
        file.close();
    }

    // Загрузка из бинарного файла
    void loadFromBinaryFile(const string& filename) {
        ifstream file(filename, ios::binary);
        while (file) {
            size_t keySize, valueSize;
            if (!file.read(reinterpret_cast<char*>(&keySize), sizeof(keySize))) break;

            string key(keySize, '\0');
            file.read(&key[0], keySize);

            file.read(reinterpret_cast<char*>(&valueSize), sizeof(valueSize));
            string value(valueSize, '\0');
            file.read(&value[0], valueSize);

            push(key, value);
        }
        file.close();
    }

    size_t getCapacity() const {
        return tableSize;
    }
};

#endif // HASH_TABLE_H_INCLUDED
