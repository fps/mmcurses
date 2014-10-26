#pragma once

#include <mmcurses/widget/base.h>

namespace mmcurses
{
    namespace widget
    {
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