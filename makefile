# Компилятор и флаги
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I. -g

# Главный исполняемый файл
EXEC = main

# Исходные файлы
SRC = main.cpp

# Заголовочные зависимости (не компилируются напрямую, но важны для перезапуска)
HEADERS = RectangularMatrix.h ArraySequence.h Sequence.h DynamicArray.h Student.h Teacher.h

# Цель по умолчанию
all: $(EXEC)

# Правило сборки основного исполняемого файла
$(EXEC): $(SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(EXEC)

# Очистка
clean:
	rm -f $(EXEC)

.PHONY: all clean
