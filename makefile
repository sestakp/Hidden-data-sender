CXX      := -c++
CXXFLAGS := -Wall -Wextra -Wpedantic -Wconversion
LDFLAGS  := -L/usr/lib -lstdc++ -lm -lcrypto -lssl -lpcap
BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
APP_DIR  := .
TARGET   := secret
INCLUDE  := -Ilibraries/
LOGIN = xsesta07
SRC      :=                      \
   $(wildcard src/libraries/*.cpp) \
   $(wildcard src/*.cpp)         \

SILENT := @

OBJECTS  := $(SRC:%.cpp=$(OBJ_DIR)/%.o)
DEPENDENCIES := $(OBJECTS:.o=.d)

all: build $(APP_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -MMD -o $@

$(APP_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $(APP_DIR)/$(TARGET) $^ $(LDFLAGS)

-include $(DEPENDENCIES)

.PHONY: all build clean debug release info

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

debug: CXXFLAGS += -D DEBUG=3 -g
debug: clean all

release: CXXFLAGS += -O2
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/$(TARGET)

info:
	@echo "[*] Application dir: ${APP_DIR}     "
	@echo "[*] Object dir:      ${OBJ_DIR}     "
	@echo "[*] Sources:         ${SRC}         "
	@echo "[*] Objects:         ${OBJECTS}     "
	@echo "[*] Dependencies:    ${DEPENDENCIES}"

submit: clean
	tar -czvf $(LOGIN).tar makefile secret.1 manual.pdf src

test:
	./secret -r sample.pdf -s localhost

listen:
	./secret -l

send_to_merlin:
	scp -r . $(LOGIN)@merlin.fit.vutbr.cz:~/ISA/Project


show_doc:
	man ./secret.1
