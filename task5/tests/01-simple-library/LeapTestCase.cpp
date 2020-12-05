//
// Created by akhtyamovpavel on 5/1/20.
//

#include "LeapTestCase.h"

#include <Functions.h>

static constexpr auto kLeapYears = {1600, 2000, 2020, 2016, 2024, 2028, 2032};
static constexpr auto kNonLeapYears = {1700, 1800, 1900, 2100, 2200, 2300, 2021, 1999, 10, 2030};


TEST_F(LeapTestCase, InvalidArgument) {
  ASSERT_THROW(IsLeap(-1), std::invalid_argument);
  ASSERT_THROW(IsLeap(0), std::invalid_argument);
  ASSERT_NO_THROW(IsLeap(10));
}

TEST_F(LeapTestCase, LeapYears) {
  for(const auto y : kLeapYears) {
    ASSERT_TRUE(IsLeap(y));
  }
}

TEST_F(LeapTestCase, NonLeapYears) {
  for(const auto y : kNonLeapYears) {
    ASSERT_FALSE(IsLeap(y));
  }
}
