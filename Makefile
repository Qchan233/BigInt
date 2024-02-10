# 定义编译器
CXX=g++
# 定义编译选项，-std=c++11是因为我们可能会使用C++11的特性
CXXFLAGS=-std=c++11 -Wall

# 定义目标文件
TARGET=BigIntTest
HEADERS=*.h

# 默认目标
all: $(TARGET)

# 编译目标
$(TARGET): BigInt.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) BigInt.cpp -o $(TARGET)

# 运行测试
test: $(TARGET)
	./$(TARGET)

# 清理编译生成的文件
clean:
	rm -f $(TARGET)

.PHONY: all test clean
