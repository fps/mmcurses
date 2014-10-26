#pragma once

#include <cstdint>
#include <vector>
#include <utility>
#include <algorithm>
#include <sstream>
#include <memory>

#include <mmcurses/geometry.h>
#include <mmcurses/render_buffer.h>
#include <mmcurses/event.h>

namespace mmcurses
{
    namespace widget
    {
        struct base;
        
        typedef std::shared_ptr<base> ptr;
        
        /**
            Abstract base type for widgets.
        */
        struct base
        {
            /**
                When true, indicates that this widget has focus (one of its 
                children having focus also indicates truth).
            */
            bool m_focussed;
            
            base() :
                m_focussed(false)
            {
            }
            
            /**
                Return the size that would be optimal for
                presenting the widget given its current 
                state.
            */
            virtual geometry::size size()
            {
                namespace g = geometry;
                return g::size(1,1);
            }
            
            /**
                Render your widget into the passed buffer.
                
                NOTE: The size of the render buffer might be
                different (most probably is) than what is 
                returned by size().
            */
            virtual void render(render_buffer_view &buffer)
            {
                namespace g = geometry;
                if (buffer.m_size.m_width > 0 && buffer.m_size.m_height > 0)
                {
                    buffer.at(g::position(0,0)) = '#';
                }
            }
            
            /**
                Return true to receive keyboard events.
                
                NOTE: The widget should set a cursor position
                if it handles keyboard events.
            */
            virtual bool focussable()
            {
                return false;
            }
            
            /**
                React to key presses.
            */
            virtual void process_key_event(const event::key &e)
            {
            }
            
            virtual bool enter_focus()
            {
                if (true == focussable())
                {
                    m_focussed = true;
                    return true;
                }
                return false;
            }
            
            virtual bool advance_focus()
            {
                m_focussed = false;
                return false;
            }
        };
    }
}