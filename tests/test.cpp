#include "gtest/gtest.h"
#include "../libs/hash_table.h"
#include "../libs/listD.h"
#include "../libs/listS.h"
#include "../libs/massive.h"
#include "../libs/queue.h"
#include "../libs/stack.h"
#include "../libs/tree.h"

// Тесты для массива --------------------------------------------------------------------------------------------------------

// Тест инициализации массива
TEST(StrArrayTest, Initialization) {
    size_t initialCapacity = 5;
    StrArray array(initialCapacity);
    EXPECT_EQ(array.sizeM(), 0); // Проверка, что размер массива равен 0 после инициализации
    EXPECT_EQ(array.getCapacity(), initialCapacity); // Проверка, что емкость массива соответствует заданной
}

// Тест добавления элементов в массив
TEST(StrArrayTest, Push) {
    StrArray array;
    array.push("hello"); // Добавление первого элемента
    EXPECT_EQ(array.sizeM(), 1);
    array.push("world"); // Добавление второго элемента
    EXPECT_EQ(array.sizeM(), 2);
    string result;
    EXPECT_TRUE(array.get(0, result)); // Проверка, что первый элемент равен "hello"
    EXPECT_EQ(result, "hello");
    EXPECT_TRUE(array.get(1, result)); // Проверка, что второй элемент равен "world"
    EXPECT_EQ(result, "world");
}

// Тест изменения размера массива
TEST(StrArrayTest, Resize) {
    StrArray array(2);
    array.push("one"); // Добавление первого элемента
    array.push("two"); // Добавление второго элемента
    array.push("three"); // Добавление третьего элемента, что должно вызвать изменение размера массива
    EXPECT_EQ(array.sizeM(), 3);
    EXPECT_EQ(array.getCapacity(), 4); // Проверка, что емкость массива увеличилась до 4
}

// Тест вставки элемента в массив по индексу
TEST(StrArrayTest, Pushi) {
    StrArray array;
    array.push("one");
    array.push("two");
    array.pushi(1, "three"); // Вставка элемента "three" на позицию 1
    EXPECT_EQ(array.sizeM(), 3);
    string result;
    EXPECT_TRUE(array.get(0, result)); // Проверка, что первый элемент остался "one"
    EXPECT_EQ(result, "one");
    EXPECT_TRUE(array.get(1, result)); // Проверка, что второй элемент теперь "three"
    EXPECT_EQ(result, "three");
    EXPECT_TRUE(array.get(2, result)); // Проверка, что третий элемент теперь "two"
    EXPECT_EQ(result, "two");
}

// Тест вставки элемента за пределами массива
TEST(StrArrayTest, PushiOutOfBounds) {
    StrArray array;
    array.push("one");
    array.push("two");
    testing::internal::CaptureStdout();
    array.pushi(3, "three"); // Попытка вставки за пределами массива
    string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "6:ERROR: Index out of range.\n"); // Проверка сообщения об ошибке
    EXPECT_EQ(array.sizeM(), 2); // Размер массива не должен измениться
    string result;
    EXPECT_TRUE(array.get(0, result)); // Проверка, что первый элемент остался "one"
    EXPECT_EQ(result, "one");
    EXPECT_TRUE(array.get(1, result)); // Проверка, что второй элемент остался "two"
    EXPECT_EQ(result, "two");
}

// Тест получения элемента по индексу
TEST(StrArrayTest, Get) {
    StrArray array;
    array.push("one");
    array.push("two");
    string result;
    EXPECT_TRUE(array.get(0, result)); // Проверка, что первый элемент равен "one"
    EXPECT_EQ(result, "one");
    EXPECT_TRUE(array.get(1, result)); // Проверка, что второй элемент равен "two"
    EXPECT_EQ(result, "two");
    EXPECT_FALSE(array.get(2, result)); // Попытка получить элемент за пределами массива
}

// Тест замены элемента по индексу
TEST(StrArrayTest, Replace) {
    StrArray array;
    array.push("one");
    array.push("two");
    array.replace(1, "three"); // Замена второго элемента на "three"
    string result;
    EXPECT_TRUE(array.get(1, result)); // Проверка, что второй элемент теперь "three"
    EXPECT_EQ(result, "three");
}

