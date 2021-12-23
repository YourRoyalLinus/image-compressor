SOURCES=$(shell find ./src -name *.cpp)
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
CEREAL_DIR := $(ROOT_DIR)Cereal-1.3.0
CIMG_DIR := $(ROOT_DIR)CImg-2.9.9

IM_TEST_LOG := $(IMAGE_MAGIC_DIR)test-suite.log

dependencies:	fetch-cimg fetch-imagemagick configure-imagemagick validate-imagemagick fetch-cereal 

fetch-cimg:
	@echo Downloading CImg-2.9.9...
	$(warning Downloading requires sudo permissions)
	@mkdir -p $(CIMG_DIR)
	@sudo wget --directory-prefix=$(CIMG_DIR) -q https://github.com/dtschump/CImg/archive/refs/tags/v.2.9.9.tar.gz
	@tar xzf $(CIMG_DIR)/v.2.9.9.tar.gz -C $(CIMG_DIR) --strip-components 1
	@echo Download Complete!

fetch-imagemagick: install-imagemagick-packages
	@echo Downloading ImageMagick...
	$(warning Downloading requires sudo permissions)
	@mkdir -p $(IMAGE_MAGIC_DIR)
	@sudo wget --directory-prefix=$(IMAGE_MAGIC_DIR) -q https://www.imagemagick.org/download/ImageMagick.tar.gz
	@tar xzf $(IMAGE_MAGIC_DIR)/ImageMagick.tar.gz -C $(IMAGE_MAGIC_DIR) --strip-components 1
	@echo Download Complete!

install-imagemagick-packages:
	@echo Installing Dependent Packages... 
	$(warning Installing dependencies requires sudo permissions)
 
	@if ! dpkg -l | grep build-essential -c -q; then sudo apt-get -y install > /dev/null build-essential ; fi 
	@if ! dpkg -l | grep libtiff-dev -c -q; then sudo apt-get -y install > /dev/null libtiff-dev; fi 	
	@if ! dpkg -l | grep libx11-dev -c -q; then sudo apt-get -y install > /dev/null libx11-dev; fi

	@echo Installation Complete!

configure-imagemagick:
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
	@echo Test results: $(IM_TEST_LOG)
	@echo Testing Complete!

fetch-cereal:
	@echo Downloading Cereal-1.3.0...
	$(warning Downloading requires sudo permissions)
	@mkdir -p $(CEREAL_DIR)
	@sudo wget --directory-prefix=$(CEREAL_DIR) -q https://github.com/USCiLab/cereal/archive/v1.3.0.tar.gz
	@tar xzf $(CEREAL_DIR)/v1.3.0.tar.gz -C $(CEREAL_DIR) --strip-components 1
	@echo Download Complete!

remove-dependencies:	remove-cimg remove-imagemagick remove-cereal
	@sudo rm -r $(ROOT_DIR)
	
remove-cimg:
	$(warning Uninstalling CImg requires sudo permissions)
	@sudo rm -r $(CIMG_DIR)

remove-imagemagick:
	$(warning Uninstalling ImageMagick requires sudo permissions)
	@sudo rm -r $(IMAGE_MAGIC_DIR)

remove-cereal:
	$(warning Uninstalling Cereal requires sudo permissions)
	@sudo rm -r $(CEREAL_DIR)
	