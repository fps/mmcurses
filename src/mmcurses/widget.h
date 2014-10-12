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
        
        struct rows : base
        {
            typedef std::vector<std::pair<ptr, uint32_t>> widgets_with_weights;
            
            widgets_with_weights m_widgets_with_weights;
            
            rows(widgets_with_weights &widgets) :
                m_widgets_with_weights(widgets)
            {
            }
            
            void render(render_buffer_view &buffer) override
            {
                namespace g = geometry;
                
                uint32_t sum_of_weights = 0;
                int sum_of_heights = 0;
                int sum_of_zero_weight_heights = 0;
                
                for (const auto &w : m_widgets_with_weights)
                {
                    sum_of_weights += w.second;
                    sum_of_heights += w.first->size().m_height;
                    if (w.second == 0)
                    {
                        sum_of_zero_weight_heights += w.first->size().m_height;
                    }
                }
                
                sum_of_weights = std::max<uint32_t>(1, sum_of_weights);
                
                int space = buffer.m_size.m_height - sum_of_heights;
                int space_remaining = space;
                
                widgets_with_weights w2 = m_widgets_with_weights;
                
                int current_y_position = 0;
                
                for (auto &w : w2)
                {
                    if (space_remaining > 0)
                    {
                        w.second = (uint32_t)std::min<float>
                        (
                            (float)space_remaining, 
                            ceilf
                            (
                                  (float)space 
                                * (float)w.second 
                                / (float)sum_of_weights
                            )
                        );
                        
                        space_remaining -= w.second;
                    }
                    
                    
                    
                    render_buffer_view widget_buffer
                    (
                        geometry::size
                        (
                            buffer.m_size.m_width, 
                            w.second + w.first->size().m_height
                        )
                    );
                    
                    w.first->render(widget_buffer);
                    
                    for (int x = 0; x < widget_buffer.m_size.m_width; ++x)
                    {
                        for (int y = 0; y < widget_buffer.m_size.m_height; ++y)
                        {
                             buffer.at(g::position(x, current_y_position + y)) = 
                                widget_buffer.at(g::position(x,y));
                        }
                    }
                    
                    current_y_position += w.second + w.first->size().m_height;
                }
            }
            
                 
            bool enter_focus() override
            {
                for (auto &w : m_widgets_with_weights)
                {
                    if (w.first->focussable())
                    {
                        w.first->enter_focus();
                        return true;
                    }
                }
                
                return false;
            }
            
            bool advance_focus() override
            {
                bool advance = false;
                for (auto &w : m_widgets_with_weights)
                {
                    if (advance)
                    {
                        if (w.first->focussable())
                        {
                            w.first->enter_focus();
                            return true;
                        }
                    }
                    
                    if (w.first->m_focussed)
                    {
                        w.first->m_focussed = w.first->advance_focus();
                        advance = !w.first->m_focussed;
                    }
                    
                }
               return false;
            }

            bool focussable() override
            {
                for (auto &w : m_widgets_with_weights)
                {
                    if (w.first->focussable())
                    {
                        return true;
                    }
                }
                
                return false;
            }
            
            void process_key_event(const event::key &e) override
            {
                for (auto &w : m_widgets_with_weights)
                {
                    w.first->process_key_event(e);
                }
            }
            
            geometry::size size() override
            {
                geometry::size r(0,0);
                
                for (const auto &w : m_widgets_with_weights)
                {
                    r.m_height += w.first->size().m_height;
                    if (w.first->size().m_width > r.m_width)
                    {
                        r.m_width = w.first->size().m_width;
                    }
                }
                
                return r;
            }
        };
        
        struct columns : base
        {
            
        };
        
        struct label : base
        {
            // Lines
            std::vector<std::string> m_text;
            
            // Cached
            geometry::size m_size;
            
            label(std::string text) :
                m_size(0,0)
            {
                set_text(text);
            }
            
            void set_text(std::string text)
            {
                m_size = geometry::size(0,0);
                std::istringstream stream(text);
                std::string line;
                while(std::getline(stream, line))
                {
                    m_text.push_back(line);
                    m_size.m_width = std::max((int)line.size(), m_size.m_width);
                    ++m_size.m_height;
                }
            }
            
            geometry::size size()
            {
                return m_size;
            }
            
            void render(render_buffer_view &buffer)
            {
                int line_index = 0;
                for (const auto& line : m_text)
                {
                    int x = 0;
                    for (const auto &c : line)
                    {
                        buffer.at(geometry::position(x, line_index)) = c;
                        ++x;
                    }
                    ++line_index;
                }
            }
        };
    }
}