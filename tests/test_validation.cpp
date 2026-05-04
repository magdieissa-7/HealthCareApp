#include <gtest/gtest.h>
#include "../src/utils/Validator.h"

TEST(ValidatorTest, NotEmptyReturnsTrue)       { EXPECT_TRUE(Validator::isNotEmpty("medicine")); }
TEST(ValidatorTest, EmptyReturnsFalse)          { EXPECT_FALSE(Validator::isNotEmpty("")); }
TEST(ValidatorTest, WhitespaceIsNotEmpty)       { EXPECT_TRUE(Validator::isNotEmpty("   ")); }
TEST(ValidatorTest, ValidTimeReturnsTrue)       { EXPECT_TRUE(Validator::isValidTime("09:30")); }
TEST(ValidatorTest, MidnightIsValid)            { EXPECT_TRUE(Validator::isValidTime("00:00")); }
TEST(ValidatorTest, LastMinuteIsValid)          { EXPECT_TRUE(Validator::isValidTime("23:59")); }
TEST(ValidatorTest, InvalidHourReturnsFalse)    { EXPECT_FALSE(Validator::isValidTime("25:00")); }
TEST(ValidatorTest, InvalidMinuteReturnsFalse)  { EXPECT_FALSE(Validator::isValidTime("12:60")); }
TEST(ValidatorTest, MissingLeadingZeroFails)    { EXPECT_FALSE(Validator::isValidTime("9:30")); }
TEST(ValidatorTest, EmptyTimeReturnsFalse)      { EXPECT_FALSE(Validator::isValidTime("")); }
