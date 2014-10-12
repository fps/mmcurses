 
#include <mmcurses/widget_application.h>

struct constant_char : mmcurses::widget::constant
{
    constant_char(mmcurses::render_buffer::character c) :
        constant(c)
    {
    }
    
    bool focussable() override
    {
        return true;
    }

    void process_key_event(const mmcurses::event::key &e) override
    {
        if (e.m_key < 255)
        {
            m_character = e.m_key;
        }
    }
};

int main()
{
    mmcurses::widget::ptr c(new constant_char('#'));
    mmcurses::widget_application a(c);
    return a.exec();
}