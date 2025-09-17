#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>

#include "biliard.hpp"

namespace bl {

void move_between_points(sf::RenderWindow &window, sf::CircleShape &ball,
                         sf::Vector2f &starting_point,
                         sf::Vector2f &ending_point, float const speed,
                         sf::RectangleShape const &upper_cushion,
                         sf::RectangleShape const &lower_cushion);

void show_trajectory(std::vector<Point> const &subsequent_point,
                     bl::Biliard const &bil, float const offset);

void make_histograms(std::vector<double> const &ball_ys,
                     std::vector<double> const &ball_angles);

void show_histograms();

void run_single_launch(bl::Biliard &bil);

void run_multi_launch(bl::Biliard &bil);

bl::Biliard build_biliard();

}  // namespace bl
