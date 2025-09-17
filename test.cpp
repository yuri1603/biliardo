#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <vector>

#include "biliard.hpp"
#include "doctest.h"
#include "statistics.hpp"

TEST_CASE("Testing intersect function") {
  SUBCASE("Intersection with a horizontal path, first constructor") {
    bl::Line l1{0., 1.};
    bl::Line l2{1., 0.};
    bl::Point point = bl::intersect(l1, l2);
    CHECK(point.x == 1.);
    CHECK(point.y == 1.);
  }
  SUBCASE("Intersection with a horizontal path, second constructor") {
    bl::Line l1{{0., 1.}, {3., 1.}};
    bl::Line l2{{1., 1.}, {0., 0.}};
    bl::Point point = bl::intersect(l1, l2);
    CHECK(point.x == 1.);
    CHECK(point.y == 1.);
  }
  SUBCASE("Intersection with a horizontal path, third constructor") {
    bl::Line l1{{0., 1.}, 0.};
    bl::Line l2{{1., 1.}, 1.};
    bl::Point point = bl::intersect(l1, l2);
    CHECK(point.x == 1.);
    CHECK(point.y == 1.);
  }
  SUBCASE("interception between two path, first constructor") {
    bl::Line l1{1., 3.};
    bl::Line l2{2., 0.};
    bl::Point point = bl::intersect(l1, l2);
    CHECK(point.x == 3.);
    CHECK(point.y == 6.);
  }
  SUBCASE("interception between two path,second constructor") {
    bl::Line l1{{0., 3.}, {-3., 0.}};
    bl::Line l2{{0., 0.}, {1., 2.}};
    bl::Point point = bl::intersect(l1, l2);
    CHECK(point.x == 3.);
    CHECK(point.y == 6.);
  }
  SUBCASE("interception between two path, third constructor") {
    bl::Line l1{{0., 3.}, 1.};
    bl::Line l2{{0., 0.}, 2};
    bl::Point point = bl::intersect(l1, l2);
    CHECK(point.x == 3.);
    CHECK(point.y == 6.);
  }
  SUBCASE("Collision point between two parallel paths") {
    bl::Line l1{2., 3.};
    bl::Line l2{2., 0.};
    bl::Point point = bl::intersect(l1, l2);
    CHECK(point.x == -1.);
    CHECK(point.y == 0.);
  }
}

TEST_CASE("Testing collide_first function") {
  bl::Line l2{-0.25, 5.};
  bl::Line l3{0.25, -5.};

  SUBCASE("first point negative x coordinate") {
    bl::Line l1{-0.5, 4.};
    bl::Point p = bl::collide_first(l1, l2, l3);
    CHECK(p.x == 12.);
    CHECK(p.y == -2.);
  }

  SUBCASE("second point negative x coordinate") {
    bl::Line l1{-0.5, 4.};
    bl::Point p = bl::collide_first(l1, l2, l3);
    CHECK(p.x == 12.);
    CHECK(p.y == -2.);
  }

  SUBCASE("both points have positive x coordinate") {
    bl::Line l1{-0.15, 4.};
    bl::Point p = bl::collide_first(l1, l2, l3);
    CHECK(p.x == 10);
    CHECK(p.y == 2.5);
  }
}

TEST_CASE("Testing bounce method") {
  SUBCASE("standard situation") {
    bl::Biliard bil(4., 3., 1);
    bl::Line l1{2., -7.};
    bl::Line l2{0.5, -3.};
    bl::Line l3 = bounce(l1, l2);
    CHECK(l3.slope_ == doctest::Approx(-2.0 / 11.0));
    CHECK(l3.y_intercept_ == doctest::Approx(-13.0 / 11.0));
  }
}

