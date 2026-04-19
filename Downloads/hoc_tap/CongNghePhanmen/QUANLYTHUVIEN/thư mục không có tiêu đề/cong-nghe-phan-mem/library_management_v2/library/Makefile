# Makefile - He thong Quan ly Thu Vien
# Cach dung: make

CXX = g++
CXXFLAGS = -std=c++11 -Wall

TARGET = library

SRCS = main.cpp utils.cpp fileio.cpp auth.cpp book.cpp reader.cpp borrow.cpp fine_report.cpp admin.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)
	@echo "Build thanh cong! Chay: ./library"

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)
