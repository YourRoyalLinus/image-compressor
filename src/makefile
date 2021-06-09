all:	compression.exe

compression.exe:	compression.o
					g++ -o compression.exe -std=gnu++17 Main.o ImageCompressionEngine.o FileConverter.o ImageCompressor.o -lstdc++fs -L/usr/X11R6/lib -lm -lpthread -lX11
compression.o:	compression.c
				
compression.c:
			g++ -c Main.cpp
			g++ -c ImageCompressionEngine.cpp
			g++ -c FileConverter.cpp
			g++ -c ImageCompressor.cpp
clean:
	rm compression.exe Main.o ImageCompressionEngine.o FileConverter.o ImageCompressor.o