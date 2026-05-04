#include <gtest/gtest.h>
#include "../src/core/User.h"
#include "../src/core/AuthManager.h"

TEST(UserTest, DefaultConstructorEmpty) {
    User u;
    EXPECT_TRUE(u.getUsername().empty());
    EXPECT_TRUE(u.getPassword().empty());
}
TEST(UserTest, SetGetUsername) { User u; u.setUsername("alice"); EXPECT_EQ(u.getUsername(), "alice"); }
TEST(UserTest, SetGetPassword) { User u; u.setPassword("pass"); EXPECT_EQ(u.getPassword(), "pass"); }
TEST(UserTest, SetGetCondition) { User u; u.setCondition("Diabetes"); EXPECT_EQ(u.getCondition(), "Diabetes"); }
TEST(UserTest, SetGetContact) { User u; u.setEmergencyContact("01099"); EXPECT_EQ(u.getEmergencyContact(), "01099"); }
