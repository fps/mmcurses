#include <mmcurses/widget_application.h>

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

#include <ncurses.h>

/**
    A very simple example that just outputs some diagnostics on the screen.
*/
struct application : mmcurses::application
{
    unsigned m_number_of_key_presses;
    
    application() :
        m_number_of_key_presses(0)
    {   
    }
    
    virtual void repaint(unsigned width, unsigned height) override
    {
        std::stringstream s;
        s << "Size: " << width << " x " << height << ". Press 'q' to quit. Key presses: " << m_number_of_key_presses;
        mvprintw(height / 2, std::max<int>(0, width / 2 - (int)s.str().size() / 2), s.str().c_str());
    }

    virtual void key_pressed(int c)
    {
        if (c == 'q' || c == 'Q')
        {
            quit(0);
        }
        
        ++m_number_of_key_presses;
        
        invalidate();
    }
};

int main()
{
    application a;
    return a.exec();
}

