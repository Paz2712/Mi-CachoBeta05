#ifndef CatchBot_hpp
#define CatchBot_hpp

#include "Jugador.h"
#include "Marcador.h"
#include "Actuacion.h"

#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include <random>
#include <string>
#include <vector>
#include <cmath>

class CatchBot : public Jugador
{
private:
  std::string nombreE = "Isidora Garcia Paz Perez, rol:202573029-4, parCatedra:2";

public:
  std::string GetNombreEst() const
  {
    return nombreE;
  }

  CatchBot(std::string nom = "CatchBot") : Jugador(nom) { nombre = nom; }
  int turnoIn = 0;

  // continuidad escalera, es posible crear una escalera o vamos muy mal
  std::vector<bool> continiudad(const std::vector<int> &dados)
  {
    std::vector<int> temp;
    for (const auto &dado : dados)
    {
      temp.push_back(dado);
    }
    sort(temp.begin(), temp.end(), std::greater<int>());
    int inicio = temp.at(0);
    int fin = temp.at(temp.size() - 1);
    int contDer = 0;
    int contIzq = 0;
    for (auto dad : temp)
    {
      if (dad == inicio)
      {
        contDer++;
      }
      inicio--;
    }
    for (int i = temp.size() - 1; 0 <= i; i--)
    {
      if (temp.at(i) == fin)
      {
        contIzq++;
      }
      fin++;
    }
    if (contDer > 3)
    {
      return {true, true};
    } // {1,3,4,5,6} lectura de dados normal
    if (contIzq > 3)
    {
      return {true, false};
    } // {1,2,3,4,6} lectura de dados revez
    return {false, false};
  }

  // posicion mala escalera, SE PUEDE CREAR UNA ESCALERA!! que dados hemos de lanzar
  std::vector<int> incontiniudad(const std::vector<int> &dados, bool lectura)
  {
    int cont = 0;
    std::vector<std::vector<int>> temp;
    for (const auto &dado : dados)
    {
      temp.push_back({dado, cont});
      cont++;
    }
    std::vector<int> pos;
    sort(temp.begin(), temp.end());

    if (lectura)
    { // {1,3,4,5,6} lectura de dados normal <--
      cont = temp.size() - 1;
      std::vector<int> fin = temp.at(cont);
      int start = fin[0];
      for (int i = cont; 0 <= i; i--, start--)
      {
        if ((temp.at(i))[0] != start)
        {
          pos.push_back((temp.at(i))[1]);
        }
      }
    }
    else
    { // {1,2,3,4,6} lectura de dados revez -->
      std::vector<int> inicio = temp.at(0);
      int start = inicio[0];
      for (auto dado : temp)
      {
        if (dado[0] != start)
        {
          pos.push_back(dado[1]);
        }
        start++;
        cont++;
      }
    }
    std::sort(pos.begin(), pos.end());
    return pos;
  }

  // rep de numeros, que numeros se repiten, cuantas veces
  std::map<int, int> iguales(const std::vector<int> &dados)
  {
    std::map<int, int> rep;
    for (int i = 0; i < dados.size(); i++)
    {
      int iguales = 0, piv = i;
      std::map<int, int> cont;
      for (int i = piv; i < dados.size(); i++)
      {
        if (dados[i] == dados[piv])
          iguales++;
        if (i == (dados.size() - 1))
        {
          rep.insert({dados[piv], iguales});
          piv++;
        }
      }
    }
    return rep;
  }

  // que dados he de relanzar
  std::vector<int> Pos(const std::vector<int> &dados, std::map<int, int> map, std::string jugada)
  {

    // jugada de grande1/2
    std::vector<int> Rpos;
    if (jugada == "grande" || jugada == "grande2")
    {
      int max = -1, numMax;
      for (auto dad : map)
      {
        if (dad.second > max)
        {
          max = dad.second;
          numMax = dad.first;
        }
      }
      for (int i = 0; i < dados.size(); i++)
      {
        if (dados[i] != numMax)
        {
          Rpos.push_back(i);
        }
      }
      std::sort(Rpos.begin(), Rpos.end());
      return Rpos;
    }
    // jugada poker
    if (jugada == "poker")
    {
      int max = -1, numMax, numMin = 0;
      for (auto dad : map)
      {
        if (dad.second > max)
        {
          max = dad.second;
          numMax = dad.first;
        }
        if (dad.second == 1)
        {
          numMin = dad.first;
        }
      }

      for (int i = 0; i < dados.size(); i++)
      {
        if (dados[i] != numMax && dados[i] != numMin)
        {
          Rpos.push_back(i);
          if (numMin == 0)
            break;
        }
      }

      std::sort(Rpos.begin(), Rpos.end());
      return Rpos;
    }
    // jugada full
    if (jugada == "full")
    {
      int max = -1, numMax;
      int numMax2;
      for (auto dad : map)
      {
        if (dad.second > max)
        {
          max = dad.second;
          numMax = dad.first;
        }
        if (dad.first != numMax || dad.second < max)
          numMax2 = dad.first;
        if (dad.second == max)
        {
          numMax2 = dad.first;
        }
      }

      for (int i = 0; i < dados.size(); i++)
      {
        if (dados[i] != numMax && dados[i] != numMax2)
        {
          Rpos.push_back(i);
        }
      }
      std::sort(Rpos.begin(), Rpos.end());
      return Rpos;
    }

    return Rpos;
  }

  // jugada de "balas","tontos","trenes", "cuadras", "quinas", "senas"
  std::vector<int> Pos5grandes(const std::vector<int> &dados, int numMax)
  {
    std::vector<int> Rpos;
    for (int i = 0; i < dados.size(); i++)
    {
      if (dados[i] != numMax)
      {
        Rpos.push_back(i);
      }
    }
    std::sort(Rpos.begin(), Rpos.end());
    return Rpos;
  }

