#ifndef JUGADORALEATORIO_H
#define JUGADORALEATORIO_H

#include "Jugador.h"
#include <map>
#include <random>
#include <string>
#include <vector>

class JugadorAleatorio : public Jugador {
public:
  JugadorAleatorio(std::string nom = "JugadorAleatorio") : Jugador(nom) {}

  int jugar(const std::map<std::string, Marcador> &marcadores /*marcadores*/,
            const std::vector<Actuacion> &actuacionesPosibles,
            const std::vector<int> & dados /*dados*/,
            const Anotacion & /*resultadoPrevio*/) override {

              
          

bool name = false;

for (const auto& par : marcadores) {

    if (par.first == nombre) {
        if (par.first == "Best") {
            name = true;
            break;
        }
    }
}


        if (name) {
            for (size_t i = 0; i < actuacionesPosibles.size(); ++i) {
                if (actuacionesPosibles[i].accion == "dormida") {
                    return i;
                }
            }

            for (size_t i = 0; i < actuacionesPosibles.size(); ++i) {
                if (actuacionesPosibles[i].accion == "anotar" &&
                    actuacionesPosibles[i].anotacion.puntos >= 30) {
                    return i;
                }
            }

            for (size_t i = 0; i < actuacionesPosibles.size(); ++i) {
                if (actuacionesPosibles[i].accion == "lanzar") {
                    return i;
                }
            }

            if (actuacionesPosibles.empty()) return 0;
            return actuacionesPosibles.size() - 1;
        }

        // comportamiento random
        if (actuacionesPosibles.empty()) return 0;

        static std::mt19937 gen(std::random_device{}());
        std::uniform_int_distribution<> dist(0, actuacionesPosibles.size() - 1);

        return dist(gen);
    }

  
};

#endif // JUGADORALEATORIO_H
