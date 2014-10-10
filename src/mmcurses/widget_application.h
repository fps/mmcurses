#pragma once

#include <mmcurses/application.h>
#include <mmcurses/widget.h>
#include <memory>

namespace mmcurses
{
    struct widget_application : application
    {
        std::shared_ptr<widget> m_widget;
    };
}
