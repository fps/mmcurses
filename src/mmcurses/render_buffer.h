 #pragma once

#include <cstdint>
#include <vector>

#include <mmcurses/geometry.h>

namespace mmcurses
{
    struct render_buffer
    {
        typedef uint32_t character;
        
        render_buffer(geometry::size s) :
            m_size(s),
            m_cursor_position(0,0),
            m_characters(s.m_width * s.m_height, ' '),
            m_null('~')
        {
        }
        
        character &at(geometry::position p)
        {
            if 
            (
                   p.m_x < m_size.m_width
                && p.m_x >= 0
                && p.m_y < m_size.m_height
                && p.m_y >= 0
            )
            {
                return m_characters[p.m_y * m_size.m_width + p.m_x];
            }
            else
            {
                return m_null;
            }
        }
        
        const geometry::size m_size;

        geometry::position m_cursor_position;
        
        std::vector<character> m_characters;

        character m_null;
    };
    
    struct render_buffer_view : render_buffer
    {
        render_buffer_view(geometry::size s) :
            render_buffer(s)
        {
        }
        
        void fill(character c, geometry::rectangle r)
        {
            for (int index_x = r.m_position.m_x; index_x < r.m_position.m_x + r.m_size.m_width; ++index_x)
            {
                for (int index_y = r.m_position.m_y; index_y < r.m_position.m_y + r.m_size.m_height; ++index_y)
                {
                    at(geometry::position(index_x, index_y)) = c;
                }
            }
        }
    };
}