#include "Block.h"



Block::Block(const int& x, const int& y, const QColor& color) : m_x(x), m_y(y), m_color(color) {}


int Block::GetX() const
{
    return m_x;
}

int Block::GetY() const
{
    return m_y;
}

QColor Block::GetColor() const
{
    return m_color;
}

void Block::SetColor(const QColor& other)
{
    m_color = other;
}