// Тест удаления элемента по индексу
TEST(StrArrayTest, Delete) {
    StrArray array;
    array.push("one");
    array.push("two");
    array.push("three");
    array.del(1); // Удаление второго элемента
    EXPECT_EQ(array.sizeM(), 2); // Проверка, что размер массива уменьшился до 2
    string result;
    EXPECT_TRUE(array.get(0, result)); // Проверка, что первый элемент остался "one"
    EXPECT_EQ(result, "one");
    EXPECT_TRUE(array.get(1, result)); // Проверка, что второй элемент теперь "three"
    EXPECT_EQ(result, "three");
}

// Тест сохранения массива в файл
TEST(StrArrayTest, SaveToFile) {
    StrArray array;
    array.push("one");
    array.push("two");
    array.push("three");
    array.saveToFile("test.txt"); // Сохранение массива в файл
    ifstream file("test.txt");
    string content;
    file >> content;
    EXPECT_EQ(content, "one;two;three"); // Проверка содержимого файла
    file.close();
    remove("test.txt"); // Удаление тестового файла
}

// Тест сериализации и десериализации массива
TEST(StrArrayTest, SerializeDeserialize) {
    StrArray array;
    array.push("Hello");
    array.push("World");
    array.push("Google");
    array.push("Test");

    array.serialize("test_serialize.bin"); // Сериализация массива в файл

    StrArray newArray;
    newArray.deserialize("test_serialize.bin"); // Десериализация массива из файла

    ASSERT_EQ(array.sizeM(), newArray.sizeM()); // Проверка, что размеры совпадают
    ASSERT_EQ(array.getCapacity(), newArray.getCapacity()); // Проверка, что емкости совпадают

    for (size_t i = 0; i < array.sizeM(); ++i) {
        string originalValue;
        string newValue;
        ASSERT_TRUE(array.get(i, originalValue)); // Проверка исходного массива
        ASSERT_TRUE(newArray.get(i, newValue)); // Проверка десериализованного массива
        ASSERT_EQ(originalValue, newValue); // Проверка, что элементы совпадают
    }
}

// Тесты для хеш-таблицы --------------------------------------------------------------------------------------------------

// Тест создания хеш-таблицы
TEST(HashTableTest, CreateHashTable) {
    HashTable table(10);
    EXPECT_EQ(table.getCapacity(), 10); // Проверка, что емкость таблицы соответствует заданной
}

// Тест добавления элемента с существующим ключом
TEST(HashTableTest, PushExistingKey) {
    HashTable table(10);
    table.push("key1", "value1");
    testing::internal::CaptureStdout();
    table.push("key1", "value2"); // Попытка добавить элемент с существующим ключом
    string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "6:ERROR: Key already exists.\n"); // Проверка сообщения об ошибке
}

// Тест успешного добавления элемента
TEST(HashTableTest, PushSuccess) {
    HashTable table(10);
    table.push("key1", "value1"); // Добавление элемента
    string result;
    EXPECT_TRUE(table.get("key1", result)); // Проверка, что элемент добавлен
    EXPECT_EQ(result, "value1");
}

// Тест поиска несуществующего ключа
TEST(HashTableTest, GetKeyNotFound) {
    HashTable table(10);
    string result;
    EXPECT_FALSE(table.get("key1", result)); // Попытка получить элемент с несуществующим ключом
}

// Тест поиска существующего ключа
TEST(HashTableTest, GetKeyFound) {
    HashTable table(10);
    table.push("key1", "value1"); // Добавление элемента
    string result;
    EXPECT_TRUE(table.get("key1", result)); // Проверка, что элемент найден
    EXPECT_EQ(result, "value1");
}

// Тест успешного удаления элемента
TEST(HashTableTest, DeleteKeySuccess) {
    HashTable table(10);
    table.push("key1", "value1"); // Добавление элемента
    EXPECT_TRUE(table.del("key1")); // Удаление элемента
    string result;
    EXPECT_FALSE(table.get("key1", result)); // Проверка, что элемент удален
}

// Тест удаления несуществующего ключа
TEST(HashTableTest, DeleteKeyNotFound) {
    HashTable table(10);
    EXPECT_FALSE(table.del("key1")); // Попытка удалить элемент с несуществующим ключом
}

// Тест сохранения и загрузки хеш-таблицы из файла
TEST(HashTableTest, SaveAndLoadFromFile) {
    HashTable table(10);
    table.push("key1", "value1");
    table.push("key2", "value2");
    table.saveToFile("test.txt"); // Сохранение таблицы в файл

    HashTable loadedTable(10);
    loadedTable.loadFromFile("test.txt"); // Загрузка таблицы из файла

    string result;
    EXPECT_TRUE(loadedTable.get("key1", result)); // Проверка, что элемент "key1" загружен
    EXPECT_EQ(result, "value1");
    EXPECT_TRUE(loadedTable.get("key2", result)); // Проверка, что элемент "key2" загружен
    EXPECT_EQ(result, "value2");
    remove("test.txt"); // Удаление тестового файла
}

