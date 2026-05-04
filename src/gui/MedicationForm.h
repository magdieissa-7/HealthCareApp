#ifndef MEDICATIONFORM_H
#define MEDICATIONFORM_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include "../core/ReminderManager.h"

class MedicationForm : public QDialog {
    Q_OBJECT
public:
    explicit MedicationForm(ReminderManager &manager, QWidget *parent = nullptr);
    ~MedicationForm() override = default;
private slots:
    void onSaveClicked();
private:
    void setupUI();
    ReminderManager &reminderManager;
    QFormLayout *formLayout;
    QLineEdit   *nameEdit;
    QLineEdit   *dosageEdit;
    QLineEdit   *timeEdit;
    QPushButton *saveButton;
    QPushButton *cancelButton;
    QLabel      *statusLabel;
};

#endif // MEDICATIONFORM_H
