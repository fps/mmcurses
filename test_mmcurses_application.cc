#include <mmcurses/widget_application.h>
#include <iostream>

struct application : mmcurses::application
{
    unsigned m_number_of_key_presses;
    
    application() :
        m_number_of_key_presses(0)
    {
        
    }
    
	virtual void repaint(unsigned width, unsigned height) override
	{
        
	}

	virtual void key_pressed(int c)
	{
		if (c == 'q')
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