// Тест большого количества операций с хеш-таблицей
TEST(HashTableTest, LargeNumberOfOperations) {
    HashTable table(100);
    for (int i = 0; i < 1000; ++i) {
        table.push("key" + to_string(i), "value" + to_string(i)); // Добавление 1000 элементов
    }
    for (int i = 0; i < 1000; ++i) {
        string result;
        EXPECT_TRUE(table.get("key" + to_string(i), result)); // Проверка, что все элементы доступны
        EXPECT_EQ(result, "value" + to_string(i));
    }
    for (int i = 0; i < 1000; ++i) {
        EXPECT_TRUE(table.del("key" + to_string(i))); // Удаление всех элементов
    }
    for (int i = 0; i < 1000; ++i) {
        string result;
        EXPECT_FALSE(table.get("key" + to_string(i), result)); // Проверка, что все элементы удалены
    }
}

// Тест сохранения хеш-таблицы в бинарный файл
TEST(HashTableTest, SaveToBinaryFile) {
    HashTable hashTable;
    hashTable.push("name", "John Doe"); // Добавление элемента
    const string filename = "test_data.bin";
    hashTable.saveToBinaryFile(filename); // Сохранение в бинарный файл

    std::ifstream file(filename, std::ios::binary);
    ASSERT_TRUE(file.is_open()); // Проверка, что файл открыт

    size_t keySize, valueSize;
    string key, value;

    // Проверка содержимого файла
    ASSERT_TRUE(file.read(reinterpret_cast<char*>(&keySize), sizeof(keySize)));
    key.resize(keySize);
    file.read(&key[0], keySize);
    ASSERT_EQ(key, "name"); // Проверка ключа

    ASSERT_TRUE(file.read(reinterpret_cast<char*>(&valueSize), sizeof(valueSize)));
    value.resize(valueSize);
    file.read(&value[0], valueSize);
    ASSERT_EQ(value, "John Doe"); // Проверка значения

    file.close();
}

// Тест загрузки хеш-таблицы из бинарного файла
TEST(HashTableTest, LoadFromBinaryFile) {
    HashTable hashTable;
    hashTable.push("name", "John Doe"); // Добавление элемента
    const string filename = "test_data.bin";
    hashTable.saveToBinaryFile(filename); // Сохранение в бинарный файл

    HashTable loadedTable;
    loadedTable.loadFromBinaryFile(filename); // Загрузка из бинарного файла

    std::string value;
    ASSERT_TRUE(loadedTable.get("name", value)); // Проверка, что элемент загружен
    ASSERT_EQ(value, "John Doe");
    fs::remove(filename); // Удаление тестового файла
}

// Тесты для двусвязного списка -------------------------------------------------------------------------------------------

// Тест создания двусвязного списка
TEST(ListDTest, CreateList) {
    ListD list;
    EXPECT_EQ(list.getHead(), nullptr); // Проверка, что голова списка пуста
    EXPECT_EQ(list.getTail(), nullptr); // Проверка, что хвост списка пуст
}

// Тест добавления элементов в начало списка
TEST(ListDTest, PushHead) {
    ListD list;
    list.pushh("value1"); // Добавление первого элемента
    EXPECT_EQ(list.getHead()->data, "value1"); // Проверка, что голова списка содержит "value1"
    EXPECT_EQ(list.getTail()->data, "value1"); // Проверка, что хвост списка содержит "value1"

    list.pushh("value2"); // Добавление второго элемента
    EXPECT_EQ(list.getHead()->data, "value2"); // Проверка, что голова списка теперь "value2"
    EXPECT_EQ(list.getHead()->next->data, "value1"); // Проверка, что следующий элемент после головы - "value1"
    EXPECT_EQ(list.getTail()->data, "value1"); // Проверка, что хвост списка остался "value1"
}

