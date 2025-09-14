#include "interface.hpp"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <string>

#include "biliard.hpp"
#include "statistics.hpp"

namespace bl {
void run_single_launch(bl::Biliard &bil) {
  std::cout << "[Modalità lancio singolo]\n";
  bool running = true;
  while (running) {
    bl::Ball ball{};
    std::cout << "Inserisci ordinata iniziale della palla ( y < "
              << bil.left_limit() << " ) \n";
    std::cin >> ball.y_coord;
    std::cout << "Inserisci angolo iniziale della palla: (-π/2 < ϑ < π/2 )\n";
    std::cin >> ball.angle;
    bil.Dynamic(ball);
    if (ball.angle == 5.) {
      std::cout << "Hai inserito valori geometricamente incompatibili col "
                   "biliardo: \n";
    } else if (ball.angle == 4.) {
      std::cout << "La palla esce dal lato sinistro del biliardon \n";
    } else {
      std::cout << "L'ordinata finale della palla è: " << ball.y_coord << '\n';
      std::cout << "L'angolo finale della palla è: " << ball.angle << '\n';
    }
    std::cout << "Vuoi effettuare un altro lancio? [Y/n] \n";
    std::string answer;
    std::cin >> answer;
    if (answer != "y") {
      running = false;
    }
  }
}

void run_multi_launch(bl::Biliard &bil) {
  std::cout << "[Modalità lanci multipli]\n";
  bool running = true;
  while (running) {
    std::vector<bl::Ball> sample;
    unsigned long int N;
    double y_mean, y_std_dev, angle_mean, angle_std_dev;
    std::cout << "Inserisci il numero di elementi del campione: (N > 2) \n";
    std::cin >> N;
    std::cout << "Inserisci la media delle ordinate iniziali: \n";
    std::cin >> y_mean;
    std::cout << "Inserisci la deviazione standard delle ordinate iniziali: (σ "
                 "> 0) \n";
    std::cin >> y_std_dev;
    std::cout << "Inserisci la media degli angoli iniziali: \n";
    std::cin >> angle_mean;
    std::cout
        << "Inserisci la deviazione standard degli angoli inziali: (σ > 0) \n";
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
    std::cout << "Vuoi effettuare un altro lancio multiplo? [Y/n] \n";
    std::string answer;
    std::cin >> answer;
    if (answer != "y") {
      running = false;
    }
  }
}

void biliard_geometry(bl::Biliard &bil) {
  std::cout << "[Nuovi parametri biliardo]\n";
  double l, y1, y2;
  std::cout << "Inserisci la lunghezza del biliardo: (l > 0) \n";
  std::cin >> l;
  std::cout
      << "Inserisci la semiampiezza del lato sinistro del biliardo: (y > 0) \n";
  std::cin >> y1;
  std::cout
      << "Inserisci la semiampiezza del lato destro del biliardo: (y > 0)\n";
  std::cin >> y2;
  bil = bl::Biliard(l, y1, y2);
}

}  // namespace bl
