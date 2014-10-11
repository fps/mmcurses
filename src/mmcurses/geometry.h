#pragma once

namespace mmcurses
{
    namespace geometry
    {
        struct position
        {
            int m_x;
            int m_y;
            
            position(int x, int y) :
                m_x(x),
                m_y(y)
            {
            }
        };
        
        struct size
        {
            int m_width;
            int m_height;
            
            size(int width, int height) :
                m_width(width),
                m_height(height)
            {
            }
            
            size &operator+(const size &s)
            {
                m_width += s.m_width;
                m_height += s.m_height;
                
                return *this;
            }
        };
        
        struct rectangle
        {
            position m_position;
            size m_size;
            
            rectangle(position p, size s) :
                m_position(p),
                m_size(s)
            {
            }
        };
    }
} 
