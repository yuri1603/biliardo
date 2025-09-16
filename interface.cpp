#include "interface.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "biliard.hpp"
#include "statistics.hpp"

namespace bl {
void run_single_launch(bl::Biliard &bil) {
  std::cout << "[Modalità lancio singolo]\n";
  bool running = true;
  while (running) {
    bl::Ball ball{};
    std::cout << "Inserisci ordinata iniziale della palla ( |y| < "
              << bil.left_limit() / 100 << " ) \n";
    std::cin >> ball.y_coord;
    std::cout << "Inserisci angolo iniziale della palla: (|ϑ| < π/2 )\n";
    std::cin >> ball.angle;
    bl::Ball initial = ball;
    bil.Dynamic(ball);
    if (ball.angle == 5.) {
      std::cout << "Hai inserito valori geometricamente incompatibili col "
                   "biliardo: \n";
    } else if (ball.angle == 4.) {
      std::cout << "La palla esce dal lato sinistro del biliardo \n";
    } else {
      std::cout << "L'ordinata finale della palla è: " << ball.y_coord << '\n';
      std::cout << "L'angolo finale della palla è: " << ball.angle << '\n';
    }
    std::cout << "Vuoi vedere il lancio? [Y/n] \n";
    char ans;
    std::cin >> ans;
    std::vector<Point> coll;
    bil.Dynamic_Trace(initial, coll);
    if (ans == 'y') {
      trajectory(coll, bil, 700.f);
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
  double l_i;
  double y1_i;
  double y2_i;
  std::cout << "Inserisci la lunghezza del biliardo: ( l < 14) \n";
  std::cin >> l_i;
  std::cout << "Inserisci la semiampiezza del lato sinistro del biliardo: ( y "
               "< 3) \n";
  std::cin >> y1_i;
  std::cout
      << "Inserisci la semiampiezza del lato destro del biliardo: (y < 3)\n";
  std::cin >> y2_i;
  double l = l_i * 100;
  double y1 = y1_i * 100;
  double y2 = y2_i * 100;
  bil = bl::Biliard(l, y1, y2);
}

void Biliard::Dynamic_Trace(Ball &b, std::vector<Point> &coll) {
  if (std::abs(b.y_coord) > upper_cushion_.y_intercept ||
      std::abs(b.angle) > (M_PI / 2)) {
    b = Ball{0., 5.};
  } else {
    Path ball_path{{0., b.y_coord}, std::tan(b.angle)};
    coll.push_back(Point{100.f, b.y_coord});
    Point collision_point =
        first_collision(ball_path, lower_cushion_, upper_cushion_);
    while (collision_point.x <= lenght_ && collision_point.x >= 0) {
      coll.push_back(Point{collision_point.x + 100.f, collision_point.y});
      if (collision_point.y > 0) {
        ball_path = Bounce(ball_path, upper_cushion_);
        collision_point = collision(ball_path, lower_cushion_);
      } else if (collision_point.y < 0) {
        ball_path = Bounce(ball_path, lower_cushion_);
        collision_point = collision(ball_path, upper_cushion_);
      }
    }
    if (upper_cushion_.slope < 0 &&
        (collision_point.x < 0 ||
         collision_point.x > collision(upper_cushion_, lower_cushion_).x)) {
      b = Ball{ball_path.y_intercept, std::atan(ball_path.slope)};
      coll.push_back(Point{100.f, ball_path.y_intercept});
    } else {
      b = Ball{ball_path.slope * lenght_ + ball_path.y_intercept,
               std::atan(ball_path.slope)};
      coll.push_back(Point{lenght_ + 100.f,
                           ball_path.slope * lenght_ + ball_path.y_intercept});
    }
  }
}

void move_between_points(sf::RenderWindow &window, sf::CircleShape &shape,
                         sf::Vector2f start, sf::Vector2f end, float speed,
                         sf::RectangleShape &upper_rect,
                         sf::RectangleShape &lower_rect) {
  shape.setPosition(start);
  sf::Clock clock;
  sf::Vector2f pos = start;
  sf::Vector2f delta = end - start;
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

    float dt = clock.restart().asSeconds();
    float step = speed * dt;
    if (traveled + step > total_dist) {
      step = total_dist - traveled;
    }

    pos += dir * step;
    traveled += step;
    shape.setPosition(pos);

    window.clear(sf::Color::Black);
    window.draw(upper_rect);
    window.draw(lower_rect);
    window.draw(shape);
    window.display();
  }
}

void trajectory(const std::vector<Point> &coll, bl::Biliard &bil,
                float speed = 100.f) {
  sf::RenderWindow window(sf::VideoMode(1600.f, 900.f), "Biliardo triangolare");
  window.setPosition(sf::Vector2i{10, 10});

  // Crea una view con origine a sinistra e centro in mezzo
  sf::View view;
  view.setSize(1600.f, -900.f);
  view.setCenter(700.f, 0.f);
  window.setView(view);

  // Crea i bordi del biliardo
  sf::RectangleShape upper_rect;
  upper_rect.setFillColor(sf::Color::White);
  float upper_rect_lenght = static_cast<float>(std::sqrt(
      bil.left_limit() * bil.left_limit() + bil.lenght() * bil.lenght()));
  upper_rect.setSize(sf::Vector2f{upper_rect_lenght, 10.f});
  upper_rect.setPosition(100.f, static_cast<float>(bil.left_limit()));
  float upper_rect_incl =
      static_cast<float>((180 * std::atan(bil.incl())) / M_PI);
  upper_rect.setRotation(upper_rect_incl);
  sf::RectangleShape lower_rect;
  lower_rect.setFillColor(sf::Color::White);
  lower_rect.setSize(sf::Vector2f{upper_rect_lenght, 10.f});
  float lower_rect_pos = static_cast<float>(-bil.left_limit());
  lower_rect.setPosition(100.f, lower_rect_pos);
  float lower_rect_incl = static_cast<float>(360 - upper_rect_incl);
  lower_rect.setRotation(lower_rect_incl);

  // Crea la palla
  sf::CircleShape ball;
  ball.setFillColor(sf::Color::White);
  ball.setRadius(10.f);
  ball.setOrigin(5.f, 5.f);

  ball.setPosition(sf::Vector2f{static_cast<float>(coll.front().x),
                                static_cast<float>(coll.front().y)});

  window.clear(sf::Color::Black);
  window.draw(ball);
  window.draw(upper_rect);
  window.draw(lower_rect);
  window.display();

  for (std::size_t i = 0; i + 1 < coll.size() && window.isOpen(); ++i) {
    sf::Vector2f A{static_cast<float>(coll[i].x),
                   static_cast<float>(coll[i].y)};
    sf::Vector2f B{static_cast<float>(coll[i + 1].x),
                   static_cast<float>(coll[i + 1].y)};
    move_between_points(window, ball, A, B, speed, upper_rect, lower_rect
                        // draw_bg
    );
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
    window.draw(upper_rect);
    window.draw(lower_rect);
    window.display();
  }
}
}  // namespace bl