// Тест добавления элементов в конец списка
TEST(ListDTest, PushTail) {
    ListD list;
    list.pusht("value1"); // Добавление первого элемента
    EXPECT_EQ(list.getHead()->data, "value1"); // Проверка, что голова списка содержит "value1"
    EXPECT_EQ(list.getTail()->data, "value1"); // Проверка, что хвост списка содержит "value1"

    list.pusht("value2"); // Добавление второго элемента
    EXPECT_EQ(list.getHead()->data, "value1"); // Проверка, что голова списка осталась "value1"
    EXPECT_EQ(list.getHead()->next->data, "value2"); // Проверка, что следующий элемент после головы - "value2"
    EXPECT_EQ(list.getTail()->data, "value2"); // Проверка, что хвост списка теперь "value2"
}

// Тест удаления элемента из головы списка
TEST(ListDTest, DeleteHead) {
    ListD list;
    list.pushh("value1"); // Добавление элемента
    list.delh(); // Удаление элемента из головы
    EXPECT_EQ(list.getHead(), nullptr); // Проверка, что голова списка пуста
    EXPECT_EQ(list.getTail(), nullptr); // Проверка, что хвост списка пуст

    list.pushh("value1");
    list.pushh("value2");
    list.delh(); // Удаление элемента из головы
    EXPECT_EQ(list.getHead()->data, "value1"); // Проверка, что голова списка теперь "value1"
    EXPECT_EQ(list.getTail()->data, "value1"); // Проверка, что хвост списка теперь "value1"
}

// Тест удаления элемента из хвоста списка
TEST(ListDTest, DeleteTail) {
    ListD list;
    list.pusht("value1"); // Добавление элемента
    list.delt(); // Удаление элемента из хвоста
    EXPECT_EQ(list.getHead(), nullptr); // Проверка, что голова списка пуста
    EXPECT_EQ(list.getTail(), nullptr); // Проверка, что хвост списка пуст

    list.pusht("value1");
    list.pusht("value2");
    list.delt(); // Удаление элемента из хвоста
    EXPECT_EQ(list.getHead()->data, "value1"); // Проверка, что голова списка осталась "value1"
    EXPECT_EQ(list.getTail()->data, "value1"); // Проверка, что хвост списка теперь "value1"
}

// Тест удаления элемента по значению
TEST(ListDTest, DeleteValue) {
    ListD list;
    list.pushh("value1");
    list.pushh("value2");
    list.pushh("value3");
    list.delv("value2"); // Удаление элемента "value2"
    EXPECT_EQ(list.getHead()->data, "value3"); // Проверка, что голова списка теперь "value3"
    EXPECT_EQ(list.getHead()->next->data, "value1"); // Проверка, что следующий элемент после головы - "value1"
    EXPECT_EQ(list.getTail()->data, "value1"); // Проверка, что хвост списка теперь "value1"

    testing::internal::CaptureStdout();
    list.delv("value4"); // Попытка удалить несуществующий элемент
    string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "Элемент не найден.\n"); // Проверка сообщения об ошибке
    EXPECT_EQ(list.getHead()->data, "value3"); // Проверка, что список не изменился
    EXPECT_EQ(list.getHead()->next->data, "value1");
    EXPECT_EQ(list.getTail()->data, "value1");
}

// Тест вывода списка
TEST(ListDTest, PrintList) {
    ListD list;
    list.pushh("value1");
    list.pushh("value2");
    list.pushh("value3");

    testing::internal::CaptureStdout();
    list.printList(); // Вывод списка
    string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "value3 value2 value1 \n"); // Проверка вывода
}

// Тест сохранения списка в файл
TEST(ListDTest, SaveToFile) {
    ListD list;
    list.pushh("value1");
    list.pushh("value2");
    list.pushh("value3");

    string filename = "test_list.txt";
    list.saveToFile(filename); // Сохранение списка в файл

    ifstream file(filename);
    stringstream buffer;
    buffer << file.rdbuf();
    string file_contents = buffer.str();
    EXPECT_EQ(file_contents, "value3;value2;value1"); // Проверка содержимого файла

    remove(filename.c_str()); // Удаление тестового файла
}

// Тест сохранения списка в бинарный файл
TEST(ListDTest, SaveToBinaryFile) {
    ListD list;
    list.pusht("First"); // Добавление элемента
    const string filename = "list_data.bin";
    list.saveToBinaryFile(filename); // Сохранение в бинарный файл

    std::ifstream file(filename, std::ios::binary);
    ASSERT_TRUE(file.is_open()); // Проверка, что файл открыт

    size_t dataSize;
    string data;

    // Проверка содержимого файла
    ASSERT_TRUE(file.read(reinterpret_cast<char*>(&dataSize), sizeof(dataSize)));
    data.resize(dataSize);
    file.read(&data[0], dataSize);
    ASSERT_EQ(data, "First"); // Проверка, что элемент сохранен

    file.close();
    remove(filename.c_str()); // Удаление тестового файла
}

