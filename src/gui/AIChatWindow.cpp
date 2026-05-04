#include "AIChatWindow.h"
#include <cstdlib>

AIChatWindow::AIChatWindow(const std::string &apiKey, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("AI Health Assistant");
    setMinimumSize(550, 500);


    assistant = std::make_unique<AIHealthAssistant>(apiKey);

    setupUI();


    connect(this, &AIChatWindow::responseReady,
            this, &AIChatWindow::onResponseReceived,
            Qt::QueuedConnection);

    appendMessage("Assistant",
        "Hello! I am your AI Health Assistant powered by Claude.\n"
        "You can ask me about medications, symptoms, or general health questions.\n"
        "Remember: always consult your doctor for medical decisions.",
        "#0066cc");
}

void AIChatWindow::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *titleLabel = new QLabel("AI Health Assistant", this);
    QFont f = titleLabel->font();
    f.setPointSize(14);
    f.setBold(true);
    titleLabel->setFont(f);
    titleLabel->setAlignment(Qt::AlignCenter);

    chatHistory = new QTextEdit(this);
    chatHistory->setReadOnly(true);
    chatHistory->setStyleSheet(
        "QTextEdit { background-color: #f9f9f9; border: 1px solid #ddd; border-radius: 6px; }");

    inputField = new QLineEdit(this);
    inputField->setPlaceholderText("Ask a health question...");
    inputField->setMinimumHeight(36);

    sendButton = new QPushButton("Send", this);
    sendButton->setMinimumHeight(36);
    sendButton->setStyleSheet(
        "QPushButton { background-color: #0066cc; color: white; "
        "border-radius: 6px; font-weight: bold; }"
        "QPushButton:hover { background-color: #0052a3; }"
        "QPushButton:disabled { background-color: #aaa; }");

    statusLabel = new QLabel("", this);
    statusLabel->setAlignment(Qt::AlignCenter);

    QHBoxLayout *inputLayout = new QHBoxLayout();
    inputLayout->addWidget(inputField);
    inputLayout->addWidget(sendButton);

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(chatHistory);
    mainLayout->addWidget(statusLabel);
    mainLayout->addLayout(inputLayout);

    connect(sendButton,  &QPushButton::clicked,    this, &AIChatWindow::onSendClicked);
    connect(inputField,  &QLineEdit::returnPressed, this, &AIChatWindow::onSendClicked);
}

void AIChatWindow::onSendClicked()
{
    QString question = inputField->text().trimmed();
    if (question.isEmpty()) return;

    appendMessage("You", question, "#333333");
    inputField->clear();
    sendButton->setEnabled(false);
    statusLabel->setText("AI is thinking...");


    assistant->ask(question.toStdString(),
        [this](const std::string &answer, const std::string &error) {
            emit responseReady(
                QString::fromStdString(answer),
                QString::fromStdString(error));
        });
}

void AIChatWindow::onResponseReceived(const QString &answer, const QString &error)
{
    sendButton->setEnabled(true);
    statusLabel->setText("");

    if (!error.isEmpty()) {
        appendMessage("Error", error, "#cc0000");
    } else {
        appendMessage("Assistant", answer, "#0066cc");
    }
}

void AIChatWindow::appendMessage(const QString &sender,
                                  const QString &text,
                                  const QString &color)
{
    chatHistory->append(
        QString("<b style='color:%1'>%2:</b> %3<br>")
            .arg(color, sender, text.toHtmlEscaped()));
}
