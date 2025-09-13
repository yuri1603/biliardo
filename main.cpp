#include <iostream>
#include <string>

#include "biliard.hpp"
#include "statistics.hpp"

void run_single_launch(bl::Biliard &bil) {
  std::cout << "[Modalità lancio singolo]\n";
  bool running = true;
  while (running) {
    bl::Ball ball{};
    std::cout << "Inserisci ordinata iniziale della palla: \n";
    std::cin >> ball.y_coord;
    std::cout << "Inserisci angolo iniziale della palla \n";
    std::cin >> ball.angle;
    bil.Dynamic(ball);
    if (ball.angle == 5.) {
      std::cout << "Hai inserito valori geometricamente incompatibili col "
                   "biliardo \n";
    } else if (ball.angle == 4) {
      std::cout << "La palla esce dal lato sinistro del biliardo \n";
    } else {
      std::cout << "L'ordinata finale della palla è: " << ball.y_coord << '\n';
      std::cout << "L'angolo finale della palla è " << ball.angle << '\n';
    }
    std::cout << "Vuoi effettuare un altro lancio? \n";
    std::string answer;
    std::cin >> answer;
    if (answer != "si") {
      running = false;
    }
  }
  // chiedi angolo, y, ecc. e usa la tua logica
}

void run_multi_launch(bl::Biliard &bil) {
  std::cout << "[Modalità lanci multipli]\n";
  bool running = true;
  while (running) {
    std::vector<bl::Ball> sample;
    unsigned long int N;
    double y_mean, y_std_dev, angle_mean, angle_std_dev;
    std::cout << "Inserisci il numero di elementi del campione: \n";
    std::cin >> N;
    std::cout << "Inserisci la media delle ordinate iniziali: \n";
    std::cin >> y_mean;
    std::cout << "Inserisci la deviazione standard delle ordinate iniziali: \n";
    std::cin >> y_std_dev;
    std::cout << "Inserisci la media degli angoli iniziali: \n";
    std::cin >> angle_mean;
    std::cout << "Inserisci la deviazione standard degli angoli inziali: \n";
    std::cin >> angle_std_dev;
    sample = bil.random_balls(N, y_mean, y_std_dev, angle_mean, angle_std_dev);
    bl::Sample ensemble;
    ensemble = bil.split(sample);
    bl::Statistics angle_result;
    angle_result = bl::stats(ensemble.angles);
    bl::Statistics y_result;
    y_result = bl::stats(ensemble.y_coord);
    std::cout << "La media delle ordinate finali è: " << y_result.mean << '\n';
    std::cout << "La deviazione standard delle ordinate finali è: "
              << y_result.std_dev << '\n';
    std::cout << "Il coefficienti di simmetria delle ordinate finali è: "
              << y_result.skewness << '\n';
    std::cout << "Il coefficiente di appiattimento delle ordinate finali è: "
              << y_result.kurtosis << '\n';
    std::cout << "La media degli angoli finali è: " << angle_result.mean
              << '\n';
    std::cout << "La deviazione standard degli angoli finali è: "
              << angle_result.std_dev << '\n';
    std::cout << "Il coefficienti di simmetria degli angoli finali è: "
              << angle_result.skewness << '\n';
    std::cout << "Il coefficiente di appiattimento degli angoli finali è: "
              << angle_result.kurtosis << '\n';
    std::cout << "Vuoi effettuare un altro lancio multiplo? \n";
    std::string answer;
    std::cin >> answer;
    if (answer != "si") {
      running = false;
    }
  }
}

void biliard_geometry(bl::Biliard &bil) {
  std::cout << "[Nuovi parametri biliardo]\n";
  double l, y1, y2;
  std::cout << "Inserisci la lugnhezza del biliardo: \n";
  std::cin >> l;
  std::cout << "Inserisci la semiampiezza del lato sinistro del biliardo: \n";
  std::cin >> y1;
  std::cout << "Inserisci la semiampiezza del lato destro del biliardo: \n";
  std::cin >> y2;
  bil = bl::Biliard(l, y1, y2);
}

int main() {
  bool running = true;
  std::string command;
  bl::Biliard bil(1., 1., 1.);

  std::cout << "=== Simulatore biliardo triangolare ===\n";

  biliard_geometry(bil);

  std::cout << "Comandi: singolo, multiplo, geometria, esci\n";

  while (running) {
    std::cout << "\n> ";
    std::cout << "Comandi: singolo, multiplo, geometria, esci\n";
    std::cin >> command;
    if (command == "singolo") {
      run_single_launch(bil);
    } else if (command == "multiplo") {
      run_multi_launch(bil);
    } else if (command == "geometria") {
      biliard_geometry(bil);
    } else if (command == "esci") {
      running = false;
    } else {
      std::cout << "Comando non riconosciuto.\n";
    }
  }

  std::cout << "Chiusura del simulatore.\n";
}
