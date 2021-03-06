#pragma once

#include <mmcurses/no_copy.h>

#include <memory>

namespace mmcurses
{
    /**
        This is a very low level abstraction which basically just provides a main loop functionality around ncurses.
        
        See the test_mmcurses_application.cc as an example of how to use this class.
        
        NOTE: In a tradeoff between feature completeness and developer effort we decided to reduce developer effort. This means: No unicode support. Also the application is required to redraw the whole screen on repaint. No sophisticated invalidation of regions or similar stuff. If you need a more bandwidth conservative library, please pay me (or somebody else) to develop it ;)
        
        See the mmcurses/widget_application.h for a more sophisticated subclass of this that supports widgets, layouts, etc.
    */
    struct application : no_copy
    {
        struct state;

        std::auto_ptr<state> m_state;

        /**
            The constructor initializes the screen and sets up some useful defaults.
            
            NOTE: We enable raw(), keypad(), noecho() and halfdelay(1).
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
           
           NOTE: You need to include ncurses.h to get access to the ncurses drawing functions.
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
            This method is called whenever the main loop gets a chance to run BEFORE any internal processing is performed.
        */
        virtual void pre_process();
        
        /**
            This method is called whenever the main loop gets a chance to run AFTER any internal processing and redrawing is performed.
        */
        virtual void post_process();
        
        /**
            This method is called after internal processing but BEFORE checking for invalidation and redrawing.
        */
        virtual void process();
        
        /**
            Set the approximate refresh interval in milliseconds.
        */
        virtual void set_refresh_interval_milliseconds(unsigned long interval);
        
        /**
            The entry point into the application's main loop. It returns the rc passed into quit(). 
            
            NOTE: The main loop runs approximately at least every tenth of a second.
        */
        virtual int exec();
        
        /**
            Run the "main loop" once.
        */
        virtual void iterate();
    };
}