  int jugar(const std::map<std::string, Marcador> &marcadores /*marcadores*/,
            const std::vector<Actuacion> &actuacionesPosibles,
            const std::vector<int> &dados /*dados*/,
            const Anotacion &resPrev /*resultadoPrevio*/) override
  {
    // yaAnotado()buscar si jugada ya hecha

    // CatchBot.Actuacion("lanzar", {' ', -1}, {0, 2, 4}, false);

    // actuacionesPosibles[casos].displayCompacto();

    // si debo relanzar dados como se debe ver mi jugada, clase hecha para ser modificada y luego igualada a la jugada ideal
    Actuacion change("lanzar", {" ", -1}, {0, 1, 2, 3, 4}, false);
    // cuanto se repiten los dados esta partida
    std::map<int, int> ddsExp = iguales(dados);
    // hay posivilidad de escalera
    std::vector<bool> escala = continiudad(dados);

    // hay dormida, solo corre si es primer turno
    if (turnoIn < 1)
    {
      for (size_t casos = 0; casos < actuacionesPosibles.size(); casos++)
      {
        if (actuacionesPosibles[casos].accion == "dormida")
        {
          return casos;
        }
      }
      turnoIn++;
    }

    // grande1 y 2
    if (ddsExp.size() < 3)
    {
      for (size_t casos = 0; casos < actuacionesPosibles.size(); casos++)
      {
        if ((actuacionesPosibles[casos].accion == "anotar") && (actuacionesPosibles[casos].anotacion.juego == "grande" || actuacionesPosibles[casos].anotacion.juego == "grande2") && actuacionesPosibles[casos].anotacion.puntos >= 40)
        {
          return casos;
        }
        else
        {
          change.indiceDados = Pos(dados, ddsExp, "grande");
          for (int i = 0; i < actuacionesPosibles.size(); i++)
          {
            if (actuacionesPosibles[i].indiceDados == change.indiceDados && actuacionesPosibles[i].accion == change.accion)
              return i;
          }
        }
      }
    }

    // poker
    if (ddsExp.size() <= 3)
    {
      for (size_t casos = 0; casos < actuacionesPosibles.size(); casos++)
      {
        if ((actuacionesPosibles[casos].accion == "anotar") && (actuacionesPosibles[casos].anotacion.juego == "poker") && actuacionesPosibles[casos].anotacion.puntos >= 40)
        {
          return casos;
        }
        else
        {
          change.indiceDados = Pos(dados, ddsExp, "poker");
          for (int i = 0; i < actuacionesPosibles.size(); i++)
          {
            if (actuacionesPosibles[i].indiceDados == change.indiceDados && actuacionesPosibles[i].accion == change.accion)
              return i;
          }
        }
      }
    }

    // full
    if (ddsExp.size() >= 2 || ddsExp.size() <= 4)
    {
      for (size_t casos = 0; casos < actuacionesPosibles.size(); casos++)
      {
        if ((actuacionesPosibles[casos].accion == "anotar") && (actuacionesPosibles[casos].anotacion.juego == "full") && actuacionesPosibles[casos].anotacion.puntos >= 30)
        {
          return casos;
        }
        else
        {
          change.indiceDados = Pos(dados, ddsExp, "full");
          for (int i = 0; i < actuacionesPosibles.size(); i++)
          {
            if (actuacionesPosibles[i].indiceDados == change.indiceDados && actuacionesPosibles[i].accion == change.accion)
              return i;
          }
        }
      }
    }

    // senas
    if (ddsExp.size() <= 3)
    {
      for (size_t casos = 0; casos < actuacionesPosibles.size(); casos++)
      {
        // actuacionesPosibles[casos].displayCompacto(); for(const auto& dado: dados);
        if (actuacionesPosibles[casos].accion == "anotar" && actuacionesPosibles[casos].anotacion.juego == "senas" && actuacionesPosibles[casos].anotacion.puntos >= 24)
        {
          return casos;
        }
        else
        {
          change.indiceDados = Pos5grandes(dados, 6);
          for (int i = 0; i < actuacionesPosibles.size(); i++)
          {
            if (actuacionesPosibles[i].indiceDados == change.indiceDados && actuacionesPosibles[i].accion == change.accion)
              return i;
          }
        }
      }
    }

    // escalera
    if (escala[0])
    {
      for (size_t casos = 0; casos < actuacionesPosibles.size(); casos++)
      {
        if ((actuacionesPosibles[casos].accion == "anotar") && (actuacionesPosibles[casos].anotacion.juego == "escalera" || actuacionesPosibles[casos].anotacion.juego == "escalera (de la 1ra mano)") && actuacionesPosibles[casos].anotacion.puntos >= 20)
        {
          return casos;
        }
        else
        {
          break;
          if (escala[0])

            return 0;
        }
      }
    }

    // unos trenes etc....
    if (ddsExp.size() <= 3)
    {
      for (size_t casos = 0; casos < actuacionesPosibles.size(); casos++)
      {
        // actuacionesPosibles[casos].displayCompacto(); for(const auto& dado: dados);
        if (actuacionesPosibles[casos].accion == "anotar")
        {
          const std::string jugada = actuacionesPosibles[casos].anotacion.juego;
          // if(jugada=="balas","tontos", "trenes", "cuadras", "quinas", "senas")
        }
      }
    }

    // std::cout << "fin turno\n"; retorno emergencia
    return actuacionesPosibles.size() - 1;
  }
};

#endif
