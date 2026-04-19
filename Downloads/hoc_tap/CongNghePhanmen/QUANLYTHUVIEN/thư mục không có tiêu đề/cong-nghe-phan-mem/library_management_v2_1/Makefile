CXX      = g++
CXXFLAGS = -std=c++17 -Wall -I./include
TARGET   = library

SRCS = src/main.cpp        \
       src/Utils.cpp       \
       src/Book.cpp        \
       src/BorrowSlip.cpp  \
       src/Fine.cpp        \
       src/User.cpp        \
       src/Reader.cpp      \
       src/Librarian.cpp   \
       src/Admin.cpp       \
       src/FileManager.cpp \
       src/LibrarySystem.cpp \
       src/LibrarySystemMenu.cpp

OBJS = $(SRCS:.cpp=.o)

# ==========================================
# Build targets
# ==========================================

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET)

run: all
	./$(TARGET)

# Windows
win:
	$(CXX) $(CXXFLAGS) -o $(TARGET).exe $(SRCS)

.PHONY: all clean run win
