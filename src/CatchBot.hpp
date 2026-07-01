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
/**/
class CatchBot : public Jugador
{
private:
  std::string nombreE;

public:
/* nombre alumno :) */
  CatchBot(){nombreE = "Isidora Garcia Paz Perez, rol:202573029-4, parCatedra:2";}
  std::string GetNombreEst() const
  {return nombreE;}

  CatchBot(std::string nom = "CatchBot") : Jugador(nom) { nombre = nom; }
  int turnoIn = 0;

  /* en esta funcion se leen los dados de izq->der y der->izq para verificar si hay posivilidad de realizar una escalera, en el caso de que si se pueda se da un booleano extra que dice como se debe leer los dados*/
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

  /* SE PUEDE CREAR UNA ESCALERA!! que dados no son continuos y en que posicion estan, con esta funcion obtenemos la posicion e los dados que no son continuos*/
  std::vector<int> incontiniudad(const std::vector<int> &dados, bool lectura)
  {
    int cont = 0;
    std::vector<std::vector<int>> temp; /* matriz con dado y pos original de dado */
    for (const auto &dado : dados)
    {
      temp.push_back({dado, cont});
      cont++;
    }
    std::vector<int> pos;
    sort(temp.begin(), temp.end());

    if (lectura)
    { // {1,3,4,5,6} lectura de dados normal der<--izq
      cont = temp.size() - 1; 
      std::vector<int> fin = temp.at(cont);//ultima pos -> 6 pivote
      int start = fin[0];
      for (int i = cont; 0 <= i; i--, start--)
      {
        if ((temp.at(i))[0] != start)// para ver la continuidad hacemos un for y le vamos restando al pivote 1
        {
          pos.push_back((temp.at(i))[1]);
        }
      }
    }
    else
    { // {1,2,3,4,6} lectura de dados revez der-->izq
      std::vector<int> inicio = temp.at(0);
      int start = inicio[0];//pos inicial ->1 pivote
      for (auto dado : temp)
      {
        if (dado[0] != start)
        {
          pos.push_back(dado[1]);// para ver la continuidad hacemos un for y le vamos sumamos al pivote 1
        }
        start++;
        cont++;
      }
    }
    std::sort(pos.begin(), pos.end());
    return pos;
  }

  /*rep de numeros, que numeros se repiten, cuantas veces? El tamaño del map permite inferir rápidamente qué jugadas son posibles: tamaño 1 o 2 sugiere una grande o un póker
  ya que 2 --> map=<3,3>,<2,2>/dado={3,3,3,2,2} o 3--> map=<3,2>,<1,2>,<2,1>/dado={3,3,1,2,1}*/
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

  // que dados he de relanzar si es posible una jugada
  std::vector<int> Pos(const std::vector<int> &dados, std::map<int, int> map, std::string jugada)
  {

    // jugada de grande1/2
    std::vector<int> Rpos; //obtenemos el valor con mayor repeticiones y lo comparamos, si el numero es distinto al mas repetido guarda la posicion
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
        if (dad.second > max) //obtenemos el valor con mayor repeticiones y lo comparamos
        {
          max = dad.second;
          numMax = dad.first;
        }
        if (dad.second == 1)//si ya hay un numero que se repite una vez se guarda como segundo num que no se lanza
        {
          numMin = dad.first;
        }
      }

      for (int i = 0; i < dados.size(); i++)
      {
        if (dados[i] != numMax && dados[i] != numMin)
        {
          Rpos.push_back(i);
          if (numMin == 0)//si no hay un numero que se repite una vez y el numMax se repite mas 3 veces se lanza el primer numero que no es el nummax
            break;
        }
      }

