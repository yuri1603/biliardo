#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdexcept>

#include "biliard.hpp"
#include "interface.hpp"

int main() {
  bool running = true;

  std::cout << "=== Simulatore biliardo triangolare ===\n";

  bl::Biliard bil = bl::build_biliard();

  while (running) {
    std::cout << "\n> ";
    std::cout
        << "Comandi: singolo [s], multiplo [m], geometria [g], esci [e] \n";
    char command;
    std::cin >> command;

    switch (command) {
      case 's':
        bl::run_single_launch(bil);
        break;

      case 'm':
        bl::run_multi_launch(bil);
        break;

      case 'g':
        bl::build_biliard();
        break;

      case 'e':
        running = false;
        break;

      default:
        std::cout << "Hai inserito un carattere non valido \n";
        throw std::runtime_error("Invalid parameter");
    }
  }

  std::cout << "Chiusura del simulatore.\n";
}
