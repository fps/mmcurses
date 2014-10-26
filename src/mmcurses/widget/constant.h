 #pragma once

#include <mmcurses/widget/base.h>

namespace mmcurses
{
    namespace widget
    {
        /**
            A widget that renders the same character to
            every position in its buffer.
        */
        struct constant : base
        {
            render_buffer::character m_character;
            
            constant(render_buffer::character c) :
                m_character(c)
            {
            }
            
            void render(render_buffer_view &buffer)
            {
                namespace g = geometry;
                
                buffer.fill
                (
                    m_character, 
                    g::rectangle(g::position(0,0), g::size(buffer.m_size))
                );
            }
            
            bool focussable() override
            {
                return false;
            }
            
            geometry::size size() override
            {
                return geometry::size(1,1);
            }
        };
    }
}