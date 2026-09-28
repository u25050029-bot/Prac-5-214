CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic
TARGET = campusguard
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
DEPS = $(OBJS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

TEST_OBJS = $(filter-out main.o,$(OBJS))

test: $(TEST_OBJS) tests/pattern_tests.cpp
	$(CXX) $(CXXFLAGS) -o pattern_tests tests/pattern_tests.cpp $(TEST_OBJS)
	./pattern_tests

run: $(TARGET)
	./$(TARGET)

valgrind: $(TARGET)
	valgrind --leak-check=full ./$(TARGET)

clean:
	rm -f $(OBJS) $(DEPS) $(TARGET) pattern_tests

-include $(DEPS)

.PHONY: all run test valgrind clean
