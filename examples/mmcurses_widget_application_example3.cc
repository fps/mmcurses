 
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

    bool process_key_event(const mmcurses::event::key &e) override
    {
        if (e.m_key < 255)
        {
            m_character = e.m_key;
            return true;
        }
        
        return false;
    }
};

int main()
{
    mmcurses::widget::rows::widgets_with_weights w =
    {
        { mmcurses::widget::ptr(new constant_char('#')), 0 },
        { mmcurses::widget::ptr(new constant_char('-')), 2 },
        { mmcurses::widget::ptr(new constant_char('+')), 1 },
        { mmcurses::widget::ptr(new mmcurses::widget::label("I am a label")), 0 }
    };
        
    mmcurses::widget::ptr r(new mmcurses::widget::rows(w));
    mmcurses::widget_application a(r);
    
    return a.exec();
}