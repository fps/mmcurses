#include <mmcurses/application.h>

#include <ncurses.h>
#include <cstdlib>
#include <chrono>
#include <thread>

namespace mmcurses
{
    struct application_state
    {
        application_state() :
            m_done(false),
            m_rc(0),
            m_width(0),
            m_height(0),
            m_invalidated(false),
            m_refresh_interval_milliseconds(10)
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
        m_state(new application_state)
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

    /**
        Reimplement this if you want to react to terminal size changes. The default implementation just calls invalidate().
    */
    void application::size_changed(unsigned width, unsigned height)
    {
        invalidate();
    }

    /**
        Causes repaint() to be called from the main loop. Refreshes the screen afterwards.
    */
    void application::invalidate()
    {
        m_state->m_invalidated = true;
    }

    /**
        Put your drawing code here. The default implementation does nothing.
    */
    void application::repaint(unsigned width, unsigned height)
    {
    }

    /**
        Reimplement this if you want to react to key presses. Check the ncurses documentation for the possible key codes k.
        
        Keys codes that are not delivered to this method: ERR, KEY_RESIZE
        
        The default implementation just calls quit(0).
    */
    void application::key_pressed(int k)
    {
        quit(0);
    }

    /**
        Call this function to make the application exit at a point in the future.
    */
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
        int c = getch();
        
        pre_process();
        
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
    
    /**
        The entry point into the application's main loop. It returns the rc passed into quit().
    */
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