// Тест загрузки списка из бинарного файла
TEST(ListDTest, LoadFromBinaryFile) {
    ListD list;
    list.pusht("First"); // Добавление элемента
    const string filename = "list_data.bin";
    list.saveToBinaryFile(filename); // Сохранение в бинарный файл

    ListD newList;
    newList.loadFromBinaryFile(filename); // Загрузка из бинарного файла

    string currentData = newList.getHeadData(); // Получение данных из головы списка
    ASSERT_NE(currentData, ""); // Проверка, что данные загружены
    ASSERT_EQ(currentData, "First"); // Проверка, что загруженный элемент равен "First"

    remove(filename.c_str()); // Удаление тестового файла
}

// Тесты для односвязного списка ------------------------------------------------------------------------------------------

// Тест создания односвязного списка
TEST(ListSTest, CreateList) {
    ListS list;
    EXPECT_EQ(list.getHead(), nullptr); // Проверка, что голова списка пуста
    EXPECT_EQ(list.getTail(), nullptr); // Проверка, что хвост списка пуст
}

// Тест добавления элементов в начало списка
TEST(ListSTest, PushHead) {
    ListS list;
    list.pushh("value1"); // Добавление первого элемента
    EXPECT_EQ(list.getHead()->data, "value1"); // Проверка, что голова списка содержит "value1"
    EXPECT_EQ(list.getTail()->data, "value1"); // Проверка, что хвост списка содержит "value1"

    list.pushh("value2"); // Добавление второго элемента
    EXPECT_EQ(list.getHead()->data, "value2"); // Проверка, что голова списка теперь "value2"
    EXPECT_EQ(list.getHead()->next->data, "value1"); // Проверка, что следующий элемент после головы - "value1"
    EXPECT_EQ(list.getTail()->data, "value1"); // Проверка, что хвост списка остался "value1"
}

// Тест добавления элементов в конец списка
TEST(ListSTest, PushTail) {
    ListS list;
    list.pusht("value1"); // Добавление первого элемента
    EXPECT_EQ(list.getHead()->data, "value1"); // Проверка, что голова списка содержит "value1"
    EXPECT_EQ(list.getTail()->data, "value1"); // Проверка, что хвост списка содержит "value1"

    list.pusht("value2"); // Добавление второго элемента
    EXPECT_EQ(list.getHead()->data, "value1"); // Проверка, что голова списка осталась "value1"
    EXPECT_EQ(list.getHead()->next->data, "value2"); // Проверка, что следующий элемент после головы - "value2"
    EXPECT_EQ(list.getTail()->data, "value2"); // Проверка, что хвост списка теперь "value2"
}

// Тест удаления элемента из головы списка
TEST(ListSTest, DeleteHead) {
    ListS list;
    list.pushh("value1"); // Добавление элемента
    list.delh(); // Удаление элемента из головы
    EXPECT_EQ(list.getHead(), nullptr); // Проверка, что голова списка пуста
    EXPECT_EQ(list.getTail(), nullptr); // Проверка, что хвост списка пуст

    list.pushh("value1");
    list.pushh("value2");
    list.delh(); // Удаление элемента из головы
    EXPECT_EQ(list.getHead()->data, "value1"); // Проверка, что голова списка теперь "value1"
    EXPECT_EQ(list.getTail()->data, "value1"); // Проверка, что хвост списка теперь "value1"
}

// Тест удаления элемента из хвоста списка
TEST(ListSTest, DeleteTail) {
    ListS list;
    list.pusht("value1"); // Добавление элемента
    list.delt(); // Удаление элемента из хвоста
    EXPECT_EQ(list.getHead(), nullptr); // Проверка, что голова списка пуста
    EXPECT_EQ(list.getTail(), nullptr); // Проверка, что хвост списка пуст

    list.pusht("value1");
    list.pusht("value2");
    list.delt(); // Удаление элемента из хвоста
    EXPECT_EQ(list.getHead()->data, "value1"); // Проверка, что голова списка осталась "value1"
    EXPECT_EQ(list.getTail()->data, "value1"); // Проверка, что хвост списка теперь "value1"
}

