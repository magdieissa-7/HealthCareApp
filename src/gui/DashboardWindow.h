#ifndef DASHBOARDWINDOW_H
#define DASHBOARDWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <memory>
#include "../core/ReminderManager.h"
#include "../core/EmergencyManager.h"
#include "../network/NetworkClient.h"

class DashboardWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit DashboardWindow(const QString &username, QWidget *parent = nullptr);
    ~DashboardWindow() override = default;

private slots:
    void onAddMedicationClicked();
    void onEmergencyClicked();
    void onAIChatClicked();
    void refreshMedicationList();

private:
    void setupUI();
    void setupMedicationTab(QWidget *tab);
    void setupEmergencyTab(QWidget *tab);
    void setupAITab(QWidget *tab);

    QString          username;
    QTabWidget      *tabWidget;
    QListWidget     *medicationList;
    QPushButton     *addMedButton;
    QPushButton     *emergencyButton;
    QPushButton     *aiChatButton;
    QLabel          *welcomeLabel;
    QLabel          *networkStatusLabel;

    ReminderManager  reminderManager;
    EmergencyManager emergencyManager;

    std::unique_ptr<NetworkClient> networkClient;
};

#endif // DASHBOARDWINDOW_H
