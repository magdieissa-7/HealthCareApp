#ifndef EMERGENCYWINDOW_H
#define EMERGENCYWINDOW_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include "../core/EmergencyManager.h"

class EmergencyWindow : public QDialog {
    Q_OBJECT
public:
    explicit EmergencyWindow(EmergencyManager &manager, QWidget *parent = nullptr);
    ~EmergencyWindow() override = default;
private slots:
    void onSendAlertClicked();
private:
    void setupUI();
    EmergencyManager &emergencyManager;
    QLineEdit   *contactEdit;
    QPushButton *sendButton;
    QLabel      *statusLabel;
};

#endif // EMERGENCYWINDOW_H
