#pragma once

#include <mmcurses/widget/base.h>

namespace mmcurses
{
    namespace widget
    {
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
    }
}
