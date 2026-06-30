CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Core headers
HEADERS = Actuacion.h Anotador.h Jugador.h CatchBot.hpp JugadorAleatorio.h JugadorHumano.h Marcador.h Tournament.h

all: cacho

cacho: main.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) main.cpp -o cacho

clean:
	rm -f cacho