// Тест удаления элемента по значению
TEST(ListSTest, DelvHeadElement) {
    ListS list;
    list.pushh("value1");
    list.pushh("value2");
    EXPECT_EQ(list.delv("value2"), "-"); // Удаление элемента "value2"
    EXPECT_EQ(list.getHead()->data, "value1"); // Проверка, что голова списка теперь "value1"
}

// Тест удаления единственного элемента из списка
TEST(ListSTest, DelhSingleElementList) {
    ListS list;
    list.pushh("value1"); // Добавление элемента
    EXPECT_EQ(list.delh(), "good"); // Удаление элемента из головы
    EXPECT_EQ(list.getHead(), nullptr); // Проверка, что голова списка пуста
    EXPECT_EQ(list.getTail(), nullptr); // Проверка, что хвост списка пуст
}

// Тест удаления элемента из пустого списка
TEST(ListSTest, DeltEmptyList) {
    ListS list;
    EXPECT_EQ(list.delt(), "Список пуст."); // Попытка удалить элемент из пустого списка
}

// Тест удаления несуществующего элемента
TEST(ListSTest, DelvElementNotFound) {
    ListS list;
    list.pushh("value1");
    list.pushh("value2");
    EXPECT_EQ(list.delv("value3"), "Элемент не найден."); // Попытка удалить несуществующий элемент
}

// Тест сохранения списка в файл
TEST(ListSTest, SaveToFile) {
    ListS list;
    list.pushh("value1");
    list.pushh("value2");
    list.pushh("value3");

    string filename = "test_list.txt";
    list.saveToFile(filename); // Сохранение списка в файл

    ifstream file(filename);
    stringstream buffer;
    buffer << file.rdbuf();
    string file_contents = buffer.str();
    EXPECT_EQ(file_contents, "value3;value2;value1"); // Проверка содержимого файла

    fs::remove(filename); // Удаление тестового файла
}

// Тест сериализации и десериализации списка
TEST(ListSTest, SerializationDeserialization) {
    ListS list;
    list.pusht("test_value"); // Добавление элемента
    list.saveToBinaryFile("test_serialization.bin"); // Сериализация списка в бинарный файл

    ListS newList;
    newList.loadFromBinaryFile("test_serialization.bin"); // Десериализация списка из файла

    ASSERT_NE(newList.getHead(), nullptr); // Проверка, что голова списка не пуста
    ASSERT_EQ(newList.getHead()->data, "test_value"); // Проверка, что данные загружены
    ASSERT_NE(newList.getTail(), nullptr); // Проверка, что хвост списка не пуст
    ASSERT_EQ(newList.getTail()->data, "test_value"); // Проверка, что данные загружены

    fs::remove("test_serialization.bin"); // Удаление тестового файла
}

// Тесты для очереди ------------------------------------------------------------------------------------------------------

// Тест создания очереди
TEST(QueueTest, CreateQueue) {
    Queue queue;
    EXPECT_EQ(queue.getHead(), nullptr); // Проверка, что голова очереди пуста
    EXPECT_EQ(queue.getTail(), nullptr); // Проверка, что хвост очереди пуст
}

// Тест добавления элементов в очередь
TEST(QueueTest, Push) {
    Queue queue;
    queue.push("value1"); // Добавление первого элемента
    EXPECT_EQ(queue.getHead()->data, "value1"); // Проверка, что голова очереди содержит "value1"
    EXPECT_EQ(queue.getTail()->data, "value1"); // Проверка, что хвост очереди содержит "value1"

    queue.push("value2"); // Добавление второго элемента
    EXPECT_EQ(queue.getHead()->data, "value1"); // Проверка, что голова очереди осталась "value1"
    EXPECT_EQ(queue.getTail()->data, "value2"); // Проверка, что хвост очереди теперь "value2"
    EXPECT_EQ(queue.getTail()->prev->data, "value1"); // Проверка, что предыдущий элемент хвоста - "value1"
}

// Тест удаления элементов из очереди
TEST(QueueTest, Delete) {
    Queue queue;
    queue.push("value1"); // Добавление элемента
    queue.del(); // Удаление элемента
    EXPECT_EQ(queue.getHead(), nullptr); // Проверка, что голова очереди пуста
    EXPECT_EQ(queue.getTail(), nullptr); // Проверка, что хвост очереди пуст

    queue.push("value1");
    queue.push("value2");
    queue.del(); // Удаление элемента
    EXPECT_EQ(queue.getHead()->data, "value2"); // Проверка, что голова очереди теперь "value2"
    EXPECT_EQ(queue.getTail()->data, "value2"); // Проверка, что хвост очереди теперь "value2"
}

