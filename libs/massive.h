#ifndef MASSIVE_H_INCLUDED
#define MASSIVE_H_INCLUDED

#include "includes.h"

class StrArray {
private:
    string* data;
    size_t size; // размер
    size_t capacity; // ёмкость

public:
    StrArray(size_t initialCapacity = 5) {
        size = 0;
        capacity = initialCapacity; // Устанавливаем начальную емкость
        data = new string[capacity]; // Выделяем память под массив
    }
    
    // Деструктор
    ~StrArray() {
        delete[] data;
    }

    void resize() {
        size_t newCapacity = capacity * 2;
        string* newData = new string[newCapacity]; // новый массив
        for (size_t i = 0; i < size; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData; //указатель на новый массив
        capacity = newCapacity;
    }

    void push(const string& value) {
        if (size >= capacity) {
            resize();
        }
        data[size++] = value; // заносим строку и увеличиваем размер
    }

    size_t sizeM() const {
        return size; // Возвращаем текущий размер
    }

    void pushi(size_t index, const string& value) {
        if (index < 0 || index > size) {
            cout << "6:ERROR: Index out of range." << endl;
            return;
        }
        if (size >= capacity) {
            resize();
        }
        // Сдвигаем элементы вправо
        for (size_t i = size; i > index; --i) {
            data[i] = data[i - 1];
        }
        data[index] = value;
        ++size;
    }

    bool get(size_t index, string& result) {
        if (index < 0 || index >= size) {
            //cout << "10:ERROR: Index out of range." << endl;
            return false;
        }
        result = data[index];
        return true;
    }

    void replace(size_t index, const string& value) {
        if (index < 0 || index >= size) {
            cout << "9:ERROR: Index out of range." << endl;
            return;
        }
        data[index] = value;
    }

    void del(size_t index) {
        if (index < 0 || index >= size) {
            cout << "11:ERROR: Index out of range." << endl;
            return;
        }
        for (size_t i = index; i < size - 1; ++i) {
            data[i] = data[i + 1];
        }
        --size;
    }

    void saveToFile(const string& filename) const {
        ofstream file(filename);
        for (size_t i = 0; i < size; ++i) {
            file << data[i];
            if (i < size - 1) {
                file << ";";
            }
        }
        file.close();
    }

    // Метод сериализации
    void serialize(const string& filename) const {
        ofstream file(filename, ios::binary);
        if (!file) {
            cerr << "Error opening file for serialization." << endl;
            return;
        }
        file.write(reinterpret_cast<const char*>(&size), sizeof(size));
        file.write(reinterpret_cast<const char*>(&capacity), sizeof(capacity));
        for (size_t i = 0; i < size; ++i) {
            size_t len = data[i].size();
            file.write(reinterpret_cast<const char*>(&len), sizeof(len));
            file.write(data[i].c_str(), len);
        }
        file.close();
    }

    // Метод десериализации
    void deserialize(const string& filename) {
        ifstream file(filename, ios::binary);
        if (!file) {
            cerr << "Error opening file for deserialization." << endl;
            return;
        }
        file.read(reinterpret_cast<char*>(&size), sizeof(size));
        file.read(reinterpret_cast<char*>(&capacity), sizeof(capacity));
        delete[] data;
        data = new string[capacity];
        for (size_t i = 0; i < size; ++i) {
            size_t len;
            file.read(reinterpret_cast<char*>(&len), sizeof(len));
            char* buffer = new char[len + 1];
            file.read(buffer, len);
            buffer[len] = '\0';
            data[i] = string(buffer);
            delete[] buffer;
        }
        file.close();
    }

    size_t getCapacity() const {
        return capacity;
    }
};

#endif // MASSIVE_H_INCLUDED
