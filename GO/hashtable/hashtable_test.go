package hashtable_test

import (
	"GO/hashtable"
	"encoding/binary"
	"os"
	"testing"
)

// Тест на создание хеш-таблицы
func TestCreateHashTable(t *testing.T) {
	hashTable := hashtable.NewHashTable(10)
	if hashTable == nil {
		t.Fatal("Не удалось создать хеш-таблицу")
	}
	if hashTable.TableSize() != 10 {
		t.Fatalf("Ожидаемый размер таблицы: 10, получено: %d", hashTable.TableSize())
	}
}

// Тест на добавление существующего ключа
func TestPushExistingKey(t *testing.T) {
	hashTable := hashtable.NewHashTable(10)
	err := hashTable.Push("key1", "value1")
	if err != nil {
		t.Fatalf("Не удалось вставить key1: %v", err)
	}
	err = hashTable.Push("key1", "value2")
	if err == nil {
		t.Fatal("Ожидалась ошибка при вставке существующего ключа")
	}
	if err.Error() != "key already exists" {
		t.Fatalf("Ожидалась ошибка 'key already exists', получено: %v", err)
	}
}

// Тест на успешное добавление ключа
func TestPushSuccess(t *testing.T) {
	hashTable := hashtable.NewHashTable(10)
	err := hashTable.Push("key1", "value1")
	if err != nil {
		t.Fatalf("Не удалось вставить key1: %v", err)
	}

	value, err := hashTable.Get("key1")
	if err != nil {
		t.Fatalf("Не удалось получить key1: %v", err)
	}
	if value != "value1" {
		t.Fatalf("Ожидаемое значение 'value1', получено: %v", value)
	}
}

// Тест на поиск несуществующего ключа
func TestGetKeyNotFound(t *testing.T) {
	hashTable := hashtable.NewHashTable(10)
	_, err := hashTable.Get("key1")
	if err == nil {
		t.Fatal("Ожидалась ошибка при поиске несуществующего ключа")
	}
	if err.Error() != "key not found" {
		t.Fatalf("Ожидалась ошибка 'key not found', получено: %v", err)
	}
}

// Тест на поиск существующего ключа
func TestGetKeyFound(t *testing.T) {
	hashTable := hashtable.NewHashTable(10)
	hashTable.Push("key1", "value1")
	value, err := hashTable.Get("key1")
	if err != nil {
		t.Fatalf("Не удалось получить key1: %v", err)
	}
	if value != "value1" {
		t.Fatalf("Ожидаемое значение 'value1', получено: %v", value)
	}
}

// Тест на успешное удаление ключа
func TestDeleteKeySuccess(t *testing.T) {
	hashTable := hashtable.NewHashTable(10)
	hashTable.Push("key1", "value1")
	err := hashTable.Delete("key1")
	if err != nil {
		t.Fatalf("Не удалось удалить key1: %v", err)
	}

	_, err = hashTable.Get("key1")
	if err == nil {
		t.Fatal("Ожидалась ошибка при поиске удалённого ключа")
	}
	if err.Error() != "key not found" {
		t.Fatalf("Ожидалась ошибка 'key not found', получено: %v", err)
	}
}

// Тест на удаление несуществующего ключа
func TestDeleteKeyNotFound(t *testing.T) {
	hashTable := hashtable.NewHashTable(10)
	err := hashTable.Delete("nonexistent")
	if err == nil {
		t.Fatal("Ожидалась ошибка при удалении несуществующего ключа")
	}
	if err.Error() != "key not found" {
		t.Fatalf("Ожидалась ошибка 'key not found', получено: %v", err)
	}
}

