#include <mmcurses/application.h>

#include <ncurses.h>
#include <cstdlib>
#include <chrono>
#include <thread>

namespace mmcurses
{
    struct application::state
    {
        state() :
            m_done(false),
            m_rc(0),
            m_width(0),
            m_height(0),
            m_invalidated(false),
            m_refresh_interval_milliseconds(100)
        {
        }
        
        bool m_done;
        int m_rc;

        int m_width;
        int m_height;
        
        bool m_invalidated;
        
        unsigned long m_refresh_interval_milliseconds;
    };
    
    application::application() :
        m_state(new state)
    {
        SCREEN *s = newterm(getenv("TERM"), stdout, stdin);
        if (s == nullptr) { /* throw */ }
        keypad(stdscr, TRUE);
        noecho();
        nodelay(stdscr,1);
        cbreak();
    }

    application::~application()
    {
        endwin();
    }

    void application::size_changed(unsigned width, unsigned height)
    {
        invalidate();
    }

    void application::invalidate()
    {
        m_state->m_invalidated = true;
    }

    void application::repaint(unsigned width, unsigned height)
    {
    }

    void application::key_pressed(int k)
    {
        quit(0);
    }

    void application::quit(int rc)
    {
        m_state->m_rc = rc;
        m_state->m_done = true;
    }

    void application::pre_process()
    {
    }
    
    void application::post_process()
    {
    }
    
    void application::process()
    {
    }
    
    void application::set_refresh_interval_milliseconds(unsigned long interval)
    {
        m_state->m_refresh_interval_milliseconds = interval;
    }
    
    void application::iterate()
    {
        pre_process();
        
        int c = getch();
        
        int x,y;
        getmaxyx(stdscr, y, x);

        bool size_change = x != m_state->m_width || y != m_state->m_height;

        m_state->m_width = x;
        m_state->m_height = y;

        if (size_change)
        {
            size_changed(m_state->m_width, m_state->m_height);
        }

        if (c != ERR && c != KEY_RESIZE)
        {
            key_pressed(c);
        }
        
        process();
        
        if (m_state->m_invalidated)
        {
            clear();
            repaint(m_state->m_width, m_state->m_height);
            refresh();
            m_state->m_invalidated = false;
        }
        
        post_process();
    }
    
    int application::exec()
    {
        while(false == m_state->m_done)
        {
            iterate();
            std::this_thread::sleep_for(std::chrono::milliseconds(m_state->m_refresh_interval_milliseconds));
        }
        return m_state->m_rc;
    }
}
