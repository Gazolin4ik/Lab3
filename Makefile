# Устанавливаем компилятор и флаги
CXX = g++
CXXFLAGS = -g -fprofile-arcs -ftest-coverage -I./libs
LDFLAGS = -lgtest -lgtest_main -lgcov -pthread

# Исходные файлы
SRCS = tests/test.cpp
OBJS = $(SRCS:.cpp=.o)

# Имя исполняемого файла
EXEC = test_runner

# Команда для компиляции тестов
$(EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $(EXEC) $(LDFLAGS)

# Команда для запуска тестов
run_tests: $(EXEC)
	./$(EXEC)

# Команды для генерации отчета о покрытии
coverage: run_tests
	# Собираем статистику покрытия
	lcov --directory . --capture --output-file coverage.info --ignore-errors usage,inconsistent,mismatch
	# Фильтруем системные файлы (например, gtest)
	lcov --remove coverage.info '/usr/*' --output-file coverage_filtered.info
	# Генерируем HTML-отчет
	genhtml coverage_filtered.info --output-directory out

# Очистка
clean:
	rm -f $(OBJS) $(EXEC) coverage.info coverage_filtered.info
	rm -rf out

# Правило для компиляции .cpp файлов в .o файлы
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean run_tests coverage
