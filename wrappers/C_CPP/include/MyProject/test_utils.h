#pragma once

#include <iostream>
#include <cmath>

/// These are macros created with the same signature as those in GoogleTest

#define EXPECT_EQ(expected, actual) \
  do { \
    if ((expected) != (actual)) { \
      std::cout << "Error: Expected " << (expected) << " to be equal to " << (actual) << " in " << __func__ << std::endl; \
    } \
  } while (0)

#define EXPECT_NE(expected, actual) \
  do { \
    if ((expected) == (actual)) { \
      std::cout << "Error: Expected " << (expected) << " to not be equal to " << (actual) << " in " << __func__ << std::endl; \
    } \
  } while (0)

#define EXPECT_NEAR(expected, actual, abs_tol) \
  do { \
    if (std::abs((expected) - (actual)) > (abs_tol)) { \
      std::cout << "Error: Expected " << (expected) << " to be within " << (abs_tol) << " of " << (actual) << " in " << __func__ << std::endl; \
    } \
  } while (0)

#define EXPECT_NEAR_REL(expected, actual, rel_tol) \
  EXPECT_NEAR(expected, actual, std::abs((expected) * (rel_tol)))
