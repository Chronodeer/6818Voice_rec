# 可执行文件
TARGET := test

# 编译器
CC := arm-linux-gcc

# 目录
BIN := ./bin

# 编译参数
CFLAGS := -I ./inc
LDFLAGS := -lpthread

# 源文件
SRC := $(wildcard ./src/*.c)

# 目标文件
OBJ := $(patsubst %.c,%.o,$(SRC))

# 默认目标
$(TARGET): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)
	@mkdir -p $(BIN)
	mv $(TARGET) $(BIN)

# 编译规则
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

# 清理
.PHONY: clean
clean:
	@echo "Cleaning..."
	rm -f $(OBJ)
	rm -f $(BIN)/$(TARGET)