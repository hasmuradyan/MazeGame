#pragma once
#include "const.h"
#include <QColor>

class Block
{
protected:
    int m_x, m_y;
    QColor m_color;

public:

    Block(const int& x, const int& y, const QColor& color);
    int GetX() const;
    int GetY() const;
    QColor GetColor() const;
    void SetColor(const QColor& other);
};

