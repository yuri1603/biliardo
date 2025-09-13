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
    bl::Path path = Bounce(r1, r2);
    CHECK(path.slope == doctest::Approx(-2.0 / 11.0));
    CHECK(path.y_intercept == doctest::Approx(-13.0 / 11.0));
  }
}

TEST_CASE("Testing dynamic method") {
  SUBCASE("no bouncing case, right exit") {
    bl::Biliard bil{6., 4., 2.8};
    bl::Ball b_i{1., 0.16515};
    bil.Dynamic(b_i);
    CHECK(b_i.y_coord == doctest::Approx(2.));
    CHECK(b_i.angle == doctest::Approx(0.16515));
  }
  SUBCASE("one bounce on the upper cushuion case, right exit") {
    bl::Biliard bil{6., 4., 2.8};
    bl::Ball b_i{2., 0.32175};
    bil.Dynamic(b_i);
    CHECK(b_i.y_coord == doctest::Approx(1.29032));
    CHECK(b_i.angle == doctest::Approx(-0.71654));
  }
  SUBCASE("one bounce on the lower bounce case, right exit") {
    bl::Biliard bil{6., 4., 2.8};
    bl::Ball b_i{-2., -0.32175};
    bil.Dynamic(b_i);
    CHECK(b_i.y_coord == doctest::Approx(-1.29032));
    CHECK(b_i.angle == doctest::Approx(0.71654));
  }
  SUBCASE("two bounce, left exit") {
    bl::Biliard bil{6., 4., 2.8};
    bl::Ball b_i{-2.02304, 1.19294};
    bil.Dynamic(b_i);
    CHECK(b_i.y_coord == 0.);
    CHECK(b_i.angle == 0.);
  }
  SUBCASE("two bounce, right exit") {
    bl::Biliard bil{10., 4., 2.8};
    bl::Ball b_i{-2.6742, 0.81480};
    bil.Dynamic(b_i);
    CHECK(b_i.y_coord == doctest::Approx(-0.062876));
    CHECK(b_i.angle == doctest::Approx(1.29251));
  }
}

TEST_CASE("Testing the in_to_fin_balls method") {
  bl::Biliard bil(6., 4., 2.8);
  std::vector<bl::Ball> balls{
      {1., 0.16515}, {2., 0.32175}, {-2., -0.32175}, {-2.02304, 1.19294}};
  bl::Sample stat_vecs = bil.in_to_fin_balls(balls);
  CHECK(stat_vecs.angles[0] == doctest::Approx(0.16515));
  CHECK(stat_vecs.y_coord[0] == doctest::Approx(2.));
  CHECK(stat_vecs.angles[1] == doctest::Approx(-0.71654));
  CHECK(stat_vecs.y_coord[1] == doctest::Approx(1.29032));
  CHECK(stat_vecs.angles[2] == doctest::Approx(0.71654));
  CHECK(stat_vecs.y_coord[2] == doctest::Approx(-1.29032));
  CHECK(stat_vecs.angles.size() == 3);
  CHECK(stat_vecs.y_coord.size() == 3);
}
