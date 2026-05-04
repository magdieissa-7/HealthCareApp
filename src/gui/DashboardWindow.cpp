#include "DashboardWindow.h"
#include "MedicationForm.h"
#include "EmergencyWindow.h"
#include "AIChatWindow.h"
#include "../network/JsonMessage.h"
#include <cstdlib>

DashboardWindow::DashboardWindow(const QString &username, QWidget *parent)
    : QMainWindow(parent), username(username)
{
    setWindowTitle("HealthCare Dashboard – " + username);
    setMinimumSize(650, 500);

    networkClient = std::make_unique<NetworkClient>("127.0.0.1", "9000");
    setupUI();


    std::string loginMsg = JsonMessage::createLoginMessage(username.toStdString());
    networkClient->sendMessage(loginMsg,
        [this](const std::string &response, const std::string &error) {
            QString status = error.empty()
                ? "Server: " + QString::fromStdString(JsonMessage::getField(response, "status"))
                : "Server offline (local mode)";
            QMetaObject::invokeMethod(this, [this, status]() {
                networkStatusLabel->setText(status);
            }, Qt::QueuedConnection);
        });
}

void DashboardWindow::setupUI()
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    welcomeLabel = new QLabel("Welcome, " + username + "!", central);
    QFont f = welcomeLabel->font();
    f.setPointSize(14); f.setBold(true);
    welcomeLabel->setFont(f);
    welcomeLabel->setAlignment(Qt::AlignCenter);

    networkStatusLabel = new QLabel("Connecting to server...", central);
    networkStatusLabel->setAlignment(Qt::AlignCenter);


    tabWidget = new QTabWidget(central);

    QWidget *medTab = new QWidget();
    setupMedicationTab(medTab);
    tabWidget->addTab(medTab, "Medications");

    QWidget *emergencyTab = new QWidget();
    setupEmergencyTab(emergencyTab);
    tabWidget->addTab(emergencyTab, "Emergency");

    QWidget *aiTab = new QWidget();
    setupAITab(aiTab);
    tabWidget->addTab(aiTab, "AI Assistant");

    mainLayout->addWidget(welcomeLabel);
    mainLayout->addWidget(networkStatusLabel);
    mainLayout->addWidget(tabWidget);
}

void DashboardWindow::setupMedicationTab(QWidget *tab)
{
    QVBoxLayout *layout = new QVBoxLayout(tab);
    medicationList = new QListWidget(tab);
    addMedButton   = new QPushButton("Add Medication", tab);
    layout->addWidget(new QLabel("Your Medications:"));
    layout->addWidget(medicationList);
    layout->addWidget(addMedButton);
    connect(addMedButton, &QPushButton::clicked,
            this, &DashboardWindow::onAddMedicationClicked);
}

void DashboardWindow::setupEmergencyTab(QWidget *tab)
{
    QVBoxLayout *layout = new QVBoxLayout(tab);
    emergencyButton = new QPushButton("Send Emergency Alert", tab);
    emergencyButton->setStyleSheet(
        "background-color: red; color: white; font-weight: bold; padding: 10px;");
    layout->addWidget(new QLabel("Emergency Actions:"));
    layout->addStretch();
    layout->addWidget(emergencyButton);
    layout->addStretch();
    connect(emergencyButton, &QPushButton::clicked,
            this, &DashboardWindow::onEmergencyClicked);
}

void DashboardWindow::setupAITab(QWidget *tab)
{
    QVBoxLayout *layout = new QVBoxLayout(tab);

    QLabel *desc = new QLabel(
        "Ask our AI Health Assistant any health-related questions.\n"
        "Powered by Claude (Anthropic).", tab);
    desc->setAlignment(Qt::AlignCenter);
    desc->setWordWrap(true);

    aiChatButton = new QPushButton("Open AI Health Assistant", tab);
    aiChatButton->setMinimumHeight(44);
    aiChatButton->setStyleSheet(
        "QPushButton { background-color: #0066cc; color: white; "
        "font-weight: bold; border-radius: 8px; font-size: 14px; }"
        "QPushButton:hover { background-color: #0052a3; }");

    layout->addStretch();
    layout->addWidget(desc);
    layout->addSpacing(20);
    layout->addWidget(aiChatButton);
    layout->addStretch();

    connect(aiChatButton, &QPushButton::clicked,
            this, &DashboardWindow::onAIChatClicked);
}

void DashboardWindow::onAddMedicationClicked()
{
    MedicationForm form(reminderManager, this);
    form.exec();
    refreshMedicationList();

    if (!reminderManager.getMedications().empty()) {
        const auto &med = reminderManager.getMedications().back();
        std::string msg = JsonMessage::createMedicationMessage(
            med.getName(), med.getDosage(), med.getTime());
        networkClient->sendMessage(msg,
            [this](const std::string &, const std::string &error) {
                if (!error.empty())
                    QMetaObject::invokeMethod(this, [this]() {
                        networkStatusLabel->setText("Medication saved locally");
                    }, Qt::QueuedConnection);
            });
    }
}

void DashboardWindow::onEmergencyClicked()
{
    EmergencyWindow *ew = new EmergencyWindow(emergencyManager, this);
    ew->exec();

    if (!emergencyManager.getEmergencyContact().empty()) {
        std::string msg = JsonMessage::createEmergencyMessage(
            emergencyManager.getEmergencyContact());
        networkClient->sendMessage(msg,
            [this](const std::string &, const std::string &error) {
                QString status = error.empty()
                    ? "Emergency alert sent to server"
                    : "Alert saved locally (server offline)";
                QMetaObject::invokeMethod(this, [this, status]() {
                    networkStatusLabel->setText(status);
                }, Qt::QueuedConnection);
            });
    }
}

void DashboardWindow::onAIChatClicked()
{

    const char *key = std::getenv("ANTHROPIC_API_KEY");
    std::string apiKey = key ? key : "";

    if (apiKey.empty()) {
        networkStatusLabel->setText(
            "Set ANTHROPIC_API_KEY environment variable to use AI Assistant.");
        return;
    }

    AIChatWindow *chat = new AIChatWindow(apiKey, this);
    chat->exec();
}

void DashboardWindow::refreshMedicationList()
{
    medicationList->clear();
    for (const auto &med : reminderManager.getMedications()) {
        QString entry = QString::fromStdString(med.getName()) + " – " +
                        QString::fromStdString(med.getDosage()) + " at " +
                        QString::fromStdString(med.getTime());
        medicationList->addItem(entry);
    }
}
