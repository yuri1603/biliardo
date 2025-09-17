#include "interface.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <climits>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "TCanvas.h"
#include "TH1.h"
#include "biliard.hpp"
#include "statistics.hpp"

namespace bl {

void move_between_points(sf::RenderWindow &window, sf::CircleShape &ball,
                         sf::Vector2f &starting_point,
                         sf::Vector2f &ending_point, float const step,
                         sf::RectangleShape const &upper_cushion,
                         sf::RectangleShape const &lower_cushion) {
  ball.setPosition(starting_point);
  sf::Vector2f pos = starting_point;
  sf::Vector2f delta = ending_point - starting_point;
  float total_dist = std::sqrt(delta.x * delta.x + delta.y * delta.y);
  sf::Vector2f dir = delta / total_dist;
  float traveled = 0.f;

  while (window.isOpen() && traveled < total_dist) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }
    float current_step = step;
    if (traveled + current_step > total_dist) {
      current_step = total_dist - traveled;
    }
    pos += dir * current_step;
    traveled += current_step;
    ball.setPosition(pos);

    window.clear(sf::Color::Black);
    window.draw(upper_cushion);
    window.draw(lower_cushion);
    window.draw(ball);
    window.display();
  }
}

void show_trajectory(std::vector<Point> const &subsequent_points,
                     bl::Biliard const &bil, float const offset) {
  sf::RenderWindow window(sf::VideoMode(1600.f, 900.f), "Biliardo triangolare");
  window.setPosition(sf::Vector2i{10, 10});

  sf::View view;
  view.setSize(1600.f, -900.f);
  view.setCenter(700.f, 0.f);
  window.setView(view);

  sf::RectangleShape upper_cushion;
  upper_cushion.setFillColor(sf::Color::White);
  float upper_rect_lenght =
      static_cast<float>(std::sqrt(bil.get_left_limit() * bil.get_left_limit() +
                                   bil.get_lenght() * bil.get_lenght()));
  upper_cushion.setSize(sf::Vector2f{upper_rect_lenght, 10.f});
  upper_cushion.setPosition(offset, static_cast<float>(bil.get_left_limit()));
  float upper_rect_incl =
      static_cast<float>((180 * std::atan(bil.get_cushion_slope())) / M_PI);
  upper_cushion.setRotation(upper_rect_incl);
  sf::RectangleShape lower_cushion;
  lower_cushion.setFillColor(sf::Color::White);
  lower_cushion.setSize(sf::Vector2f{upper_rect_lenght, 10.f});
  float lower_rect_pos = static_cast<float>(-bil.get_left_limit());
  lower_cushion.setPosition(offset, lower_rect_pos);
  float lower_rect_incl = static_cast<float>(360 - upper_rect_incl);
  lower_cushion.setRotation(lower_rect_incl);

  sf::CircleShape ball;
  ball.setFillColor(sf::Color::White);
  ball.setRadius(10.f);
  ball.setOrigin(5.f, 5.f);
  ball.setPosition(
      sf::Vector2f{static_cast<float>(subsequent_points.front().x),
                   static_cast<float>(subsequent_points.front().y)});

  window.clear(sf::Color::Black);
  window.draw(ball);
  window.draw(upper_cushion);
  window.draw(lower_cushion);
  window.display();

  for (std::size_t i = 0; i + 1 < subsequent_points.size() && window.isOpen();
       ++i) {
    sf::Vector2f A{static_cast<float>(subsequent_points[i].x),
                   static_cast<float>(subsequent_points[i].y)};
    sf::Vector2f B{static_cast<float>(subsequent_points[i + 1].x),
                   static_cast<float>(subsequent_points[i + 1].y)};
    move_between_points(window, ball, A, B, 2.f, upper_cushion, lower_cushion);
  }

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    window.clear(sf::Color::Black);
    window.draw(ball);
    window.draw(upper_cushion);
    window.draw(lower_cushion);
    window.display();
  }
}

void make_histograms(std::vector<double> const &ball_ys,
                     std::vector<double> const &ball_angles) {
  auto mm1 = std::minmax_element(ball_ys.begin(), ball_ys.end());
  TH1D h1("h1", "Istogramma degli angoli;Angolo(rad);Ocorrenze", 50, *mm1.first,
          *mm1.second);
  for (double x : ball_ys) h1.Fill(x);
  TCanvas c1("c1", "c1", 800, 500);
  h1.Draw();
  c1.SaveAs("hist1.png");

  auto mm2 = std::minmax_element(ball_angles.begin(), ball_angles.end());
  TH1D h2("h2", "Istogramma delle ordinate;Ordinata(px);Occorrenze", 50,
          *mm2.first, *mm2.second);
  for (double x : ball_angles) h2.Fill(x);
  TCanvas c2("c2", "c2", 800, 500);
  h2.Draw();
  c2.SaveAs("hist2.png");
}

void show_histograms() {
  sf::RenderWindow window(sf::VideoMode(1750, 600), "Istogrammi");

  sf::Texture t1, t2;
  t1.loadFromFile("hist1.png");
  t2.loadFromFile("hist2.png");

  sf::Sprite s1(t1), s2(t2);
  s1.setPosition(50.f, 50.f);
  s2.setPosition(900.f, 50.f);

  while (window.isOpen()) {
    sf::Event e;
    while (window.pollEvent(e))
      if (e.type == sf::Event::Closed) window.close();

    window.clear();
    window.draw(s1);
    window.draw(s2);
    window.display();
  }
}

