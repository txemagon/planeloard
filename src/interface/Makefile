OUTPUT = libinterface.a

DEST = ../..

SRC_DIR = .
OBJ_DIR = obj
INC_DIR = inc
LIB_DIR = lib

SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

CPPFLAGS += -I.
CFLAGS += -Wall
LDFLAGS += -L$(LIB_DIR)
LDLIBS += -lGL -lGLEW -lSDL2

CC = g++

all: prepare $(OUTPUT)

$(OUTPUT): $(OBJ)
	ar cr $(OUTPUT) $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: prepare clean dist

prepare:
	[ -d $(OBJ_DIR) ] || mkdir $(OBJ_DIR)

clean:
	$(RM) $(OBJ_DIR)/* $(OUTPUT)

dist:
	cp $(OUTPUT) $(DEST)/$(LIB_DIR)
	cp *.h $(DEST)/$(INC_DIR)
