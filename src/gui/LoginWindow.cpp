#include "LoginWindow.h"
#include "DashboardWindow.h"
#include "../utils/Validator.h"

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("HealthCare App – Login");
    setMinimumSize(400, 300);
    setupUI();
}

void LoginWindow::setupUI()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    layout = new QVBoxLayout(centralWidget);

    titleLabel = new QLabel("HealthCare Login", centralWidget);
    QFont f = titleLabel->font();
    f.setPointSize(16);
    f.setBold(true);
    titleLabel->setFont(f);
    titleLabel->setAlignment(Qt::AlignCenter);

    usernameEdit = new QLineEdit(centralWidget);
    usernameEdit->setPlaceholderText("Username");

    passwordEdit = new QLineEdit(centralWidget);
    passwordEdit->setPlaceholderText("Password");
    passwordEdit->setEchoMode(QLineEdit::Password);

    loginButton = new QPushButton("Login", centralWidget);

    statusLabel = new QLabel("", centralWidget);
    statusLabel->setAlignment(Qt::AlignCenter);

    layout->addWidget(titleLabel);
    layout->addSpacing(10);
    layout->addWidget(usernameEdit);
    layout->addWidget(passwordEdit);
    layout->addWidget(loginButton);
    layout->addWidget(statusLabel);
    layout->addStretch();

    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
}

void LoginWindow::onLoginClicked()
{
    std::string username = usernameEdit->text().toStdString();
    std::string password = passwordEdit->text().toStdString();


    if (!Validator::isNotEmpty(username) || !Validator::isNotEmpty(password)) {
        statusLabel->setText("Username and password must not be empty.");
        return;
    }


    if (authManager.authenticate(username, password)) {
        emit loginSuccessful(QString::fromStdString(username));
        auto *dashboard = new DashboardWindow(QString::fromStdString(username));
        dashboard->show();
        close();
    } else {
        statusLabel->setText("Invalid credentials. Please try again.");
    }
}
