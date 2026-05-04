#include "Medication.h"
Medication::Medication() {}
Medication::Medication(std::string name, std::string dosage, std::string time)
    : name(name), dosage(dosage), time(time) {}
std::string Medication::getName() const { return name; }
std::string Medication::getDosage() const { return dosage; }
std::string Medication::getTime() const { return time; }
