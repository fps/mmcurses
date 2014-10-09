#include <mmcurses/application.h>

#include <ncurses.h>
#include <cstdlib>

namespace mmcurses
{
    application::application() :
        m_done(false),
        m_rc(0),
        m_width(0),
        m_height(0),
        m_invalidated(false)
    {
        SCREEN *s = newterm(getenv("TERM"), stdout, stdin);
        if (s == nullptr) { /* throw */ }
        keypad(stdscr, TRUE);
        raw();
        noecho();
        halfdelay(1);
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
        m_invalidated = true;
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
        m_rc = rc;
        m_done = true;
    }

    /**
        The entry point into the application's main loop. It returns the rc passed into quit().
    */
    int application::exec()
    {
        while(false == m_done)
        {
            int c = getch();
            
            int x,y;
            getmaxyx(stdscr, y, x);

            bool size_change = x != m_width || y != m_height;

            m_width = x;
            m_height = y;

            if (size_change)
            {
                size_changed(m_width, m_height);
            }        

            if (m_invalidated)
            {
                clear();
                repaint(m_width, m_height);
                refresh();
                m_invalidated = false;
            }
            
            if (c != ERR && c != KEY_RESIZE)
            {
                key_pressed(c);
            }
        }
        return m_rc;
    }
}