void run_single_launch(bl::Biliard &bil) {
  std::cout << "[Modalità lancio singolo]\n";
  bool running = true;
  while (running) {
    bl::Ball ball_i{};
    std::cout << "Inserisci ordinata iniziale della palla ( |y| < "
              << bil.get_left_limit() / 100 << " ) \n";
    std::cin >> ball_i.y;
    if (std::abs(ball_i.y) >= bil.get_left_limit() / 100) {
      throw std::runtime_error("Hai inserito un parametro non valido");
    }
    std::cout << "Inserisci angolo iniziale della palla: ( - π/2 < ϑ < π/2 )\n";
    std::cin >> ball_i.angle;
    if (ball_i.angle >= M_PI / 2 || ball_i.angle <= -M_PI / 2) {
      throw std::runtime_error("Hai inserito un parametro non valido");
    }
    Ball ball{ball_i.y * 100, ball_i.angle};
    bl::Ball initial = ball;
    bil.compute_final_position(ball);
    if (ball.angle == 2.) {
      std::cout << "La palla esce dal lato sinistro del biliardo \n";
    } else {
      std::cout << "L'ordinata finale della palla è: " << ball.y << '\n';
      std::cout << "L'angolo finale della palla è: " << ball.angle << '\n';
    }
    std::cout << "Vuoi vedere il lancio? [Y/n] \n";
    char ans;
    std::cin >> ans;
    std::vector<Point> subsequent_points;
    bil.trace_trajectory(initial, subsequent_points, 100.);
    if (ans == 'y') {
      show_trajectory(subsequent_points, bil, 100.f);
    }
    std::cout << "Vuoi effettuare un altro lancio? [Y/n] \n";
    char answer;
    std::cin >> answer;
    if (answer != 'y') {
      running = false;
    }
  }
}

void run_multi_launch(bl::Biliard &bil) {
  std::cout << "[Modalità lanci multipli]\n";
  bool running = true;
  while (running) {
    std::vector<bl::Ball> sample;
    long int N;
    double y_mean, y_std_dev, angle_mean, angle_std_dev;
    std::cout << "Inserisci il numero di elementi del campione: (N > 2) \n";
    std::cin >> N;
    if (N <= 2) {
      throw std::runtime_error("Hai inserito un valore non valido");
    }
    std::cout
        << "Inserisci la media delle ordinate iniziali: (si consiglia |y| < "
        << bil.get_left_limit() / 100 << " )\n";
    std::cin >> y_mean;
    std::cout << "Inserisci la deviazione standard delle ordinate iniziali: (σ "
                 "> 0) \n";
    std::cin >> y_std_dev;
    if (y_std_dev <= 0) {
      throw std::runtime_error("Hai inserito un valore non valido");
    }
    std::cout << "Inserisci la media degli angoli iniziali: (si consiglia - "
                 "π/2 < ϑ < π/2 )\n";
    std::cin >> angle_mean;
    std::cout << "Inserisci la deviazione standard degli angoli inziali: (σ "
                 "> 0) \n";
    std::cin >> angle_std_dev;
    if (angle_std_dev <= 0) {
      throw std::runtime_error("Hai inserito un valore non valido");
    }
    sample = bil.generate_random_balls(static_cast<unsigned long int>(N),
                                       y_mean * 100, y_std_dev * 100,
                                       angle_mean, angle_std_dev);
    bl::Samples ensemble;
    ensemble = bil.split_for_stats(sample);
    bl::Statistics angle_result;
    angle_result = bl::evaluate_statistics(ensemble.ball_angles);
    bl::Statistics y_result;
    y_result = bl::evaluate_statistics(ensemble.ball_ys);
    std::cout << "La media delle ordinate finali è: " << y_result.mean << '\n';
    std::cout << "La deviazione standard delle ordinate finali è: "
              << y_result.std_dev << '\n';
    std::cout << "Il coefficiente di simmetria delle ordinate finali è: "
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
    std::cout << "Vuoi visualizzare gli istogrammi dei parametri in uscita? "
                 "[Y/n]\n";
    char ans;
    std::cin >> ans;
    if (ans == 'y') {
      make_histograms(ensemble.ball_angles, ensemble.ball_ys);
      show_histograms();
    }
    std::cout << "Vuoi effettuare un altro lancio multiplo? [Y/n] \n";
    std::string answer;
    std::cin >> answer;
    if (answer != "y") {
      running = false;
    }
  }
}

bl::Biliard build_biliard() {
  std::cout << "[Nuovi parametri biliardo]\n";
  double l_i;
  double y1_i;
  double y2_i;
  std::cout
      << "Inserisci la lunghezza del biliardo: ( l > 0, si consiglia 6 <= "
         "l <= 12 ) \n";
  std::cin >> l_i;
  if (l_i <= 0) {
    throw std::runtime_error("Hai inserito un valore non valido");
  }
  std::cout << "Inserisci la semiampiezza del lato sinistro del biliardo: ( y "
               "> 0, si consiglia y <= 3.5 )\n";
  std::cin >> y1_i;
  if (y1_i <= 0) {
    throw std::runtime_error("Hai inserito un valore non valido");
  }
  std::cout << "Inserisci la semiampiezza del lato destro del biliardo: ( y "
               "> 0, si consiglia y <= 3.5 )\n";
  std::cin >> y2_i;
  if (y2_i <= 0) {
    throw std::runtime_error("Hai inserito un valore non valido");
  }
  double l = l_i * 100;
  double y1 = y1_i * 100;
  double y2 = y2_i * 100;
  return bl::Biliard(l, y1, y2);
}

}  // namespace bl