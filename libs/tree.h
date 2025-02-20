#pragma once
#include "includes.h"

class AVLTree {
private:
    struct Node {
        int key;
        int height;
        Node* left;
        Node* right;

        Node(int k) : key(k), height(1), left(nullptr), right(nullptr) {}
    };

    Node* root; // Корневой узел дерева

    // Вспомогательные функции
    int height(Node* n) const {
        return n ? n->height : 0;
    }

    int getBalance(Node* n) const {
        return n ? height(n->left) - height(n->right) : 0;
    }

    void updateHeight(Node* n) {
        if (n) {
            n->height = 1 + std::max(height(n->left), height(n->right));
        }
    }

    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        updateHeight(y);
        updateHeight(x);

        return x;
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        updateHeight(x);
        updateHeight(y);

        return y;
    }

    Node* insert(Node* node, int key) {
        if (!node) return new Node(key);

        if (key < node->key) {
            node->left = insert(node->left, key);
        } else if (key > node->key) {
            node->right = insert(node->right, key);
        } else {
            return node; // Дубликаты не допускаются
        }

        updateHeight(node);

        int balance = getBalance(node);

        // Балансировка
        if (balance > 1 && key < node->left->key) {
            return rotateRight(node);
        }

        if (balance < -1 && key > node->right->key) {
            return rotateLeft(node);
        }

        if (balance > 1 && key > node->left->key) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        if (balance < -1 && key < node->right->key) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    Node* search(Node* node, int key) const {
        if (!node || node->key == key) {
            return node;
        }

        if (key < node->key) {
            return search(node->left, key);
        }

        return search(node->right, key);
    }

    Node* minValueNode(Node* node) const {
        Node* current = node;

        while (current && current->left != nullptr) {
            current = current->left;
        }

        return current;
    }

    Node* deleteNode(Node* root, int key) {
        if (!root) return root;

        if (key < root->key) {
            root->left = deleteNode(root->left, key);
        } else if (key > root->key) {
            root->right = deleteNode(root->right, key);
        } else {
            if (!root->left || !root->right) {
                Node* temp = root->left ? root->left : root->right;

                if (!temp) {
                    temp = root;
                    root = nullptr;
                } else {
                    *root = *temp;
                }
                delete temp;
            } else {
                Node* temp = minValueNode(root->right);
                root->key = temp->key;
                root->right = deleteNode(root->right, temp->key);
            }
        }

        if (!root) return root;

        updateHeight(root);

        int balance = getBalance(root);

        if (balance > 1 && getBalance(root->left) >= 0) {
            return rotateRight(root);
        }

        if (balance < -1 && getBalance(root->right) <= 0) {
            return rotateLeft(root);
        }

        if (balance > 1 && getBalance(root->left) < 0) {
            root->left = rotateLeft(root->left);
            return rotateRight(root);
        }

        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = rotateRight(root->right);
            return rotateLeft(root);
        }

        return root;
    }

    void print(Node* node, int space, int level) const {
        if (!node) return;

        space += 5;

        print(node->right, space, level + 1);


        std::cout << std::endl << std::setw(space) << node->key;

        print(node->left, space, level + 1);
    }

    void free(Node* node) {
        if (!node) return;

        free(node->left);
        free(node->right);

        delete node;
    }

    void writeToFile(Node* node, std::ofstream& file, int space, int level) const {
        if (!node) return;

        space += 5;

        writeToFile(node->right, file, space, level + 1);

        file << std::setw(space) << node->key << std::endl;

        writeToFile(node->left, file, space, level + 1);
    }

public:
    AVLTree() : root(nullptr) {}

    ~AVLTree() {
        clear();
    }

    void serialize(const std::string& filename) const {
        std::ofstream outFile(filename, std::ios::binary);
        if (!outFile) {
            std::cerr << "Ошибка при открытии файла для сериализации." << std::endl;
            return;
        }

        serializeNode(root, outFile);
        outFile.close();
    }

    // Рекурсивная функция для сериализации узлов
    void serializeNode(Node* node, std::ofstream& outFile) const {
        if (!node) {
            bool nullNode = true;
            outFile.write(reinterpret_cast<char*>(&nullNode), sizeof(bool));  // Отметим, что узел пустой
            return;
        }

        bool nullNode = false;
        outFile.write(reinterpret_cast<char*>(&nullNode), sizeof(bool));  // Узел не пустой
        outFile.write(reinterpret_cast<char*>(&node->key), sizeof(int));  // Записываем ключ узла

        serializeNode(node->left, outFile);  // Рекурсивная запись левого поддерева
        serializeNode(node->right, outFile); // Рекурсивная запись правого поддерева
    }

    // Функция для десериализации дерева из бинарного файла
    void deserialize(const std::string& filename) {
        std::ifstream inFile(filename, std::ios::binary);
        if (!inFile) {
            std::cerr << "Ошибка при открытии файла для десериализации." << std::endl;
            return;
        }

        clear();  // Очищаем текущее дерево, если оно есть
        deserializeNode(inFile);  // Загружаем дерево из файла
        inFile.close();
    }

    // Рекурсивная функция для десериализации узлов
    void deserializeNode(std::ifstream& inFile) {
        bool nullNode;
        inFile.read(reinterpret_cast<char*>(&nullNode), sizeof(bool));  // Читаем, пустой ли узел

        if (nullNode) {
            return;  // Если узел пустой, ничего не делаем
        }

        int key;
        inFile.read(reinterpret_cast<char*>(&key), sizeof(int));  // Читаем ключ узла

        root = insert(root, key);  // Вставляем ключ в дерево

        // Рекурсивно восстанавливаем левое и правое поддерево
        deserializeNode(inFile);
        deserializeNode(inFile);
    }

    void insert(int key) {
        root = insert(root, key);
    }

    bool search(int key) const {
        return search(root, key) != nullptr;
    }

    void remove(int key) {
        root = deleteNode(root, key);
    }

    void print() const {
        print(root, 0, 0);  // Перегрузка для вызова print с параметрами по умолчанию
    }

    void clear() {
        free(root);
        root = nullptr;
    }

    void writeToFile(const std::string& filename) const {
        std::ofstream file(filename);

        if (!file.is_open()) {
            std::cerr << "Ошибка при открытии файла для записи" << std::endl;
            return;
        }

        writeToFile(root, file, 0, 0);  // Перегрузка для вызова writeToFile с параметрами по умолчанию
        file.close();
    }
};

