#include "board.h"

void Board::setPlayerCanMove(const bool& b)
{
    playerCanMove = b;
}

void Board::GenerateMaze()
{
    srand(time(0));

    struct G_Block
    {
        int m_x, m_y;
        bool m_isWall;
        bool m_isVisited;
        void Set(const int& x, const int& y, const bool& isWall, const bool& isVisited)
        {
            m_x = x;
            m_y = y;
            m_isWall = isWall;
            m_isVisited = isVisited;
        }
    };

    QStack<G_Block*> NotVisited;

    G_Block G_block[MAZE_WIDTH][MAZE_HEIGHT];
    for (int x = 0; x < MAZE_WIDTH; x++)
    {
        for (int y = 0; y < MAZE_HEIGHT; y++)
        {

            if (x % 2 == 1 && y % 2 == 1)
            {
                G_block[x][y].Set(x, y, false, false);
                NotVisited.push(&G_block[x][y]);
            }
            else
            {
                G_block[x][y].Set(x, y, true, true);
            }
        }
    }
    \
    G_Block* curBlock = NotVisited.pop();
    curBlock->m_isVisited = true;


    G_Block* neighbors[4];
    int neighborsCount = 0;

    while (!NotVisited.isEmpty())
    {
        neighborsCount = 0;
        int dx[] = { -2, 2, 0, 0 };
        int dy[] = { 0, 0, -2, 2 };

        for (int i = 0; i < 4; ++i)
        {
            int nx = curBlock->m_x + dx[i];
            int ny = curBlock->m_y + dy[i];


            if (nx >= 0 && ny >= 0 && nx < MAZE_WIDTH && ny < MAZE_HEIGHT && !G_block[nx][ny].m_isVisited)
            {
                neighbors[neighborsCount] = &G_block[nx][ny];
                neighborsCount++;
            }
        }

        if (neighborsCount > 0)
        {
            NotVisited.push(curBlock);

            G_Block* neighbor = neighbors[rand() % neighborsCount];
            int deltaX = neighbor->m_x - curBlock->m_x;
            int deltaY = neighbor->m_y - curBlock->m_y;

            G_block[curBlock->m_x + deltaX / 2][curBlock->m_y + deltaY / 2].m_isWall = false;

            curBlock = neighbor;
            curBlock->m_isVisited = true;
        }
        else
        {
            curBlock = NotVisited.pop();
            curBlock->m_isVisited = true;
        }
    }


    m_block = new Block** [MAZE_WIDTH];
    for (int x = 0; x < MAZE_WIDTH; x++)
    {
        m_block[x] = new Block * [MAZE_HEIGHT];
        for (int y = 0; y < MAZE_HEIGHT; y++)
        {
            if (G_block[x][y].m_isWall == true)
            {
                m_block[x][y] = new Block(x, y, "black");
            }
            else
            {
                m_block[x][y] = new Block(x, y, "white");
            }
        }
    }
}



Board::~Board()
{
    for (int x = 0; x < MAZE_WIDTH; x++)
    {
        for (int y = 0; y < MAZE_HEIGHT; y++)
        {
            delete m_block[x][y];
        }
    }
    for (int x = 0; x < MAZE_WIDTH; x++)
    {
        delete m_block[x];
    }
    delete m_block;
}

Board::Board() : m_player(0), m_finish(0), playerCanMove(false)
{
    GenerateNewMazeLevel();
}

void Board::drawSquare(QPainter* p, Block* block)
{
    QRect rect(block->GetX() * LENGTH_OF_SQUARE, block->GetY() * LENGTH_OF_SQUARE, LENGTH_OF_SQUARE, LENGTH_OF_SQUARE);
    p->drawRect(rect);
    p->fillRect(rect, block->GetColor());
}

void Board::GenerateNewMazeLevel()
{

    delete m_player;
    m_player = new Player;

    delete m_finish;
    m_finish = new Block(MAZE_WIDTH - 2, 1, "Green");

    GenerateMaze();

    update();
}

void Board::paintEvent(QPaintEvent* event)
{
    QPainter *p = new QPainter(this);

    QPen* pen = new QPen;
    pen->setWidth(3);
    p->setPen(*pen);

    for (int x = 0; x < MAZE_WIDTH; x++)
    {
        for (int y = 0; y < MAZE_HEIGHT; y++)
        {
            drawSquare(p, m_block[x][y]);
        }
    }

    drawSquare(p, m_player);
    drawSquare(p, m_finish);
}



void Board::keyPressEvent(QKeyEvent* event)
{
    if (playerCanMove)
    {
        switch (event->key())
        {
        case Qt::Key_Left:
            for (int i = 0; i < PLAYER_SPEED; i++)
                m_player->tryMove(m_player->GetX() - 1, m_player->GetY(), m_block);
            break;
        case Qt::Key_Right:
            for (int i = 0; i < PLAYER_SPEED; i++)
                m_player->tryMove(m_player->GetX() + 1, m_player->GetY(), m_block);
            break;
        case Qt::Key_Down:
            for (int i = 0; i < PLAYER_SPEED; i++)
                m_player->tryMove(m_player->GetX(), m_player->GetY() + 1, m_block);
            break;
        case Qt::Key_Up:
            for (int i = 0; i < PLAYER_SPEED; i++)
                m_player->tryMove(m_player->GetX(), m_player->GetY() - 1, m_block);
            break;
        }


        if (m_player->GetX() == MAZE_WIDTH - 2 && m_player->GetY() == 1)
        {
            emit PlayerReachedFinish();
        }
        update();
    }
}
