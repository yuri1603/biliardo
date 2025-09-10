#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "biliard.hpp"
#include "doctest.h"

TEST_CASE("Testing collision function") {
  SUBCASE("Intersection with a horizontal path, first constructor") {
    bl::Path r1{0., 1.};
    bl::Path r2{1., 0.};
    bl::Point point = bl::collision(r1, r2);
    CHECK(point.x == doctest::Approx(1.));
    CHECK(point.y == doctest::Approx(1.));
  }
  SUBCASE("Intersection with a horizontal path, second constructor") {
    bl::Path r1{{0., 1.}, {3., 1.}};
    bl::Path r2{{1., 1.}, {0., 0.}};
    bl::Point point = bl::collision(r1, r2);
    CHECK(point.x == doctest::Approx(1.));
    CHECK(point.y == doctest::Approx(1.));
  }
  SUBCASE("Intersection with a horizontal path, third constructor") {
    bl::Path r1{{0., 1.}, 0.};
    bl::Path r2{{1., 1.}, 1.};
    bl::Point point = bl::collision(r1, r2);
    CHECK(point.x == doctest::Approx(1.));
    CHECK(point.y == doctest::Approx(1.));
  }
  SUBCASE("interception between two path, first constructor") {
    bl::Path r1{1., 3.};
    bl::Path r2{2., 0.};
    bl::Point point = bl::collision(r1, r2);
    CHECK(point.x == doctest::Approx(3.));
    CHECK(point.y == doctest::Approx(6.));
  }
  SUBCASE("interception between two path,second constructor") {
    bl::Path r1{{0., 3.}, {-3., 0.}};
    bl::Path r2{{0., 0.}, {1., 2.}};
    bl::Point point = bl::collision(r1, r2);
    CHECK(point.x == doctest::Approx(3.));
    CHECK(point.y == doctest::Approx(6.));
  }
  SUBCASE("interception between two path, third constructor") {
    bl::Path r1{{0., 3.}, 1.};
    bl::Path r2{{0., 0.}, 2};
    bl::Point point = bl::collision(r1, r2);
    CHECK(point.x == doctest::Approx(3.));
    CHECK(point.y == doctest::Approx(6.));
  }
  SUBCASE("Collision point beteen two parallel paths") {
    bl::Path r1{2., 3.};
    bl::Path r2{2., 0.};
    CHECK_THROWS(bl::collision(r1, r2));
  }
}

TEST_CASE("Testing first_collision function") {

  SUBCASE("first point negative x coordinate") {
    bl::Path r2{-0.25, 5.};
    bl::Path r1{-0.5, 4.};
    bl::Path r3{0.25, -5.};
    bl::Point point = bl::first_collision(r1, r2, r3);
    CHECK(point.x == 12.);
    CHECK(point.y == -2.);
  }

  SUBCASE("second point negative x coordinate") {
    bl::Path r3{-0.25, 5.};
    bl::Path r1{-0.5, 4.};
    bl::Path r2{0.25, -5.};
    bl::Point point = bl::first_collision(r1, r2, r3);
    CHECK(point.x == 12.);
    CHECK(point.y == -2.);
  }

  SUBCASE("both points have positive x coordinate") {
    bl::Path r3{-0.25, 5.};
    bl::Path r1{-0.15, 4.};
    bl::Path r2{0.25, -5.};
    bl::Point point = bl::first_collision(r1, r2, r3);
    CHECK(point.x == 10);
    CHECK(point.y == 2.5);
  }
}

TEST_CASE("Testing the no-bouncing mode") {
  bl::Biliard bil(10., 5., 2.5);
  bl::Ball b({0., 4.}, -0.5);
  bl::Ball ball = bil.Bounce(b);
  CHECK(ball.start_point.x == 10);
  CHECK(ball.start_point.y == -1);
  CHECK(ball.slope == -0.5);
}

