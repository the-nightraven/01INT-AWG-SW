##################################################################################
#
# GNU Makefile for Animal War game
#
# @author: Tanasescu Darius for game engine and code, Ivan Corina for concept 
#          art, visuals and mechanics
#
# @desc: build rules for cpp targets including flags for compiling and linking
#
##################################################################################

BUILD_DIR = build/
LIBS_DIR = libs/
SRC_DIR = src/

BIN_NAME = game
CXX = g++
FILES = $(BUILD_DIR)main.o \
		$(BUILD_DIR)logger.o \
		$(BUILD_DIR)app.o \
		$(BUILD_DIR)window.o \
		$(BUILD_DIR)reader.o \
		$(BUILD_DIR)updater.o \
		$(BUILD_DIR)player.o \
		$(BUILD_DIR)renderer.o \
		$(BUILD_DIR)engine_monitor.o \
		$(BUILD_DIR)debugger.o

CXXFLAGS = -w -lSDL2

INC = -Ilibs/ \
      -Isrc/

clean:
	rm -rf build

all: dir $(FILES)
	$(CXX) $(CXXFLAGS) $(INC) ./$(FILES) -o ./$(BUILD_DIR)$(BIN_NAME)
	chmod +x ./$(BUILD_DIR)$(BIN_NAME)
	rm -f $(FILES)
	./$(BUILD_DIR)$(BIN_NAME)


dir:
	mkdir $(BUILD_DIR)

$(BUILD_DIR)main.o: main.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c main.cpp -o $(BUILD_DIR)main.o

$(BUILD_DIR)logger.o: $(LIBS_DIR)logger/logger.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c $(LIBS_DIR)logger/logger.cpp -o $(BUILD_DIR)logger.o

$(BUILD_DIR)app.o: $(SRC_DIR)app/app.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c $(SRC_DIR)app/app.cpp -o $(BUILD_DIR)app.o

$(BUILD_DIR)window.o: $(LIBS_DIR)window/window.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c $(LIBS_DIR)window/window.cpp -o $(BUILD_DIR)window.o

$(BUILD_DIR)reader.o: $(LIBS_DIR)event_reader/reader.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c $(LIBS_DIR)event_reader/reader.cpp -o $(BUILD_DIR)reader.o

$(BUILD_DIR)updater.o: $(LIBS_DIR)updater/updater.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c $(LIBS_DIR)updater/updater.cpp -o $(BUILD_DIR)updater.o

$(BUILD_DIR)player.o: $(SRC_DIR)player/player.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c $(SRC_DIR)player/player.cpp -o $(BUILD_DIR)player.o

$(BUILD_DIR)renderer.o: $(LIBS_DIR)renderer/renderer.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c $(LIBS_DIR)renderer/renderer.cpp -o $(BUILD_DIR)renderer.o

$(BUILD_DIR)engine_monitor.o: $(LIBS_DIR)monitor/engine_monitor.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c $(LIBS_DIR)monitor/engine_monitor.cpp -o $(BUILD_DIR)engine_monitor.o

$(BUILD_DIR)debugger.o: $(LIBS_DIR)debug/debugger.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c $(LIBS_DIR)debug/debugger.cpp -o $(BUILD_DIR)debugger.o