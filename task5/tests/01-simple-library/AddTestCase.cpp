//
// Created by akhtyamovpavel on 5/1/20.
//

#include "AddTestCase.h"
#include "Functions.h"

#include <limits>
#include <random>

static constexpr auto kArray = {1, 10, -100, 1000, 0, 1124, 178312, 1892891289, -12412, -21812, 189, 871287};
static std::mt19937 random_generator(12812);


static int MaybeMakeNegative(int value) noexcept {
  static std::mt19937 g(12);
  return (g() % 2 == 0 ? value : -value);
}


TEST_F(AddTestCase, AddZero) {
  for(const auto value : kArray) {
    ASSERT_EQ(Add(value, 0), value);
    ASSERT_EQ(Add(0, value), value);
  }
}

TEST_F(AddTestCase, AddSameOpposite) {
  for(const auto value : kArray) {
    ASSERT_EQ(Add(value, -value), 0);
  }
}

TEST_F(AddTestCase, RandomNumbers) {
  for(int i = 0; i < 10'000; ++i) {
    auto a = MaybeMakeNegative(random_generator() % (uint32_t)std::numeric_limits<int>::max());
    auto b = MaybeMakeNegative(random_generator() % (uint32_t)std::numeric_limits<int>::max());

    ASSERT_EQ(Add(a, b), a + b);
  }
}
