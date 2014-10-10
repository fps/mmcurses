#pragma once

namespace mmcurses
{
    namespace event
    {
        struct base
        {
            virtual ~base()
            {
            }
        };
        
        struct key : base
        {
                int m_key;
        };
        
        struct mouse_press : base
        {
            int m_button;
            geometry::position m_position;
        };
    }
}