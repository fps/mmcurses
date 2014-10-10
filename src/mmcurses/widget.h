#pragma once

#include <cstdint>
#include <vector>

#include <mmcurses/geometry.h>
#include <mmcurses/render_buffer.h>
#include <mmcurses/event.h>

namespace mmcurses
{
    namespace widget
    {
        /**
            Abstract base type for widgets.
        */
        struct base
        {
            /**
                Return the size that would be optimal for
                presenting the widget given its current 
                state.
            */
            virtual geometry::size size() = 0;
            
            /**
                Render your widget into the passed buffer.
                
                NOTE: The size of the render buffer might be
                different (most probably is) than what is 
                returned by size().
            */
            virtual void render(render_buffer_view &buffer) = 0;
            
            /**
                Return true to receive keyboard events.
                
                NOTE: The widget should set a cursor position
                if it handles keyboard events.
            */
            virtual bool focussable() = 0;
            
            /**
                Return true if you handled the event.
            */
            virtual bool process_event(const event::base &e) = 0;
        };
        
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
                buffer.fill(m_character, geometry::rectangle(geometry::position(0,0), geometry::size(buffer.m_size)));
            }
            
            bool focussable() override
            {
                return false;
            }
            
            bool process_event(const event::base &e) override
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