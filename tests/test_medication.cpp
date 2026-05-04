#include <gtest/gtest.h>
#include "../src/core/Medication.h"

TEST(MedicationTest, DefaultConstructorEmptyFields) {
    Medication med;
    EXPECT_TRUE(med.getName().empty());
    EXPECT_TRUE(med.getDosage().empty());
    EXPECT_TRUE(med.getTime().empty());
}
TEST(MedicationTest, ParameterizedConstructorSetsFields) {
    Medication med("Insulin", "10mg", "08:00");
    EXPECT_EQ(med.getName(),   "Insulin");
    EXPECT_EQ(med.getDosage(), "10mg");
    EXPECT_EQ(med.getTime(),   "08:00");
}
TEST(MedicationTest, SpecialCharactersInName) {
    Medication med("Amoxicillin-500", "500mg", "07:30");
    EXPECT_EQ(med.getName(), "Amoxicillin-500");
}
