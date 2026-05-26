# greedysnake - Terminal Snake Game
# Makefile for MinGW-w64 / G++ on Windows

CXX      = g++
CXXFLAGS = -Wall -Wextra -std=c++20 -Iinclude
LDFLAGS  =
TARGET   = greedysnake.exe
SRCDIR   = src
OBJDIR   = build

SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(TARGET)

run: $(TARGET)
	./$(TARGET)
