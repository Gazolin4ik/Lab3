package hashtable

import (
	"bufio"
	"encoding/binary"
	"errors"
	"fmt"
	"os"
	"strings"
)

type KeyValuePair struct {
	Key   string
	Value string
	Next  *KeyValuePair
}

type HashTable struct {
	table     []*KeyValuePair
	tableSize int
}

func NewHashTable(initialCapacity int) *HashTable {
	return &HashTable{
		table:     make([]*KeyValuePair, initialCapacity),
		tableSize: initialCapacity,
	}
}

func (ht *HashTable) hashFunction(key string) int {
	hash := 0
	for _, c := range key {
		hash = hash*31 + int(c)
	}
	return hash % ht.tableSize
}

func (ht *HashTable) Push(key, value string) error {
	hash := ht.hashFunction(key)
	current := ht.table[hash]
	for current != nil {
		if current.Key == key {
			return errors.New("key already exists")
		}
		current = current.Next
	}
	newPair := &KeyValuePair{Key: key, Value: value, Next: ht.table[hash]}
	ht.table[hash] = newPair
	return nil
}

func (ht *HashTable) Get(key string) (string, error) {
	hash := ht.hashFunction(key)
	current := ht.table[hash]
	for current != nil {
		if current.Key == key {
			return current.Value, nil
		}
		current = current.Next
	}
	return "", errors.New("key not found")
}

func (ht *HashTable) Delete(key string) error {
	hash := ht.hashFunction(key)
	current := ht.table[hash]
	var prev *KeyValuePair
	for current != nil {
		if current.Key == key {
			if prev == nil {
				ht.table[hash] = current.Next
			} else {
				prev.Next = current.Next
			}
			return nil
		}
		prev = current
		current = current.Next
	}
	return errors.New("key not found")
}

func (ht *HashTable) SaveToFile(filename string) error {
	file, _ := os.Create(filename)
	defer file.Close()

	writer := bufio.NewWriter(file)
	for _, bucket := range ht.table {
		current := bucket
		for current != nil {
			_, _ = writer.WriteString(current.Key + ";" + current.Value + "\n")
			current = current.Next
		}
	}
	_ = writer.Flush()
	return nil
}

func (ht *HashTable) LoadFromFile(filename string) error {
	file, _ := os.Open(filename)
	defer file.Close()

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := scanner.Text()
		parts := strings.SplitN(line, ";", 2)
		if len(parts) == 2 {
			_ = ht.Push(parts[0], parts[1])
		}
	}
	return scanner.Err()
}

func (ht *HashTable) SaveToBinaryFile(filename string) error {
	file, _ := os.Create(filename)
	defer file.Close()

	for _, bucket := range ht.table {
		current := bucket
		for current != nil {
			keyLen := int32(len(current.Key))
			valueLen := int32(len(current.Value))
			_ = binary.Write(file, binary.LittleEndian, keyLen)
			_, _ = file.Write([]byte(current.Key))
			_ = binary.Write(file, binary.LittleEndian, valueLen)
			_, _ = file.Write([]byte(current.Value))
			current = current.Next
		}
	}
	return nil
}

func (ht *HashTable) LoadFromBinaryFile(filename string) error {
	file, _ := os.Open(filename)
	defer file.Close()

	for {
		var keyLen int32
		err := binary.Read(file, binary.LittleEndian, &keyLen)
		if err != nil {
			break
		}

		key := make([]byte, keyLen)
		_, _ = file.Read(key)

		var valueLen int32
		err = binary.Read(file, binary.LittleEndian, &valueLen)
		if err != nil {
			break
		}

		value := make([]byte, valueLen)
		_, _ = file.Read(value)

		_ = ht.Push(string(key), string(value))
	}

	return nil
}

func (ht *HashTable) Print() error {
	for i, bucket := range ht.table {
		if bucket != nil {
			fmt.Printf("Bucket %d:\n", i)
			current := bucket
			for current != nil {
				fmt.Printf("  Key: %s, Value: %s\n", current.Key, current.Value)
				current = current.Next
			}
		}
	}
	return nil
}

func (ht *HashTable) TableSize() int {
	return ht.tableSize
}
