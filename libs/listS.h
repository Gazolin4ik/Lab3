#ifndef LISTS_H_INCLUDED
#define LISTS_H_INCLUDED

#include "includes.h"

class ListS {
private:
    class Node {
    public:
        string data;
        Node* next;

        Node(const string& value) : data(value), next(nullptr) {}
    };

    Node* head;
    Node* tail;

public:
    ListS() : head(nullptr), tail(nullptr) {}

    ~ListS() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void pushh(const string& value) {
        Node* newNode = new Node(value);
        newNode->next = head;
        head = newNode;
        if (tail == nullptr) {
            tail = head;
        }
    }

    void pusht(const string& value) {
        Node* newNode = new Node(value);
        if (tail == nullptr) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    string delh() {
        if (head == nullptr) {
            return "Список пуст.";
        }
        Node* temp = head;
        head = head->next;
        if (head == nullptr) {
            tail = nullptr;
        }
        delete temp;
        return "good";
    }

    string delt() {
        if (head == nullptr) {
            return "Список пуст.";
        }
        if (head == tail) {
            delete head;
            head = tail = nullptr;
            return "---";
        }
        Node* current = head;
        while (current->next != tail) {
            current = current->next;
        }
        delete tail;
        tail = current;
        tail->next = nullptr;
        return "good";
    }

    string delv(const string& value) {
        if (head == nullptr) {
            return "Список пуст.";
        }
        if (head->data == value) {
            delh();
            return "-";
        }
        Node* current = head;
        while (current->next != nullptr && current->next->data != value) {
            current = current->next;
        }
        if (current->next == nullptr) {
            return "Элемент не найден.";
        }
        Node* temp = current->next;
        current->next = current->next->next;
        if (current->next == nullptr) {
            tail = current;
        }
        delete temp;
        return " ";
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

            pusht(data);  // Добавляем данные в список
        }
        file.close();
    }

    // Публичные методы для получения значений head и tail
    Node* getHead() const {
        return head;
    }

    Node* getTail() const {
        return tail;
    }
};

#endif // LISTS_H_INCLUDED