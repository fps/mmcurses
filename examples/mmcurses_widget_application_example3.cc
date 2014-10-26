 
#include <mmcurses/widget_application.h>
#include <mmcurses/widget/rows.h>
#include <mmcurses/widget/label.h>
#include <mmcurses/widget/constant.h>

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
        if (false == m_focussed)
        {
            return;
        }
        
        if (e.m_key < 255)
        {
            m_character = e.m_key;
        }
    }
};

int main()
{
    mmcurses::widget::rows::widgets_with_weights w =
    {
        { mmcurses::widget::ptr(new mmcurses::widget::label("I am a label")), 0 },
        { mmcurses::widget::ptr(new constant_char('#')), 0 },
        { mmcurses::widget::ptr(new constant_char('-')), 2 },
        { mmcurses::widget::ptr(new mmcurses::widget::label("I am a label\nwith a newline.\n\nand some more\n\n")), 0 },
        { mmcurses::widget::ptr(new constant_char('+')), 1 },
        { mmcurses::widget::ptr(new mmcurses::widget::label("I am a label")), 0 }
    };
        
    mmcurses::widget::ptr r(new mmcurses::widget::rows(w));
    mmcurses::widget_application a(r);
    
    return a.exec();
}