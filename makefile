SOURCES=$(wildcard src/*.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))
LIBS=-lstdc++fs -L/usr/X11R6/lib -lm -lpthread -lX11
CFLAGS= -std=gnu++17
CC=g++
OUTPUT_BIN=/build
EXECUTABLE=build/compress.exe

all:	build $(OUTPUT_BIN)

$(OUTPUT_BIN):  $(OBJECTS)
	$(CC) $(CFLAGS) $(addprefix build/, $(notdir $(OBJECTS))) $(LIBS) -o $(EXECUTABLE)

$(OBJECTS): src/%.o : src/%.cpp
	$(CC) $(CFLAGS) -c $< $(LIBS) -o build/$(notdir $@)

build:
	@mkdir -p build
	
clean:
	rm -rf $(addprefix build/, $(notdir $(OBJECTS))) $(EXECUTABLE)
	rm -r build/

ROOT_DIR=lib/
IMAGE_MAGIC_DIR := $(ROOT_DIR)ImageMagick-Latest/
TEST_LOG := $(IMAGE_MAGIC_DIR)test-suite.log

dependencies:	install-imagemagick	validate-imagemagick

install-imagemagick:	libs
	@echo Configuring ImageMagick...
	$(warning Configuring requires sudo permissions)
	@cd $(IMAGE_MAGIC_DIR) && ./configure --disable-shared --silent
	@echo Configuration Complete!

	$(warning Installing requires sudo permissions)
	@echo Installing ImageMagick...
	@sudo make -C $(IMAGE_MAGIC_DIR) > /dev/null 
	@sudo make install -C $(IMAGE_MAGIC_DIR) > /dev/null 
	@sudo rm $(IMAGE_MAGIC_DIR)ImageMagick.tar.gz
	@echo Installation Complete!

validate-imagemagick:
	@echo Running Test Suite...
	$(warning Testing requires sudo permissions)
	@sudo make check -C $(IMAGE_MAGIC_DIR) > /dev/null 
	@echo Test results: $(TEST_LOG)
	@echo Testing Complete!

remove-imagemagick:
	$(warning Uninstalling ImageMagick requires sudo permissions)
	@sudo rm -r $(IMAGE_MAGIC_DIR)
	@sudo rm -r lib/

libs:	packages
	@echo Downloading ImageMagick...
	$(warning Downloading requires sudo permissions)
	@mkdir -p $(IMAGE_MAGIC_DIR)
	@sudo wget --directory-prefix=$(IMAGE_MAGIC_DIR) -q https://www.imagemagick.org/download/ImageMagick.tar.gz
	@tar xzf $(IMAGE_MAGIC_DIR)/ImageMagick.tar.gz -C $(IMAGE_MAGIC_DIR) --strip-components 1
	@echo Download Complete!

packages:
	@echo Installing Dependent Packages... 
	$(warning Installing dependencies requires sudo permissions)
 
	@if ! dpkg -l | grep build-essential -c -q; then sudo apt-get -y install > /dev/null build-essential ; fi 
	@if ! dpkg -l | grep libjpeg62-dev -c -q; then sudo apt-get -y install > /dev/null libjpeg62-dev; fi 
	@if ! dpkg -l | grep libpng-dev -c -q; then sudo apt-get -y install > /dev/null libpng-dev; fi 
	@if ! dpkg -l | grep libtiff-dev -c -q; then sudo apt-get -y install > /dev/null libtiff-dev; fi 	

	@echo Installation Complete!
