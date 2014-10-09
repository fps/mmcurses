#include <mmcurses/widget_application.h>

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>


/**
    A very simple example that just outputs some diagnostics on the screen.
*/
struct application : mmcurses::application
{
	virtual void repaint(unsigned width, unsigned height) override
	{
        std::stringstream s;
        s << "Size: " << width << " x " << height << ". Press 'q' to quit.";
        mvprintw(height / 2, std::max<int>(0, width / 2 - (int)s.str().size() / 2), s.str().c_str());
	}

	virtual void key_pressed(int c)
	{
		if (c == 'q')
		{
			quit(0);
		}
        
        invalidate();
	}
};

int main()
{
	application a;
	return a.exec();
}

