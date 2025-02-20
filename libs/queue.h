#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#include "includes.h"

class Queue {
private:
    class Node {
    public:
        string data;
        Node* next;
        Node* prev;

        Node(const string& value) : data(value), next(nullptr), prev(nullptr) {}
    };

    Node* head;
    Node* tail;

public:
    Queue() : head(nullptr), tail(nullptr) {}

    ~Queue() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void push(const string& value) {
        Node* newNode = new Node(value);
        if (tail == nullptr) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    void del() {
        Node* temp = head;
        head = head->next;
        if (head != nullptr) {
            head->prev = nullptr;
        } else {
            tail = nullptr;
        }
        delete temp;
    }

    void saveToFile(const string& filename) {
        ofstream file(filename);
        Node* current = head;
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
        Node* current = head;
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

            push(data);  // Добавляем данные в очередь
        }
        file.close();
    }

    Node* getHead() const {
        return head;
    }

    Node* getTail() const {
        return tail;
    }
};

#endif // QUEUE_H_INCLUDED
