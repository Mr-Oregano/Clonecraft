CXX       := g++
CXX_FLAGS := -std=c++17 -ggdb

BIN     := bin
SRC     := src
INCLUDE := -Iinclude -Ivendor/glm -Ivendor/glad -Ivendor/stb_image
VENDOR := vendor

LIBRARIES   := -lOpenGL32 -lGdi32
EXECUTABLE  := Minecraft.exe

all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp $(VENDOR)/glad/*.c $(VENDOR)/stb_image/*.c
	$(CXX) $(CXX_FLAGS) $(INCLUDE) $^ -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/*