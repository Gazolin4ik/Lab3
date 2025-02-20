#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include "includes.h"

class Stack {
private:
    class Node {
    public:
        string data;
        Node* next;

        Node(const string& value) : data(value), next(nullptr) {}
    };

    Node* top;

public:
    Stack() : top(nullptr) {}

    ~Stack() {
        while (top != nullptr) {
            Node* temp = top;
            top = top->next;
            delete temp;
        }
    }

    void push(const string& value) {
        Node* newNode = new Node(value);
        newNode->next = top;
        top = newNode;
    }

    void del() {
        if (top != nullptr) {
            Node* temp = top;
            top = top->next;
            delete temp;
        }
    }

    void saveToFile(const string& filename) {
        ofstream file(filename);
        Node* current = top;
        while (current != nullptr) {
            file << current->data;
            if (current->next != nullptr) {
                file << ";";
            }
            current = current->next;
        }
        file.close();
    }
    
    // Метод сериализации: сохранение в бинарный файл
    void saveToBinaryFile(const string& filename) {
        std::ofstream file(filename, std::ios::binary);
        Node* current = top;
        while (current != nullptr) {
            size_t dataSize = current->data.size();  // Получаем размер данных
            file.write(reinterpret_cast<char*>(&dataSize), sizeof(dataSize));  // Записываем размер
            file.write(current->data.c_str(), dataSize);  // Записываем данные
            current = current->next;
        }
        file.close();
    }

    // Метод десериализации: загрузка из бинарного файла
    void loadFromBinaryFile(const string& filename) {
        std::ifstream file(filename, std::ios::binary);
        while (file) {
            size_t dataSize;
            if (!file.read(reinterpret_cast<char*>(&dataSize), sizeof(dataSize))) break;  // Чтение размера данных

            string data(dataSize, '\0');
            file.read(&data[0], dataSize);  // Чтение самих данных

            push(data);  // Добавляем данные в стек
        }
        file.close();
    }

    // Публичный метод для получения значения top
    Node* getTop() const {
        return top;
    }
};

void stack(string fileName, string actions);
bool emptyFile(string fileName);

#endif // STACK_H_INCLUDED
