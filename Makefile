# RetroWave-4M Graphics Library Makefile

CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11 -g
CFLAGS += -Iinclude
LDFLAGS = -lm -lrt

# Sorgenti
SOURCES = src/retrowave_graphics.c src/rw_text.c src/rw_background.c src/rw_2d.c src/rw_math.c
OBJECTS = $(SOURCES:.c=.o)

# Header
HEADERS = include/retrowave_graphics.h

# Target
TARGET = libretrowave.a

# Test
TEST_SOURCES = tests/test_graphics.c
TEST_TARGET = test_graphics

# ============================================
# Regole
# ============================================

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(OBJECTS)
	ar rcs $@ $^
	@echo "Built: $@"

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_SOURCES) $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJECTS) $(TARGET) $(TEST_TARGET)
	@echo "Clean complete"

# ============================================
# Debug
# ============================================

debug:
	$(CC) $(CFLAGS) -DDEBUG -g $(SOURCES) -o debug_binary $(LDFLAGS)

# ============================================
# Install (per sistema host)
# ============================================

PREFIX ?= /usr/local
INSTALL_HEADERS = $(PREFIX)/include
INSTALL_LIB = $(PREFIX)/lib

install:
	install -d $(INSTALL_HEADERS)
	install -m 644 $(HEADERS) $(INSTALL_HEADERS)/
	install -d $(INSTALL_LIB)
	install -m 644 $(TARGET) $(INSTALL_LIB)/
	@echo "Installed to $(PREFIX)"

uninstall:
	rm -f $(INSTALL_HEADERS)/retrowave_graphics.h
	rm -f $(INSTALL_LIB)/$(TARGET)
	@echo "Uninstalled"
