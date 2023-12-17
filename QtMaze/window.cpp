#include "window.h"
#include "const.h"


Window::Window() : timer(0)
{

    board = new Board();
    resize(1800, 1080);
    setWindowState(Qt::WindowFullScreen);
    setStyleSheet("background-color: lightsteelblue;");

    LCDNum = new QLCDNumber(3);
    LCDNum->setSegmentStyle(QLCDNumber::Filled);
    LCDNum->setStyleSheet("background-color: grey;");


    startButton = new QPushButton("Start");
    QFont font = startButton->font();
    font.setBold(true);
    startButton->setFont(font);
    startButton->setStyleSheet("background-color: grey;");


    quitButton = new QPushButton("Quit");
    font.setBold(true);
    quitButton->setFont(font);
    quitButton->setStyleSheet("background-color: grey;");


    newLevelButton = new QPushButton("New Level");
    font.setBold(true);
    newLevelButton->setFont(font);
    newLevelButton->setStyleSheet("background-color: grey;");

    instructionButton = new QPushButton("Instructions");
    font.setBold(true);
    instructionButton->setFont(font);
    instructionButton->setStyleSheet("background-color: grey;");



    board->setFocus();
    startButton->setFocusPolicy(Qt::NoFocus);
    quitButton->setFocusPolicy(Qt::NoFocus);
    newLevelButton->setFocusPolicy(Qt::NoFocus);
    instructionButton->setFocusPolicy(Qt::NoFocus);



    connect(quitButton, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(newLevelButton, SIGNAL(clicked()), this, SLOT(NewLevel()));
    connect(startButton, SIGNAL(clicked()), this, SLOT(GameStart()));
    connect (instructionButton, SIGNAL(clicked()), this, SLOT(showInstructionsWidget()));

    connect(this, SIGNAL(timeIsUp()), this, SLOT(GameLose()));
    connect(board, SIGNAL(PlayerReachedFinish()), this, SLOT(GameWin()));



    loseLabel = new QLabel("<p style='color:Red; text-align: center;\
                    font-size: 100pt; font-weight: bold;'>You Lose!</p>");
    winLabel = new QLabel("<p style='color:Green; text-align: center;\
                    font-size: 100pt; font-weight: bold;'>You Win!</p>");
    loseLabel->hide();
    winLabel->hide();

    QGridLayout* layout = new QGridLayout;



    layout->addWidget(board, 0, 0, 8, 4);

    layout->addWidget(loseLabel, 0, 0, 8, 4);
    layout->addWidget(winLabel, 0, 0, 8, 4);

    layout->addWidget(LCDNum, 0, 5);

    layout->addWidget(startButton, 1, 5);
    layout->addWidget(newLevelButton, 2, 5);
    layout->addWidget(quitButton, 3, 5);
    layout->addWidget(instructionButton, 4, 5);

    setLayout(layout);


}

void Window::NewLevel()
{
    if (timer != 0 && timer->isActive())
    {
        GameOver();
    }

    LCDNum->display(0);
    loseLabel->hide();
    winLabel->hide();

    board->GenerateNewMazeLevel();
}

void Window::showInstructionsWidget()
{
    if (instructionsWidget)
    {
        delete instructionsWidget;
        instructionsWidget = nullptr;
    }
    if (!instructionsWidget)
    {
        instructionsWidget = new QWidget;
        QLabel *instructionsLabel = new QLabel("START  -  When you press START, a random maze will be generated and control buttons start working<br><br>"
                                               "NEW LEVEL  -  When you press NEW LEVEL, there will be generated another random maze<br><br>"
                                               "QUIT  -  When you press QUIT the game will be stopped and wondow will be closed<br><br>"
                                               "GAME RULES  -  If a player reaches the finish line before time runs out, he wins, the game stops and displays message You Win!<br>"
                                               "               If the player runs out of time and does not reach the finish line, he loses, the game stops and displays message You Lose!");

        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(instructionsLabel);

        instructionsLabel->setStyleSheet("font-size: 18px; color: #333333;font-weight: bold;");

        instructionsWidget->setStyleSheet("background-color: lightgrey;");
        instructionsWidget->setLayout(layout);
        instructionsWidget->setWindowTitle("Instructions");
        instructionsWidget->setGeometry(300, 200, 600, 500);
        instructionsWidget->show();
    }
}

void Window::GameLose()
{
    GameOver();
    loseLabel->show();
}

void Window::GameWin()
{
    GameOver();
    winLabel->show();
}



void Window::GameOver()
{
    timer->stop();
    delete timer;
    timer = 0;

    isStarted = false;

    board->setPlayerCanMove(false);
}

void Window::UpdateLcdTimer()
{
    timeLeft--;
    LCDNum->display(timeLeft);
    if (timeLeft == 0)
    {
        emit timeIsUp();
    }
}

void Window::keyPressEvent(QKeyEvent* event)
{
    board->keyPressEvent(event);
}


void Window::GameStart()
{
    if (!isStarted)
    {
        if (loseLabel->isHidden()==false)
        {
            loseLabel->hide();
            board->GenerateNewMazeLevel();
        }

        if (winLabel->isHidden()==false)
        {
            winLabel->hide();
            board->GenerateNewMazeLevel();
        }
    }
    isStarted = true;
    board->setPlayerCanMove(true);

    timeLeft = ROUND_TIME;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(UpdateLcdTimer()));
    timer->start(1200);
}
