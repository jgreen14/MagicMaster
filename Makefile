all: remomsdl

remomsdl: remomsdl.o renderer.o unitpos.o engine.o unit.o
	g++ -g -o remomsdl remomsdl.o renderer.o unitpos.o engine.o unit.o `sdl-config --libs` -lSDL_image -lSDL_gfx -lSDL_ttf libsqlite3.a -lpthread -ldl

remomsdl.o: remomsdl.cpp
	g++ -c remomsdl.cpp 

renderer.o: renderer.cpp
	g++ -c renderer.cpp 

unitpos.o: unitpos.cpp
	g++ -c unitpos.cpp

engine.o: engine.cpp
	g++ -c engine.cpp

unit.o: unit.cpp
	g++ -c unit.cpp

clean:
	rm -f remomsdl remomsdl.o renderer.o unitpos.o engine.o unit.o
