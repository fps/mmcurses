#pragma once

namespace mmcurses
{
    class no_copy
    {
        public:
            no_copy()
            {
            }
        
        private:
            no_copy(const no_copy&);
            no_copy& operator=(const no_copy&);
    };
}