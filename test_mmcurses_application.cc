#include <mmcurses/widget_application.h>
#include <iostream>

struct application : mmcurses::application
{
	virtual void repaint(unsigned width, unsigned height) override
	{
	}

	virtual void key_pressed(int c)
	{
		if (c == 'q')
		{
			quit(0);
		}
	}
};

int main()
{
	application a;
	return a.exec();
}

