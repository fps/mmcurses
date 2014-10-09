#pragma once

#include <memory>

namespace mmcurses
{
    struct application_state;

    /**
        This is a very low level abstraction which basically just provides a main loop functionality around ncurses.
        
        See the test_mmcurses_application.cc as an example of how to use this class.
        
        NOTE: In a tradeoff between feature completeness and developer effort, we chose to not support unicode. Also the application is required to redraw the whole screen on repaint. No sophisticated invalidation of regions or similar stuff. If you need a more bandwidth conservative library, please pay me to develop it ;)
        
        See the mmcurses/widget_application.h for a more sophisticated subclass of this that supports widgets, layouts, etc.
    */
    struct application
    {
        std::auto_ptr<application_state> m_state;

        /**
            The constructor initializes the screen and sets up some useful defaults.
        */
        application();

        /**
            Restores the terminal to the state before the constructor was called.
        */
        virtual ~application();

        /**
            Reimplement this if you want to react to terminal size changes. The default implementation just calls invalidate().
        */
        virtual void size_changed(unsigned width, unsigned height);
   
        /**
            Causes repaint() to be called from the main loop. Clears the screen before calling repaint() and refreshes the screen afterwards by calling ncurses' refresh().
        */
        virtual void invalidate();
   
        /**
            Put your drawing code here. The default implementation does nothing.
        */
        virtual void repaint(unsigned width, unsigned height);

        /**
            Reimplement this if you want to react to key presses. Check the ncurses documentation for the possible key codes k.
            
            Keys codes that are not delivered to this method: ERR, KEY_RESIZE
            
            The default implementation just calls quit(0).
        */
        virtual void key_pressed(int k);
        
        /**
            Call this function to make the application exit at a point in the future.
        */
        virtual void quit(int rc);

        /**
            The entry point into the application's main loop. It returns the rc passed into quit().
        */
        virtual int exec();
    };
}

