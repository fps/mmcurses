#pragma once

#include <cstdint>
#include <vector>
#include <utility>
#include <algorithm>

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
            
            bool process_key_event(const event::key &e) override
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
                            buffer.at(geometry::position(x, current_y_position + y)) = widget_buffer.at(geometry::position(x,y));
                        }   
                    }
                    
                    current_y_position += w.second + w.first->size().m_height;
                }
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
        
        struct columns
        {
            
        };
    }
}