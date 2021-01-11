
SRC_PATH = ./src
BIN_PATH = ./bin
LIB_PATH = ./lib
OBJ_PATH = ./include

OUT_DIR = $(BIN_PATH) $(LIB_PATH)

all : common server client

MKDIR_P = mkdir -p

.PHONY: directories

directories: ${OUT_DIR}

${OUT_DIR}:
	${MKDIR_P} ${OUT_DIR}

# common : dir
# 	cd common;
# 	g++ net_manager.cpp -c -I. -o $(LIB_PATH)/net_manager.o

# server : common
# 	g++
	
# CC = g++


# all : common server client

# obj : 
# 	g++ $(SRC_PATH)/common/ -o ;


# OBJECTS := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
# $(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
#     @$(CC) $(CFLAGS) -c $< -o $@
#     @echo "Compiled "$<" successfully!"

# clean:
# 	rm -rf $(BIN_PATH) $(LIB_PATH);


# # # project name (generate executable with this name)
# # TARGET   = projectname

# # CC       = gcc
# # # compiling flags here
# # CFLAGS   = -std=c99 -Wall -I.

# # LINKER   = gcc
# # # linking flags here
# # LFLAGS   = -Wall -I. -lm

# # # change these to proper directories where each file should be
# # SRCDIR   = src
# # OBJDIR   = obj
# # BINDIR   = bin

# # SOURCES  := $(wildcard $(SRCDIR)/*.c)
# # INCLUDES := $(wildcard $(SRCDIR)/*.h)
# # OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
# # rm       = rm -f

# # $(BINDIR)/$(TARGET): $(OBJECTS)
# #     @$(LINKER) $(OBJECTS) $(LFLAGS) -o $@
# #     @echo "Linking complete!"

# # $(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
# #     @$(CC) $(CFLAGS) -c $< -o $@
# #     @echo "Compiled "$<" successfully!"

# # .PHONY: clean
# # clean:
# #     @$(rm) $(OBJECTS)
# #     @echo "Cleanup complete!"

# # .PHONY: remove
# # remove: clean
# #     @$(rm) $(BINDIR)/$(TARGET)
# #     @echo "Executable removed!"
