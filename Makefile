CC = clang
CFLAGS = -I$(LIB_DIR)/quickjs -I$(INCLUDE_DIR)

LIB_DIR = ./lib
JS_DIST_DIR = ./psvmjs/dist
SRC_DIR = ./src
INCLUDE_DIR =  ./include
DIST_DIR = ./dist

.phony: all clean clean-all

all: psvm

psvm: psvm-js.h | $(DIST_DIR)
	$(CC) $(CFLAGS) -o $(DIST_DIR)/psvm  $(SRC_DIR)/psvm.c $(LIB_DIR)/quickjs/libquickjs.a
 
psvm-js.h: quickjs | $(INCLUDE_DIR)
	./$(LIB_DIR)/quickjs/qjsc -c -o $(INCLUDE_DIR)/psvm-js.h $(JS_DIST_DIR)/globalize.js

quickjs:
	$(MAKE) -C $(LIB_DIR)/quickjs

$(DIST_DIR):
	mkdir -p $@

clean-all: clean
	$(MAKE) -C $(LIB_DIR)/quickjs clean

clean:
	rm -rf dist $(INCLUDE_DIR)/psvm-js.h
