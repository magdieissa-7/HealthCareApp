#ifndef MEDICATION_H
#define MEDICATION_H
#include <string>

class Medication {
public:
    Medication();
    Medication(std::string name, std::string dosage, std::string time);
    std::string getName() const;
    std::string getDosage() const;
    std::string getTime() const;

private:
    std::string name;
    std::string dosage;
    std::string time;
};

#endif // MEDICATION_H
