#pragma once

namespace mmcurses
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
    }
    
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
    
    struct render_buffer
    {
        size size();
        void set_size();
        
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
                std::string m_character;
                
            };
            
            struct mouse : base
            {
                int m_button;
                position m_position;
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
            virtual size size() = 0;
            
            /**
                Render your widget into the passed buffer.
                
                NOTE: The size of the render buffer might be
                different (most probably is) than what is 
                returned by preferred_size().
            */
            virtual void render(render_buffer &buffer) = 0;
            
            virtual bool focussable() = 0;
            
            virtual bool process_event(const event &e) = 0;
        };
        
        /**
            A widget that renders the same character to
            every position in its buffer.
        */
        struct constant
        {
            constant()
            {
            }
            
            virtual size preferred_size() override
            {
                return size(1,1);
            }
        };
    }
}