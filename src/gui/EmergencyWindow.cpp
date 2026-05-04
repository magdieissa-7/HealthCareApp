#include "EmergencyWindow.h"
#include "../utils/Validator.h"

EmergencyWindow::EmergencyWindow(EmergencyManager &manager, QWidget *parent)
    : QDialog(parent), emergencyManager(manager)
{
    setWindowTitle("Emergency Alert");
    setMinimumWidth(380);
    setupUI();
}

void EmergencyWindow::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *infoLabel = new QLabel(
        "Enter emergency contact and press Send Alert.", this);
    infoLabel->setWordWrap(true);
    contactEdit = new QLineEdit(this);
    contactEdit->setPlaceholderText("Emergency contact phone / email");
    sendButton  = new QPushButton("Send Alert", this);
    sendButton->setStyleSheet(
        "background-color: red; color: white; font-weight: bold;");
    statusLabel = new QLabel("", this);
    statusLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(infoLabel);
    layout->addWidget(contactEdit);
    layout->addWidget(statusLabel);
    layout->addWidget(sendButton);
    connect(sendButton, &QPushButton::clicked,
            this, &EmergencyWindow::onSendAlertClicked);
}

void EmergencyWindow::onSendAlertClicked()
{
    std::string contact = contactEdit->text().toStdString();
    if (!Validator::isNotEmpty(contact)) {
        statusLabel->setText("Please enter an emergency contact.");
        return;
    }
    emergencyManager.setEmergencyContact(contact);
    std::string msg = emergencyManager.createEmergencyMessage();
    statusLabel->setText(QString::fromStdString("Sent: " + msg));
}
