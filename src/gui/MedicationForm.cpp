#include "MedicationForm.h"
#include "../utils/Validator.h"
#include "../core/Medication.h"

MedicationForm::MedicationForm(ReminderManager &manager, QWidget *parent)
    : QDialog(parent), reminderManager(manager)
{
    setWindowTitle("Add Medication");
    setMinimumWidth(350);
    setupUI();
}

void MedicationForm::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    formLayout = new QFormLayout();
    nameEdit   = new QLineEdit(this);
    dosageEdit = new QLineEdit(this);
    timeEdit   = new QLineEdit(this);
    timeEdit->setPlaceholderText("HH:MM (24-hour)");
    formLayout->addRow("Medication Name:", nameEdit);
    formLayout->addRow("Dosage:",          dosageEdit);
    formLayout->addRow("Time:",            timeEdit);
    saveButton   = new QPushButton("Save",   this);
    cancelButton = new QPushButton("Cancel", this);
    statusLabel  = new QLabel("", this);
    statusLabel->setAlignment(Qt::AlignCenter);
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(saveButton);
    btnLayout->addWidget(cancelButton);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(statusLabel);
    mainLayout->addLayout(btnLayout);
    connect(saveButton,   &QPushButton::clicked, this, &MedicationForm::onSaveClicked);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void MedicationForm::onSaveClicked()
{
    std::string name   = nameEdit->text().toStdString();
    std::string dosage = dosageEdit->text().toStdString();
    std::string time   = timeEdit->text().toStdString();
    if (!Validator::isNotEmpty(name) || !Validator::isNotEmpty(dosage)) {
        statusLabel->setText("Name and dosage must not be empty.");
        return;
    }
    if (!Validator::isValidTime(time)) {
        statusLabel->setText("Time must be in HH:MM format (e.g. 09:30).");
        return;
    }
    reminderManager.addMedication(Medication(name, dosage, time));
    accept();
}
