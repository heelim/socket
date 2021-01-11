
SRC_PATH = ./src
BIN_PATH = ./bin
LIB_PATH = ./lib
INC_PATH = ./include

OUT_DIR = $(BIN_PATH) $(LIB_PATH)
MKDIR_P = mkdir -p

.PHONY: all
all : common server client test

.PHONY: common
common: directories
	g++ $(SRC_PATH)/common/CommunicationManager.cpp -c -I$(INC_PATH) -o $(LIB_PATH)/CommunicationManager.o;
	ar -cr $(LIB_PATH)/libcom.a $(LIB_PATH)/CommunicationManager.o

.PHONY: server
server:
	g++ $(SRC_PATH)/server/server.cpp -lcom -L$(LIB_PATH) -I$(INC_PATH) -o $(BIN_PATH)/server

.PHONY: client
client:
	g++ $(SRC_PATH)/client/client.cpp -L$(LIB_PATH) -I$(INC_PATH) -o $(BIN_PATH)/client

.PHONY: directories
directories: ${OUT_DIR}

${OUT_DIR}:
	${MKDIR_P} ${OUT_DIR}

.PHONY: test
test: 
	${MKDIR_P} test;
	${MKDIR_P} test/server test/client1 test/client2
	cp $(BIN_PATH)/server test/server/
	cp $(BIN_PATH)/client test/client1/
	cp $(BIN_PATH)/client test/client2/
	cp $(SRC_PATH)/client/make test/client1/make.1
	cp $(SRC_PATH)/client/make test/client2/make.2

.PHONY: clean
clean :
	rm -rf $(OUT_DIR);
	rm -rf test;

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
