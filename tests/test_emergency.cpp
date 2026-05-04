#include <gtest/gtest.h>
#include "../src/core/EmergencyManager.h"

TEST(EmergencyManagerTest, DefaultContactEmpty) {
    EmergencyManager em;
    EXPECT_TRUE(em.getEmergencyContact().empty());
}
TEST(EmergencyManagerTest, SetAndGetContact) {
    EmergencyManager em;
    em.setEmergencyContact("01012345678");
    EXPECT_EQ(em.getEmergencyContact(), "01012345678");
}
TEST(EmergencyManagerTest, MessageNotEmpty) {
    EmergencyManager em;
    em.setEmergencyContact("01012345678");
    EXPECT_FALSE(em.createEmergencyMessage().empty());
}
TEST(EmergencyManagerTest, MessageContainsAlertWord) {
    EmergencyManager em;
    em.setEmergencyContact("doctor@hospital.com");
    EXPECT_NE(em.createEmergencyMessage().find("Emergency"), std::string::npos);
}
TEST(EmergencyManagerTest, MessageContainsContact) {
    EmergencyManager em;
    em.setEmergencyContact("01099999999");
    EXPECT_NE(em.createEmergencyMessage().find("01099999999"), std::string::npos);
}
