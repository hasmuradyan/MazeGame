#pragma once
#include <QWidget>
#include <QPainter>
#include <QKeyEvent>
#include <QStack>
#include "const.h"
#include "block.h"
#include "player.h"
#include <vector>
#include <ctime>
#include <iostream>
#include <random>
#include <stack>

class Board : public QWidget
{
    Q_OBJECT

private:
    Block*** m_block;
    Player* m_player;
    Block* m_finish;
    bool playerCanMove;
    void GenerateMaze();

protected:
    void paintEvent(QPaintEvent*);

public:
    Board();
    ~Board();
    void drawSquare(QPainter* p, Block* block);
    void setPlayerCanMove(const bool&);
    void keyPressEvent(QKeyEvent* event) override;

public slots:
    void GenerateNewMazeLevel();

signals:
    void PlayerReachedFinish();
};

