//este archivo se realizo para testear la funciones de comprobacion de jugadas del bot individualmente
#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include <random>
#include <string>
#include <vector>
#include <cmath>

using namespace std;
/*
"balas",    "tontos", "trenes", "cuadras", "quinas", "senas",
        "escalera", "full",   "poker",  "grande",  "grande2"

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







*/

//que dados he de relanzar /*por imprementar*/
  std::vector<int> Pos5grandes(const std::vector<int> &dados, int numMax){

    //jugada de grande1/2 //listo
    std::vector<int> Rpos;
    for(int i= 0;i<dados.size();i++){if(dados[i] != numMax){Rpos.push_back(i);}}
    std::sort(Rpos.begin(),Rpos.end());
    return Rpos;
    }


int main(){
    std::vector<int> v= {6,5,6,5,5};
    std::map<int,int> vv= {{1,1},{2,1},{3,4}};
    std::string juego= "grande";
    std::vector<int> V(0,5);
    cout<< " AAAAAAAAAAAAAAAAAAAA " <<endl;
    for(auto a: V)
    {cout << a;}
    cout <<endl;
}