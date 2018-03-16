EXE = viewer

SRC_DIR = src
OBJ_DIR = obj
LIB_DIR = lib
INCLUDE = ./inc
DIST = ./bin

SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
LIB = $(wildcard $(LIB_DIR)/*.a)
LIB_LINKS = $(LIB:$(LIB_DIR)/lib%.a=-l%)

CPPFLAGS +=-I${INCLUDE} # Preprocessor flags
CFLAGS += -Wall         # Compiler flags
LDFLAGS += -Llib        # Linker flags
LDLIBS += $(LIB_LINKS) -lGL -lGLEW -lSDL2

CC=g++

.PHONY: all clean prepare

all: $(DIST)/$(EXE) prepare

$(DIST)/$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ) $(DIST)/$(EXE)

prepare:
	[ -d $(OBJ_DIR) ] || mkdir $(OBJ_DIR)



