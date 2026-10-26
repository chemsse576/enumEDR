CC = x86_64-w64-mingw32-gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -lpsapi
TARGET = enumEDR.exe
SRC = enumEDR.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	del /f $(TARGET) 2>nul

.PHONY: all clean