// Тест сохранения очереди в файл
TEST(QueueTest, SaveToFile) {
    Queue queue;
    queue.push("value1");
    queue.push("value2");
    queue.push("value3");

    string filename = "test_queue.txt";
    queue.saveToFile(filename); // Сохранение очереди в файл

    ifstream file(filename);
    stringstream buffer;
    buffer << file.rdbuf();
    string file_contents = buffer.str();
    EXPECT_EQ(file_contents, "value1;value2;value3"); // Проверка содержимого файла

    remove(filename.c_str()); // Удаление тестового файла
}

// Тест сериализации и десериализации очереди
TEST(QueueTest, SerializationDeserialization) {
    Queue queue;
    queue.push("test_value"); // Добавление элемента
    queue.saveToBinaryFile("test_serialization.bin"); // Сериализация очереди в бинарный файл

    Queue newQueue;
    newQueue.loadFromBinaryFile("test_serialization.bin"); // Десериализация очереди из файла

    ASSERT_NE(newQueue.getHead(), nullptr); // Проверка, что голова очереди не пуста
    ASSERT_EQ(newQueue.getHead()->data, "test_value"); // Проверка, что данные загружены
    ASSERT_NE(newQueue.getTail(), nullptr); // Проверка, что хвост очереди не пуст
    ASSERT_EQ(newQueue.getTail()->data, "test_value"); // Проверка, что данные загружены

    fs::remove("test_serialization.bin"); // Удаление тестового файла
}

// Тесты для стека --------------------------------------------------------------------------------------------------------

// Тест создания стека
TEST(StackTest, CreateStack) {
    Stack stack;
    EXPECT_EQ(stack.getTop(), nullptr); // Проверка, что вершина стека пуста
}

// Тест добавления элементов в стек
TEST(StackTest, Push) {
    Stack stack;
    stack.push("value1"); // Добавление первого элемента
    EXPECT_EQ(stack.getTop()->data, "value1"); // Проверка, что вершина стека содержит "value1"

    stack.push("value2"); // Добавление второго элемента
    EXPECT_EQ(stack.getTop()->data, "value2"); // Проверка, что вершина стека теперь "value2"
    EXPECT_EQ(stack.getTop()->next->data, "value1"); // Проверка, что следующий элемент после вершины - "value1"
}

// Тест удаления элементов из стека
TEST(StackTest, Delete) {
    Stack stack;
    stack.push("value1"); // Добавление элемента
    stack.del(); // Удаление элемента
    EXPECT_EQ(stack.getTop(), nullptr); // Проверка, что вершина стека пуста

    stack.push("value1");
    stack.push("value2");
    stack.del(); // Удаление элемента
    EXPECT_EQ(stack.getTop()->data, "value1"); // Проверка, что вершина стека теперь "value1"
}

// Тест сохранения стека в файл
TEST(StackTest, SaveToFile) {
    Stack stack;
    stack.push("value1");
    stack.push("value2");
    stack.push("value3");

    string filename = "test_stack.txt";
    stack.saveToFile(filename); // Сохранение стека в файл

    ifstream file(filename);
    stringstream buffer;
    buffer << file.rdbuf();
    string file_contents = buffer.str();
    EXPECT_EQ(file_contents, "value3;value2;value1"); // Проверка содержимого файла

    remove(filename.c_str()); // Удаление тестового файла
}

// Тест сериализации и десериализации стека
TEST(StackTest, SerializationDeserialization) {
    Stack stack;
    stack.push("test_value"); // Добавление элемента
    stack.saveToBinaryFile("test_serialization.bin"); // Сериализация стека в бинарный файл

    Stack newStack;
    newStack.loadFromBinaryFile("test_serialization.bin"); // Десериализация стека из файла

    ASSERT_NE(newStack.getTop(), nullptr); // Проверка, что вершина стека не пуста
    ASSERT_EQ(newStack.getTop()->data, "test_value"); // Проверка, что данные загружены

    fs::remove("test_serialization.bin"); // Удаление тестового файла
}

// Тесты для AVL дерева ---------------------------------------------------------------------------------------------------

