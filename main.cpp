#include <iostream>

#include "biliard.hpp"
#include "interface.hpp"

int main() {
  bool running = true;
  bl::Biliard bil(1., 1., 1.);

  std::cout << "=== Simulatore biliardo triangolare ===\n";

  biliard_geometry(bil);

  while (running) {
    std::cout << "\n> ";
    std::cout
        << "Comandi: singolo [s], multiplo [m], geometria [g], esci [e] \n";
    char command;
    std::cin >> command;

    switch (command) {
      case 's':
        run_single_launch(bil);
        break;

      case 'm':
        run_multi_launch(bil);
        break;

      case 'g':
        biliard_geometry(bil);
        break;

      case 'e':
        running = false;
        break;

      default:
        std::cout << "Comando non riconosciuto.\n";
        break;
    }
  }

  std::cout << "Chiusura del simulatore.\n";
}