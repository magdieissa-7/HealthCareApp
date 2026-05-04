#include <gtest/gtest.h>
#include "../src/core/ReminderManager.h"
#include "../src/core/Medication.h"

TEST(ReminderManagerTest, StartsEmpty) {
    ReminderManager rm;
    EXPECT_EQ(rm.size(), 0u);
}
TEST(ReminderManagerTest, AddIncreasesSize) {
    ReminderManager rm;
    rm.addMedication(Medication("Aspirin", "100mg", "08:00"));
    EXPECT_EQ(rm.size(), 1u);
}
TEST(ReminderManagerTest, GetMedicationsCorrect) {
    ReminderManager rm;
    rm.addMedication(Medication("Paracetamol", "500mg", "12:00"));
    ASSERT_EQ(rm.size(), 1u);
    EXPECT_EQ(rm.getMedications()[0].getName(), "Paracetamol");
}
TEST(ReminderManagerTest, RemoveDecreasesSize) {
    ReminderManager rm;
    rm.addMedication(Medication("Med A", "1mg", "09:00"));
    rm.addMedication(Medication("Med B", "2mg", "10:00"));
    EXPECT_TRUE(rm.removeMedication(0));
    EXPECT_EQ(rm.size(), 1u);
    EXPECT_EQ(rm.getMedications()[0].getName(), "Med B");
}
TEST(ReminderManagerTest, RemoveOutOfBoundsFalse) {
    ReminderManager rm;
    EXPECT_FALSE(rm.removeMedication(5));
}
TEST(ReminderManagerTest, ClearEmptiesAll) {
    ReminderManager rm;
    rm.addMedication(Medication("X", "1mg", "07:00"));
    rm.clear();
    EXPECT_EQ(rm.size(), 0u);
}
