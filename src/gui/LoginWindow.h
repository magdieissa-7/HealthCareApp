#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include "../core/AuthManager.h"

class LoginWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow() override = default;

signals:
    void loginSuccessful(const QString &username);

private slots:
    void onLoginClicked();

private:
    void setupUI();

    // AuthManager holds the business logic – not the GUI
    AuthManager authManager;

    QWidget     *centralWidget;
    QVBoxLayout *layout;
    QLabel      *titleLabel;
    QLineEdit   *usernameEdit;
    QLineEdit   *passwordEdit;
    QPushButton *loginButton;
    QLabel      *statusLabel;
};

#endif // LOGINWINDOW_H
