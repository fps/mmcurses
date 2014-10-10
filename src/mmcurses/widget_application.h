#pragma once

#include <mmcurses/application.h>
#include <mmcurses/widget.h>

#include <memory>

#include <ncurses.h>

namespace mmcurses
{
    struct widget_application : application
    {
        widget_application(std::shared_ptr<widget::base> widget) :
            m_widget(widget)
        {
            mousemask(ALL_MOUSE_EVENTS, NULL);
        }
        
        std::shared_ptr<widget::base> m_widget;
        
        void repaint(unsigned width, unsigned height) override
        {
            if (false == m_widget)
            {
                return;
            }
            
            render_buffer_view buffer(geometry::size(width, height));
            
            m_widget->render(buffer);
            
            for (int index_x = 0; index_x < buffer.m_size.m_width; ++index_x)
            {
                for (int index_y = 0; index_y < buffer.m_size.m_height; ++index_y)
                {
                    uint32_t c[2] =
                    {
                        buffer.at(geometry::position(index_x, index_y)),
                        0
                    };
                    
                    mvprintw(index_y, index_x, (char*)(c));
                }
            }
        }
        
        void key_pressed(int k) override
        {
            event::key key_event;
            m_widget->process_event(key_event);
        }
    };
    
    typedef std::shared_ptr<widget::base> widget_ptr;
}
