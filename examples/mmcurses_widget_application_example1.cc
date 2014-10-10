 
#include <mmcurses/widget_application.h>

int main()
{
    mmcurses::widget::ptr c(new mmcurses::widget::constant('#'));
    mmcurses::widget_application a(c);
    return a.exec();
}