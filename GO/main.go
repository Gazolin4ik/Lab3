package main

import (
	"GO/hashtable"
	"fmt"
	"log"
)

func main() {
	// Создаем хеш-таблицу
	hashTable := hashtable.NewHashTable(10)

	// Добавляем элементы
	err := hashTable.Push("name", "John Doe")
	if err != nil {
		log.Fatalf("Ошибка при добавлении элемента: %v", err)
	}
	err = hashTable.Push("age", "30")
	if err != nil {
		log.Fatalf("Ошибка при добавлении элемента: %v", err)
	}
	err = hashTable.Push("city", "New York")
	if err != nil {
		log.Fatalf("Ошибка при добавлении элемента: %v", err)
	}

	// Получаем значение по ключу
	value, err := hashTable.Get("name")
	if err != nil {
		log.Fatalf("Ошибка при получении значения: %v", err)
	}
	fmt.Println("Найдено:", value)

	// Удаляем элемент
	err = hashTable.Delete("age")
	if err != nil {
		log.Fatalf("Ошибка при удалении элемента: %v", err)
	}

	// Сохраняем в текстовый файл
	err = hashTable.SaveToFile("hashtable.txt")
	if err != nil {
		log.Fatalf("Ошибка при сохранении в файл: %v", err)
	}

	// Загружаем из текстового файла
	err = hashTable.LoadFromFile("hashtable.txt")
	if err != nil {
		log.Fatalf("Ошибка при загрузке из файла: %v", err)
	}

	// Сохраняем в бинарный файл
	err = hashTable.SaveToBinaryFile("hashtable.bin")
	if err != nil {
		log.Fatalf("Ошибка при сохранении в бинарный файл: %v", err)
	}

	// Загружаем из бинарного файла
	err = hashTable.LoadFromBinaryFile("hashtable.bin")
	if err != nil {
		log.Fatalf("Ошибка при загрузке из бинарного файла: %v", err)
	}

	// Печатаем все содержимое хеш-таблицы
	hashTable.Print()
}
