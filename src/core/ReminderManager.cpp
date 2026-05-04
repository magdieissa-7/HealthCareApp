#include "ReminderManager.h"

void ReminderManager::addMedication(const Medication &medication)
{
    medications.push_back(medication);
}

bool ReminderManager::removeMedication(std::size_t index)
{
    if (index >= medications.size()) return false;
    medications.erase(medications.begin() + static_cast<std::ptrdiff_t>(index));
    return true;
}

std::vector<Medication> ReminderManager::getMedications() const
{
    return medications;
}

std::size_t ReminderManager::size() const
{
    return medications.size();
}

void ReminderManager::clear()
{
    medications.clear();
}
