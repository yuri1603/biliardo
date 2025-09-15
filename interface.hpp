#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
// #include <functional>
#include <vector>

#include "biliard.hpp"

namespace bl {

void run_single_launch(bl::Biliard &bil);

void run_multi_launch(bl::Biliard &bil);

void biliard_geometry(bl::Biliard &bil);

void move_between_points(sf::RenderWindow &window, sf::CircleShape &shape,
                         sf::Vector2f start, sf::Vector2f end, float speed
                        //  , const std::function<void(sf::RenderWindow)>& draw_bg
                        );

void trajectory (std::vector<Point> const& coll, bl::Biliard &bil, float speed);

}  // namespace bl
