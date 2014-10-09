#pragma once

#include <mmcurses/application.h>
#include <memory>

namespace mmcurses
{
    struct size
    {
        unsigned m_width;
        unsigned m_height;
    };

    struct position
    {
        unsigned m_x;
        unsigned m_y;
    };

    struct region
    {
        position m_position;
        size m_size;
    };


    struct widget
    {
        virtual ~widget()
        {
            
        }

        virtual bool want_focus()
        {
            return false;
        }

        virtual void render(region the_region)
        {

        } 
    };

    struct widget_application : application
    {
        std::shared_ptr<widget> m_widget;
    };
}
