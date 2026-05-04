#ifndef REMINDER_MANAGER_H
#define REMINDER_MANAGER_H

#include "Medication.h"
#include <vector>

class ReminderManager {
public:
    void addMedication(const Medication &medication);
    bool removeMedication(std::size_t index);          // new: remove by index
    std::vector<Medication> getMedications() const;
    std::size_t size() const;
    void clear();

private:
    std::vector<Medication> medications;
};

#endif // REMINDER_MANAGER_H
