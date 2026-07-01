#include "JugadorAleatorio.h"
#include "CatchBot.hpp"
#include "Tournament.h"
#include <iostream>
#include <memory>

int main()
{
    Tournament t;

    t.addPlayer(
        []()
        { return std::make_shared<JugadorAleatorio>("Bot_Random_1"); });
    t.addPlayer(
        []()
        { return std::make_shared<CatchBot>("CatchBot"); });
    t.addPlayer(
        []()
        { return std::make_shared<JugadorAleatorio>("Bot"); });
    std::cout << "Starting simulation with 3 Random Players (100 matches)..."
              << std::endl;

    t.run(100);
    t.displayResults();

    return 0;
}
