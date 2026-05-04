#ifndef AI_CHAT_WINDOW_H
#define AI_CHAT_WINDOW_H

#include <QDialog>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <memory>
#include "../core/AIHealthAssistant.h"

// AIChatWindow is the 5th screen in the app.
// It provides an LLM-powered AI health assistant chat interface.
// Business logic (API calls) lives in AIHealthAssistant, NOT here.
class AIChatWindow : public QDialog {
    Q_OBJECT

public:
    explicit AIChatWindow(const std::string &apiKey, QWidget *parent = nullptr);
    ~AIChatWindow() override = default;

private slots:
    void onSendClicked();
    void onResponseReceived(const QString &answer, const QString &error);

signals:
    // Used to safely update GUI from background thread
    void responseReady(const QString &answer, const QString &error);

private:
    void setupUI();
    void appendMessage(const QString &sender, const QString &text, const QString &color);

    std::unique_ptr<AIHealthAssistant> assistant;

    QTextEdit   *chatHistory;
    QLineEdit   *inputField;
    QPushButton *sendButton;
    QLabel      *statusLabel;
};

#endif // AI_CHAT_WINDOW_H
