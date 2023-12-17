#pragma once
#include "board.h"
#include <QWidget>
#include <QLabel>
#include <QLCDNumber>
#include <QPushButton>
#include <QTimer>
#include <QKeyEvent>
#include <QGridLayout>
#include <QApplication>
#include <QMainWindow>
#include <QDialog>

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();
    void keyPressEvent(QKeyEvent* event) override;

private:
    void GameOver();
    int timeLeft;
    bool isStarted;
    QTimer* timer;
    Board* board;
    QLCDNumber* LCDNum;
    QPushButton* startButton;
    QPushButton* quitButton;
    QPushButton* instructionButton;
    QPushButton* newLevelButton;
    QLabel* loseLabel;
    QLabel* winLabel;
    QWidget *instructionsWidget = nullptr;

signals:
    void timeIsUp();

public slots:
    void UpdateLcdTimer();
    void GameStart();
    void GameLose();
    void GameWin();
    void NewLevel();
    void showInstructionsWidget();
};
