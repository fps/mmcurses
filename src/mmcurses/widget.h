#pragma once

#include <cstdint>
#include <vector>
#include <utility>

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
            virtual bool process_key_event(const event::key &e) = 0;
        };
        
        typedef std::shared_ptr<base> ptr;
        
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
                
                buffer.fill(m_character, g::rectangle(g::position(0,0), g::size(buffer.m_size)));
            }
            
            bool focussable() override
            {
                return false;
            }
            
            bool process_key_event(const event::key &e) override
            {
                return false;
            }
            
            geometry::size size() override
            {
                return geometry::size(1,1);
            }
        };
        
        struct rows
        {
            typedef std::vector<std::pair<ptr, float>> widgets_with_weights;
            
            widgets_with_weights m_widgets_with_weights;
            
            rows(widgets_with_weights &widgets) :
                m_widgets_with_weights(widgets)
            {
                
            }
        };
        
        struct columns
        {
            
        };
    }
}