// Тест на сохранение и загрузку хеш-таблицы из файла
func TestSaveAndLoadFromFile(t *testing.T) {
	hashTable := hashtable.NewHashTable(10)
	hashTable.Push("key1", "value1")
	hashTable.Push("key2", "value2")

	err := hashTable.SaveToFile("test.txt")
	if err != nil {
		t.Fatalf("Не удалось сохранить в файл: %v", err)
	}

	loadedTable := hashtable.NewHashTable(10)
	err = loadedTable.LoadFromFile("test.txt")
	if err != nil {
		t.Fatalf("Не удалось загрузить из файла: %v", err)
	}

	value, err := loadedTable.Get("key1")
	if err != nil || value != "value1" {
		t.Fatalf("Не удалось получить key1 после загрузки из файла: %v", err)
	}

	value, err = loadedTable.Get("key2")
	if err != nil || value != "value2" {
		t.Fatalf("Не удалось получить key2 после загрузки из файла: %v", err)
	}

	// Очистка
	err = os.Remove("test.txt")
	if err != nil {
		t.Fatalf("Не удалось удалить тестовый файл: %v", err)
	}
}

// Тест на сохранение хеш-таблицы в бинарный файл
func TestSaveToBinaryFile(t *testing.T) {
	hashTable := hashtable.NewHashTable(10)
	hashTable.Push("name", "John Doe")

	err := hashTable.SaveToBinaryFile("test_data.bin")
	if err != nil {
		t.Fatalf("Не удалось сохранить в бинарный файл: %v", err)
	}

	// Проверка, что бинарный файл был создан и содержит данные
	file, err := os.Open("test_data.bin")
	if err != nil {
		t.Fatalf("Не удалось открыть бинарный файл: %v", err)
	}
	defer file.Close()

	// Чтение и проверка содержимого
	var keyLen, valueLen int32
	var key []byte
	var value []byte

	// Чтение длин ключа и значения
	err = binary.Read(file, binary.LittleEndian, &keyLen)
	if err != nil {
		t.Fatalf("Не удалось прочитать длину ключа из бинарного файла: %v", err)
	}

	key = make([]byte, keyLen)
	_, err = file.Read(key)
	if err != nil {
		t.Fatalf("Не удалось прочитать ключ из бинарного файла: %v", err)
	}

	err = binary.Read(file, binary.LittleEndian, &valueLen)
	if err != nil {
		t.Fatalf("Не удалось прочитать длину значения из бинарного файла: %v", err)
	}

	value = make([]byte, valueLen)
	_, err = file.Read(value)
	if err != nil {
		t.Fatalf("Не удалось прочитать значение из бинарного файла: %v", err)
	}

	// Проверка содержимого
	if string(key) != "name" || string(value) != "John Doe" {
		t.Fatalf("Некорректное содержимое бинарного файла: ключ = %s, значение = %s", key, value)
	}

	// Очистка
	err = os.Remove("test_data.bin")
	if err != nil {
		t.Fatalf("Не удалось удалить бинарный файл: %v", err)
	}
}

// Тест на загрузку хеш-таблицы из бинарного файла
func TestLoadFromBinaryFile(t *testing.T) {
	hashTable := hashtable.NewHashTable(10)
	hashTable.Push("name", "John Doe")

	err := hashTable.SaveToBinaryFile("test_data.bin")
	if err != nil {
		t.Fatalf("Не удалось сохранить в бинарный файл: %v", err)
	}

	loadedTable := hashtable.NewHashTable(10)
	err = loadedTable.LoadFromBinaryFile("test_data.bin")
	if err != nil {
		t.Fatalf("Не удалось загрузить из бинарного файла: %v", err)
	}

	// Проверка, что данные загружены корректно
	value, err := loadedTable.Get("name")
	if err != nil || value != "John Doe" {
		t.Fatalf("Не удалось получить ключ после загрузки из бинарного файла: %v", err)
	}

	// Очистка
	err = os.Remove("test_data.bin")
	if err != nil {
		t.Fatalf("Не удалось удалить бинарный файл: %v", err)
	}
}

// Тест на вывод хеш-таблицы
func TestPrint(t *testing.T) {
	hashTable := hashtable.NewHashTable(10)
	err := hashTable.Push("key1", "value1")
	if err != nil {
		t.Fatalf("Не удалось вставить key1: %v", err)
	}

	err = hashTable.Print()
	if err != nil {
		t.Fatalf("Не удалось вывести хеш-таблицу: %v", err)
	}
}