// Тест вставки и поиска элементов в AVL дереве
TEST(AVLTreeTest, InsertAndSearch) {
    AVLTree tree;
    tree.insert(10); // Вставка элемента 10
    tree.insert(20); // Вставка элемента 20
    tree.insert(30); // Вставка элемента 30

    EXPECT_TRUE(tree.search(10)); // Проверка, что элемент 10 найден
    EXPECT_TRUE(tree.search(20)); // Проверка, что элемент 20 найден
    EXPECT_TRUE(tree.search(30)); // Проверка, что элемент 30 найден
    EXPECT_FALSE(tree.search(40)); // Проверка, что элемент 40 не найден
}

// Тест удаления элементов из AVL дерева
TEST(AVLTreeTest, Remove) {
    AVLTree tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);

    tree.remove(20); // Удаление элемента 20
    EXPECT_FALSE(tree.search(20)); // Проверка, что элемент 20 удален
    EXPECT_TRUE(tree.search(10)); // Проверка, что элемент 10 остался
    EXPECT_TRUE(tree.search(30)); // Проверка, что элемент 30 остался

    tree.remove(10); // Удаление элемента 10
    EXPECT_FALSE(tree.search(10)); // Проверка, что элемент 10 удален
    EXPECT_TRUE(tree.search(30)); // Проверка, что элемент 30 остался

    tree.remove(30); // Удаление элемента 30
    EXPECT_FALSE(tree.search(30)); // Проверка, что элемент 30 удален
}

// Тест сериализации и десериализации AVL дерева
TEST(AVLTreeTest, SerializeDeserialize) {
    AVLTree tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);

    std::string filename = "test_tree.dat";
    tree.serialize(filename); // Сериализация дерева в файл

    AVLTree newTree;
    newTree.deserialize(filename); // Десериализация дерева из файла

    EXPECT_TRUE(newTree.search(10)); // Проверка, что элемент 10 загружен
    EXPECT_TRUE(newTree.search(20)); // Проверка, что элемент 20 загружен
    EXPECT_TRUE(newTree.search(30)); // Проверка, что элемент 30 загружен
    EXPECT_FALSE(newTree.search(40)); // Проверка, что элемент 40 не загружен
}

// Тест для пустого AVL дерева
TEST(AVLTreeTest, EmptyTree) {
    AVLTree tree;
    EXPECT_FALSE(tree.search(10)); // Проверка, что элемент 10 не найден
    tree.remove(10); // Попытка удалить элемент 10
    EXPECT_FALSE(tree.search(10)); // Проверка, что элемент 10 не найден
}

// Тест для вставки дубликатов в AVL дерево
TEST(AVLTreeTest, InsertDuplicates) {
    AVLTree tree;
    tree.insert(10); // Вставка элемента 10
    tree.insert(10); // Вставка дубликата элемента 10
    tree.insert(10); // Вставка дубликата элемента 10

    EXPECT_TRUE(tree.search(10)); // Проверка, что элемент 10 найден
    EXPECT_FALSE(tree.search(20)); // Проверка, что элемент 20 не найден
}

// Тест для удаления несуществующего элемента из AVL дерева
TEST(AVLTreeTest, RemoveNonExistent) {
    AVLTree tree;
    tree.insert(10);
    tree.insert(20);

    tree.remove(30); // Попытка удалить несуществующий элемент 30
    EXPECT_TRUE(tree.search(10)); // Проверка, что элемент 10 остался
    EXPECT_TRUE(tree.search(20)); // Проверка, что элемент 20 остался
    EXPECT_FALSE(tree.search(30)); // Проверка, что элемент 30 не найден
}

// Тест для сериализации и десериализации пустого AVL дерева
TEST(AVLTreeTest, SerializeDeserializeEmptyTree) {
    AVLTree tree;
    std::string filename = "empty_tree.dat";
    tree.serialize(filename); // Сериализация пустого дерева в файл

    AVLTree newTree;
    newTree.deserialize(filename); // Десериализация дерева из файла

    EXPECT_FALSE(newTree.search(10)); // Проверка, что элемент 10 не найден
}

// Тест для сериализации и десериализации AVL дерева с одним элементом
TEST(AVLTreeTest, SerializeDeserializeSingleElementTree) {
    AVLTree tree;
    tree.insert(10); // Вставка элемента 10
    std::string filename = "single_element_tree.dat";
    tree.serialize(filename); // Сериализация дерева в файл

    AVLTree newTree;
    newTree.deserialize(filename); // Десериализация дерева из файла

    EXPECT_TRUE(newTree.search(10)); // Проверка, что элемент 10 загружен
    EXPECT_FALSE(newTree.search(20)); // Проверка, что элемент 20 не загружен
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}