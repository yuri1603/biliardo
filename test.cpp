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
    bl::Point point = bl::collision(r1, r2);
    CHECK(point.x == -1.);
    CHECK(point.y == 0.);
  }
}

TEST_CASE("Testing first_collision function") {
  bl::Path r2{-0.25, 5.};
  bl::Path r3{0.25, -5.};

  SUBCASE("first point negative x coordinate") {
    bl::Path r1{-0.5, 4.};
    bl::Point point = bl::first_collision(r1, r2, r3);
    CHECK(point.x == 12.);
    CHECK(point.y == -2.);
  }

  SUBCASE("second point negative x coordinate") {
    bl::Path r1{-0.5, 4.};
    bl::Point point = bl::first_collision(r1, r2, r3);
    CHECK(point.x == 12.);
    CHECK(point.y == -2.);
  }

  SUBCASE("both points have positive x coordinate") {
    bl::Path r1{-0.15, 4.};
    bl::Point point = bl::first_collision(r1, r2, r3);
    CHECK(point.x == 10);
    CHECK(point.y == 2.5);
  }
}

TEST_CASE("Testing bounce method") {
  SUBCASE("standard situation") {
    bl::Biliard bil(4., 3., 1);
    bl::Path r1{2., -7.};
    bl::Path r2{0.5, -3.};
    bl::Path path = bil.Bounce(r1, r2);
    CHECK(path.slope == doctest::Approx(-2.0 / 11.0));
    CHECK(path.y_intercept == doctest::Approx(-13.0 / 11.0));
  }
}

TEST_CASE("Testing dynamic method") {
  SUBCASE("no bouncing case") {
    bl::Biliard bil{6., 4., 2.8};
    bl::Ball b_i{{0., 1.}, 1. / 6.};
    bl::Ball b_f = bil.Dynamic(b_i);
    CHECK(b_f.start_point.x == 6.);
    CHECK(b_f.start_point.y == 2.);
    CHECK(b_f.slope == doctest::Approx(1. / 6.));
  }
  SUBCASE("one bounce case") {
    bl::Biliard bil{6., 4., 2.8};
    bl::Ball b_i{{0., 2.}, 1. / 3.};
    bl::Ball b_f = bil.Dynamic(b_i);
    CHECK(b_f.start_point.x == 6.);
    CHECK(b_f.start_point.y == doctest::Approx(1.29032));
    CHECK(b_f.slope == doctest::Approx(-0.87096));
  }
  SUBCASE("one bounce case") {
    bl::Biliard bil{6., 4., 2.8};
    bl::Ball b_i{{0., -2.}, -1. / 3.};
    bl::Ball b_f = bil.Dynamic(b_i);
    CHECK(b_f.start_point.x == 6.);
    CHECK(b_f.start_point.y == doctest::Approx(-1.29032));
    CHECK(b_f.slope == doctest::Approx(0.87096));
  }
}