      std::sort(Rpos.begin(), Rpos.end());
      return Rpos;
    }
    // jugada full
    /*Guarda los dos números con más repeticiones y lanza solo el dado que no pertenece a ninguno de los dos grupos con mayor repeticion*/
    if (jugada == "full")
    {
      int max = -1, numMax;
      int numMax2;
      for (auto dad : map)
      {
        if (dad.second > max)//si ya hay un num repetido 2 o 3 veces
        {
          max = dad.second;
          numMax = dad.first;
        }
        if (dad.first != numMax || dad.second < max)// en el caso que un num se rep 3 y el resto una vez agarra el primer numero que sea distinto al mas repetido para no lanzarlo
          numMax2 = dad.first;
        if (dad.second == max)//si hay dos nums repetidos 2 veces guarda el segundo numero que se repite 2 veces
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
  /* ya que todas requieren repetir un número específico; se pasa el número objetivo 1,2,3,4 y se obtienen las posiciones de los dados que no coinciden */
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

    // si debo relanzar dados como se debe ver mi jugada, clase hecha para ser modificada y luego igualada a la jugada ideal
    Actuacion change("lanzar", {" ", -1}, {0, 1, 2, 3, 4}, false);
    // cuanto se repiten los dados esta partida
    std::map<int, int> ddsExp = iguales(dados);
    // hay posivilidad de escalera? si la hay como debo leer los dados
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

    //El bot siempre intenta primero el puntaje máximo de mano por lo cual ve todas las opciones posibles, si no es posible anotar al inicio verifica si se puede crear alguna tirada de hueva;
    //el caso de tonton,balas, senas solo anotara si hay 4 o mas dados iguales dado que el puntaje no combia si es de mano o huevo
    //esta es la forma que trabaja con todas las jugadas, se puede hacer de mano; no, puedo crear un lanzamiento de hueva, lanzamos si podemos. y asi hasta que esten anotados la mayoria de cosas

    // grande1 y 2
    if (ddsExp.size() < 3)// ya hay jugada con map= <num,5> o se puede hacer crear jugada con map = <num1,3>,<num2,2>
    {
      for (size_t casos = 0; casos < actuacionesPosibles.size(); casos++)
      {
        if ((actuacionesPosibles[casos].accion == "anotar") && (actuacionesPosibles[casos].anotacion.juego == "grande" || actuacionesPosibles[casos].anotacion.juego == "grande2") && actuacionesPosibles[casos].anotacion.puntos >= 40)
        {
          return casos;
        }
      }
      for (size_t casos = 0; casos < actuacionesPosibles.size(); casos++)
      {
          change.indiceDados = Pos(dados, ddsExp, "grande");
          for (int i = 0; i < actuacionesPosibles.size(); i++)
          {
            if (actuacionesPosibles[i].indiceDados == change.indiceDados && actuacionesPosibles[i].accion == change.accion)
              return i;
          }
          break;//revisamos si se puede realizar ese lanzamiento con los dados que debemos mantener si no se puede salimos del ciclo
        }
      } 
      

    // poker
    if (ddsExp.size() <= 3 || ddsExp.size()!=1) // ya hay jugada con map= <num1,4>,<num2,1> o se puede hacer crear jugada con map = <>,<>,<> 
    {
      for (size_t casos = 0; casos < actuacionesPosibles.size(); casos++)
      {
        if ((actuacionesPosibles[casos].accion == "anotar") && (actuacionesPosibles[casos].anotacion.juego == "poker") && actuacionesPosibles[casos].anotacion.puntos >= 40)
        {
          return casos;
        }
      }
      for (size_t casos = 0; casos < actuacionesPosibles.size(); casos++)
      {
          change.indiceDados = Pos(dados, ddsExp, "poker");
          for (int i = 0; i < actuacionesPosibles.size(); i++)
          {
            if (actuacionesPosibles[i].indiceDados == change.indiceDados && actuacionesPosibles[i].accion == change.accion)
              return i;
          }
          break;//revisamos si se puede realizar ese lanzamiento con los dados que debemos mantener si no se puede salimos del ciclo
      }
        
    }

    // full
    if (ddsExp.size() >= 2 || ddsExp.size() <= 4) // ya hay jugada con map= <num1,2>,<num2,3> o se puede hacer crear jugada con map = <>,<>,<> o <>,<>,<>,<>
    {
      for (size_t casos = 0; casos < actuacionesPosibles.size(); casos++)
      {
        if ((actuacionesPosibles[casos].accion == "anotar") && (actuacionesPosibles[casos].anotacion.juego == "full") && actuacionesPosibles[casos].anotacion.puntos >= 30)
        {
          return casos;
        }
      }
      for (size_t casos = 0; casos < actuacionesPosibles.size(); casos++)
        {
          change.indiceDados = Pos(dados, ddsExp, "full");
          for (int i = 0; i < actuacionesPosibles.size(); i++)
          {
            if (actuacionesPosibles[i].indiceDados == change.indiceDados && actuacionesPosibles[i].accion == change.accion)
              return i;
          }
          break;//revisamos si se puede realizar ese lanzamiento con los dados que debemos mantener si no se puede salimos del ciclo
        }
    }

    // senas
    if (ddsExp.size() <= 3)
    {
      for (size_t casos = 0; casos < actuacionesPosibles.size(); casos++)
      {

        if (actuacionesPosibles[casos].accion == "anotar" && actuacionesPosibles[casos].anotacion.juego == "senas" && actuacionesPosibles[casos].anotacion.puntos >= 24)
        {
          return casos;
        }
      }
      for (size_t casos = 0; casos < actuacionesPosibles.size(); casos++)
        {
          change.indiceDados = Pos5grandes(dados, 6);
          for (int i = 0; i < actuacionesPosibles.size(); i++)
          {
            if (actuacionesPosibles[i].indiceDados == change.indiceDados && actuacionesPosibles[i].accion == change.accion)
              return i;
          }
          break;//revisamos si se puede realizar ese lanzamiento con los dados que debemos mantener si no se puede salimos del ciclo
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
      }
      for (size_t casos = 0; casos < actuacionesPosibles.size(); casos++)
        {
          change.indiceDados = incontiniudad(dados, escala[1]);
          for (int i = 0; i < actuacionesPosibles.size(); i++)
          {
            if (actuacionesPosibles[i].indiceDados == change.indiceDados && actuacionesPosibles[i].accion == change.accion)
              return i;
          }
          break;//revisamos si se puede realizar ese lanzamiento con los dados que debemos mantener si no se puede salimos del ciclo
        }
    }

    // "balas","tontos","trenes","cuadras","quinas"
    if (ddsExp.size() <= 3)
    {
      for (int q = 0; q < 5; q++)// revisamos individualmente cada jugada de la que da mas punto a la que da menos puntos
      {
        //quinas
        if(q==0){
          for (size_t casos = 0; casos < actuacionesPosibles.size(); casos++)
          {
            if (actuacionesPosibles[casos].accion == "anotar" && actuacionesPosibles[casos].anotacion.juego == "quinas" && actuacionesPosibles[casos].anotacion.puntos >= 20)
            {
              return casos;
            }
          }
          for (size_t casos = 0; casos < actuacionesPosibles.size(); casos++)
            {
              change.indiceDados = Pos5grandes(dados, 5);
              for (int i = 0; i < actuacionesPosibles.size(); i++)
              {
                if (actuacionesPosibles[i].indiceDados == change.indiceDados && actuacionesPosibles[i].accion == change.accion)
                  return i;
              }
              break;//revisamos si se puede realizar ese lanzamiento con los dados que debemos mantener si no se puede salimos del ciclo
            }
        }

        //cuadras
        if(q==1){
          for (size_t casos = 0; casos < actuacionesPosibles.size(); casos++)
          {
            if (actuacionesPosibles[casos].accion == "anotar" && actuacionesPosibles[casos].anotacion.juego == "cuadras" && actuacionesPosibles[casos].anotacion.puntos >= 16)
            {
              return casos;
            }
          }
          for (size_t casos = 0; casos < actuacionesPosibles.size(); casos++)
            {
              change.indiceDados = Pos5grandes(dados, 4);
              for (int i = 0; i < actuacionesPosibles.size(); i++)
              {
                if (actuacionesPosibles[i].indiceDados == change.indiceDados && actuacionesPosibles[i].accion == change.accion)
                  return i;
              }
              break;
            }
        }

        //trenes
        if(q==2){
          for (size_t casos = 0; casos < actuacionesPosibles.size(); casos++)
          {
            if (actuacionesPosibles[casos].accion == "anotar" && actuacionesPosibles[casos].anotacion.juego == "trenes" && actuacionesPosibles[casos].anotacion.puntos >= 12)
            {
              return casos;
            }
          }
          for (size_t casos = 0; casos < actuacionesPosibles.size(); casos++)
            {
              change.indiceDados = Pos5grandes(dados, 3);
              for (int i = 0; i < actuacionesPosibles.size(); i++)
              {
                if (actuacionesPosibles[i].indiceDados == change.indiceDados && actuacionesPosibles[i].accion == change.accion)
                  return i;
              }
              break;
            }
        }

        //tontos
        if(q==3){
          for (size_t casos = 0; casos < actuacionesPosibles.size(); casos++)
          {
            if (actuacionesPosibles[casos].accion == "anotar" && actuacionesPosibles[casos].anotacion.juego == "tontos" && actuacionesPosibles[casos].anotacion.puntos >= 8)
            {
              return casos;
            }
          }
          for (size_t casos = 0; casos < actuacionesPosibles.size(); casos++)
            {
              change.indiceDados = Pos5grandes(dados, 2);
              for (int i = 0; i < actuacionesPosibles.size(); i++)
              {
                if (actuacionesPosibles[i].indiceDados == change.indiceDados && actuacionesPosibles[i].accion == change.accion)
                  return i;
              }
              break;
            }
        }

        //balas
        if(q==4){
          for (size_t casos = 0; casos < actuacionesPosibles.size(); casos++)
          {
            if (actuacionesPosibles[casos].accion == "anotar" && actuacionesPosibles[casos].anotacion.juego == "balas" && actuacionesPosibles[casos].anotacion.puntos >= 4)
            {
              return casos;
            }
          }
          for (size_t casos = 0; casos < actuacionesPosibles.size(); casos++)
            {
              change.indiceDados = Pos5grandes(dados, 1);
              for (int i = 0; i < actuacionesPosibles.size(); i++)
              {
                if (actuacionesPosibles[i].indiceDados == change.indiceDados && actuacionesPosibles[i].accion == change.accion)
                  return i;
              }
              break;
            }
        }
      }
    }

    // retorno emergencia
    return actuacionesPosibles.size() - 1;
  }
};

#endif
