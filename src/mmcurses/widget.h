#pragma once

#include <cstdint>
#include <vector>

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
    
    struct render_buffer
    {
        typedef uint32_t character;
        
        render_buffer(geometry::size s) :
            m_size(s),
            m_cursor_position(0,0),
            m_characters(s.m_width * s.m_height, ' ')
        {
        }
        
        character &at(geometry::position p)
        {
            return m_characters[p.m_y * m_size.m_width + p.m_x];
        }
        
        const geometry::size m_size;

        geometry::position m_cursor_position;
        
        std::vector<character> m_characters;
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
    
    namespace widget
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