#include <mmcurses/application.h>

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

#include <ncurses.h>
#include <chrono>

/**
    A very simple example that creates a simple animation.
*/
struct application : mmcurses::application
{
    // Varies between 0.0f and 1.0f
    float m_progress;
    const float m_speed;
    
    std::chrono::system_clock::time_point m_last_time;
    
    application() :
        m_progress(0.0f),
        m_speed(0.5f),
        m_last_time(std::chrono::system_clock::now())
    {
        curs_set(0);
        set_refresh_interval_milliseconds(50);
    }
    
    virtual void repaint(unsigned width, unsigned height) override
    {
        mvprintw((int)(m_progress * (float)height), (int)(m_progress * float(width)), "*");
        mvprintw(height - 1, 0, "Press 'q' to quit");
    }

    virtual void key_pressed(int c) override
    {
        if (c == 'q' || c == 'Q')
        {
            quit(0);
        }
        else
        {
            invalidate();
        }
    }
    
    virtual void process() override
    {
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        std::chrono::milliseconds delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_last_time);
        m_last_time = now;
        
        m_progress += m_speed * (float)delta.count()/float(1000);
        m_progress = fmodf(m_progress, 1.0f);
        
        invalidate();
    }
};

int main()
{
    application a;
    return a.exec();
}

