SRC_FILE = ./src/*.c
INCLUDE_DIR = -I./include
LINK_LIB = -lm
TARGET_BIN = rgb_trans

all:
	gcc ${SRC_FILE} ${INCLUDE_DIR} ${LINK_LIB} -o ${TARGET_BIN}

clean:
	rm -rf ${TARGET_BIN} test*