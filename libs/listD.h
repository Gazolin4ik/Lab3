#ifndef LISTD_H_INCLUDED
#define LISTD_H_INCLUDED

#include "includes.h"

class ListD {
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
    ListD() : head(nullptr), tail(nullptr) {}

    ~ListD() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void pushh(const string& value) {
        Node* newNode = new Node(value);
        newNode->next = head;
        if (head != nullptr) {
            head->prev = newNode;
        }
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
            newNode->prev = tail;
            tail = newNode;
        }
    }

    void delh() {
        if (head == nullptr) {
            cout << "Список пуст." << endl;
            return;
        }
        Node* temp = head;
        head = head->next;
        if (head != nullptr) {
            head->prev = nullptr;
        } else {
            tail = nullptr;
        }
        delete temp;
    }

    void delt() {
        if (head == nullptr) {
            cout << "Список пуст." << endl;
            return;
        }
        if (head == tail) {
            delete head;
            head = tail = nullptr;
            return;
        }
        Node* temp = tail;
        tail = tail->prev;
        tail->next = nullptr;
        delete temp;
    }

    void delv(const string& value) {
        if (head == nullptr) {
            cout << "Список пуст." << endl;
            return;
        }
        if (head->data == value) {
            delh();
            return;
        }
        Node* current = head;
        while (current != nullptr && current->data != value) {
            current = current->next;
        }
        if (current == nullptr) {
            cout << "Элемент не найден." << endl;
            return;
        }
        if (current->next != nullptr) {
            current->next->prev = current->prev;
        } else {
            tail = current->prev;
        }
        if (current->prev != nullptr) {
            current->prev->next = current->next;
        } else {
            head = current->next;
        }
        delete current;
    }

    void printList() {
        Node* current = head;
        while (current != nullptr) {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
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

    void saveToBinaryFile(const string& filename) {
        std::ofstream file(filename, std::ios::binary);
        Node* current = head;
        while (current != nullptr) {
            size_t dataSize = current->data.size();
            file.write(reinterpret_cast<char*>(&dataSize), sizeof(dataSize));
            file.write(current->data.c_str(), dataSize);
            current = current->next;
        }
        file.close();
    }

    void loadFromBinaryFile(const string& filename) {
        std::ifstream file(filename, std::ios::binary);
        while (file) {
            size_t dataSize;
            if (!file.read(reinterpret_cast<char*>(&dataSize), sizeof(dataSize))) break;

            string data(dataSize, '\0');
            file.read(&data[0], dataSize);

            pusht(data);
        }
        file.close();
    }

    string getHeadData() const {
        if (head != nullptr) {
            return head->data;
        }
        return "";
    }

    // Публичные методы для получения значений head и tail
    Node* getHead() const {
        return head;
    }

    Node* getTail() const {
        return tail;
    }
};

#endif // LISTD_H_INCLUDED
