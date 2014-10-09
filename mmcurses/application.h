#pragma once

#include <ncurses.h>
#include <cstdlib>

namespace mmcurses
{
    struct event
    {
        virtual ~event()
        {
        }
    };

    struct key
    {
    };

    struct application
    {
        bool m_done;
        int m_rc;

        int m_width;
        int m_height;

        application() :
            m_done(false),
            m_rc(0),
            m_width(0),
            m_height(0)
        {
            SCREEN *s = newterm(getenv("TERM"), stdout, stdin);
            if (s == nullptr) { /* throw */ }
            keypad(stdscr, TRUE);
            raw();
            noecho();
            halfdelay(1);
        }

        virtual ~application()
        {
            endwin();
        }

        virtual void size_changed(unsigned width, unsigned height)
        {
            repaint(width, height);
        }
   
        virtual void repaint(unsigned width, unsigned height)
        {
            
        }

	    virtual void key_pressed(int c)
        {
            quit(0);
        }

        virtual void quit(int rc)
        {
            m_rc = rc;
            m_done = true;
        }

        virtual int exec()
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

                if (c != ERR)
                {
                    key_pressed(c);
                }
            }
            return m_rc;
        }

        //! return true if 
        virtual bool process_event(const event &e)
        {
            return false;
        }
    };
}