TEST_CASE("Testing compute_final_position") {
  SUBCASE("Testing compute_final_position with negative slope upper cushion") {
    bl::Biliard bil{6., 4., 2.8};
    SUBCASE("no bouncing case, right exit") {
      bl::Ball b{1., 0.16515};
      bil.compute_final_position(b);
      CHECK(b.y == doctest::Approx(2.));
      CHECK(b.angle == doctest::Approx(0.16515));
    }

    SUBCASE("one bounce on the upper cushion case, right exit") {
      bl::Ball b{2., 0.32175};
      bil.compute_final_position(b);
      CHECK(b.y == doctest::Approx(1.29032));
      CHECK(b.angle == doctest::Approx(-0.71654));
    }
    SUBCASE("one bounce on the lower cushion case, right exit") {
      bl::Ball b{-2., -0.32175};
      bil.compute_final_position(b);
      CHECK(b.y == doctest::Approx(-1.29032));
      CHECK(b.angle == doctest::Approx(0.71654));
    }
    SUBCASE("two bounce, left exit") {
      bl::Ball b{-2.02304, 1.19294};
      bil.compute_final_position(b);
      CHECK(b.y == 0.);
      CHECK(b.angle == 2.);
    }
    SUBCASE("two bounce, right exit") {
      bl::Biliard bil2{10., 4., 2.8};
      bl::Ball b{-2.6742, 0.81480};
      bil2.compute_final_position(b);
      CHECK(b.y == doctest::Approx(-0.062876));
      CHECK(b.angle == doctest::Approx(1.29251));
    }
  }

  SUBCASE(
      "Testing compute_final_position method with positive slope upper "
      "cushions") {
    bl::Biliard bil{6., 2.8, 4.};

    SUBCASE("no bouncing case, right exit") {
      bl::Ball b{1., 0.16515};
      bil.compute_final_position(b);
      CHECK(b.y == doctest::Approx(2.));
      CHECK(b.angle == doctest::Approx(0.16515));
    }
    SUBCASE("no bouncing case, right exit") {
      bl::Ball b{0., 0.};
      bil.compute_final_position(b);
      CHECK(b.y == 0.);
      CHECK(b.angle == 0.);
    }
    SUBCASE("one bounce on the upper cushuion case, right exit") {
      bl::Ball b{1., 0.7};
      bil.compute_final_position(b);
      CHECK(b.y == doctest::Approx(2.35311));
      CHECK(b.angle == doctest::Approx(-0.305209));
    }
    SUBCASE("one bounce on the lower bounce case, right exit") {
      bl::Ball b{-1., -0.7};
      bil.compute_final_position(b);
      CHECK(b.y == doctest::Approx(-2.35311));
      CHECK(b.angle == doctest::Approx(0.305209));
    }

    SUBCASE("two bounce, right exit") {
      bl::Ball b{1, 1.5};
      bil.compute_final_position(b);
      CHECK(b.y == doctest::Approx(-1.1228));
      CHECK(b.angle == doctest::Approx(0.710418));
    }
  }

  SUBCASE("Testing compute_final_position with horizontal cushions") {
    bl::Biliard bil{10., 4., 4.};

    SUBCASE("no bouncing case, right exit") {
      bl::Ball b_i{1., 0.16515};
      bil.compute_final_position(b_i);
      CHECK(b_i.y == doctest::Approx(2.66668));
      CHECK(b_i.angle == doctest::Approx(0.16515));
    }

    SUBCASE("one bounce on the upper cushuion case, right exit") {
      bl::Ball b_i{1., 0.4};
      bil.compute_final_position(b_i);
      CHECK(b_i.y == doctest::Approx(2.77207));
      CHECK(b_i.angle == doctest::Approx(-0.4));
    }
    SUBCASE("one bounce on the lower bounce case, right exit") {
      bl::Ball b_i{-1., -0.4};
      bil.compute_final_position(b_i);
      CHECK(b_i.y == doctest::Approx(-2.77207));
      CHECK(b_i.angle == doctest::Approx(0.4));
    }

    SUBCASE("two bounce, right exit") {
      bl::Ball b_i{-1., -1.};
      bil.compute_final_position(b_i);
      CHECK(b_i.y == doctest::Approx(-0.574077));
      CHECK(b_i.angle == doctest::Approx(-1.));
    }
  }
}

TEST_CASE("Testing the split_for_stats method") {
  bl::Biliard bil(6., 4., 2.8);
  std::vector<bl::Ball> balls{
      {1., 0.16515}, {2., 0.32175}, {-2., -0.32175}, {-2.02304, 1.19294}};
  bl::Samples stat_vecs = bil.split_for_stats(balls);
  CHECK(stat_vecs.ball_angles[0] == doctest::Approx(0.16515));
  CHECK(stat_vecs.ball_ys[0] == doctest::Approx(2.));
  CHECK(stat_vecs.ball_angles[1] == doctest::Approx(-0.71654));
  CHECK(stat_vecs.ball_ys[1] == doctest::Approx(1.29032));
  CHECK(stat_vecs.ball_angles[2] == doctest::Approx(0.71654));
  CHECK(stat_vecs.ball_ys[2] == doctest::Approx(-1.29032));
  CHECK(stat_vecs.ball_angles.size() == 3);
  CHECK(stat_vecs.ball_ys.size() == 3);
}

TEST_CASE("Testing evaluate_statistics method") {
  SUBCASE("ensemble size is 5") {
    std::vector<double> vec{5., 2., -3., -1., 0.};
    bl::Statistics result = bl::evaluate_statistics(vec);
    CHECK(result.mean == doctest::Approx(0.6).epsilon(0.001));
    CHECK(result.std_dev == doctest::Approx(3.0496).epsilon(0.001));
    CHECK(result.skewness == doctest::Approx(0.2606).epsilon(0.001));
    CHECK(result.kurtosis == doctest::Approx(1.2794).epsilon(0.001));
  }
}
