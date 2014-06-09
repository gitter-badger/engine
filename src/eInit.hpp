/*!
 * \file eInit.hpp
 * \brief \b Classes: \a eInit
 * \sa e_eInit.cpp e_event.cpp
 */
/*
 *  E Engine
 *  Copyright (C) 2013 Daniel Mensinger
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef E_INIT_HPP
#define E_INIT_HPP

#include "defines.hpp"

#if UNIX_X11
#include <GL/glxew.h>
#include "x11/context.hpp"

#elif WINDOWS
#include "windows/context.hpp"
#endif

#include <boost/thread.hpp>



namespace e_engine {

typedef GLvoid( *RENDER_FUNC )( eWinInfo info );


/*!
 * \class e_engine::eInit
 * \brief Init the <b>E Engine</b>
 *
 * This class handels the context with the eContext class. It
 * sets the Crt-C handle function and starts the main and eventLoop
 * loop. This should be after eWindowData the 1st class you generate
 * and you will do all window and curser config with it.
 *
 * \note Until now there is no MS Windows or Mac support
 *
 * \note Cursor functions are not implemented yet
 *
 * \sa eContext eWindowData e_eInit.cpp e_event.cpp
 */
#if UNIX_X11
class eInit : public unix_x11::eContext {
#elif WINDOWS
class eInit : public windows_win32::eContext {
#endif
   private:
      RENDER_FUNC       fRender;

      bool              vMainLoopRunning_B;
      bool              vBoolCloseWindow_B;

      bool              vHaveGLEW_B;

      bool              vEventLoopHasFinished_B;
      bool              vRenderLoopHasFinished_B;

      boost::thread     vEventLoop_BT;
      boost::thread     vRenderLoop_BT;
      boost::thread     vQuitMainLoop_BT;
      boost::thread     vRestartThread_BT;
      
      boost::mutex      vMainLoopMutex_BT;
      boost::mutex      vEventLoopMutex_BT;
      
      boost::condition_variable vMainLoopWait_BT;
      boost::condition_variable vEventLoopWait_BT;
      
      bool              vMainLoopPaused_B;
      bool              vEventLoopPaused_B;
      
      bool              vMainLoopISPaused_B;
      bool              vEventLoopISPaused_B;
      
      bool              vLoopsPaused_B;
      
      int               vCreateWindowReturn_I;

      
#if WINDOWS
      boost::mutex              vCreateWindowMutex_BT;
      boost::condition_variable vCreateWindowCondition_BT;
      
      boost::mutex              vStartEventMutex_BT;
      boost::condition_variable vStartEventCondition_BT;
      bool                      vContinueWithEventLoop_B;     
#endif

      // Thread Functions --------------------------------------------------------- ###
      int  renderLoop();       //!< The render loop function
      int  eventLoop();        //!< The event loop function ( In PLATFORM/e_event.cpp )
      int  quitMainLoopCall(); //!< The actual function to quit the main loop

      // Signal handling ---------------------------------------------------------- ###
      static GLvoid handleSignal( int _signal ); //!< The signal handle function
      GLvoid _setThisForHandleSignal();          //!< Unfortunately you can not pass this with signal()

      // Standart callbacks NEW --------------------------------------------------- ###

      //! The standard render function
      static inline GLvoid standardRender( eWinInfo _info ) {
         glClearColor( 0, 0, 0, 1 );
         glClear( GL_COLOR_BUFFER_BIT );
         _info.eInitPointer->swapBuffers();
      }

      //! The standard Resize function
      GLvoid s_standardResize( eWinInfo _info ) {}

      //! The standard Window close function
      GLvoid s_standardWindowClose( eWinInfo _info );

      //! The standard Key function
      GLvoid s_standardKey( eWinInfo _info ) {}

      //! The standard Mouse function
      GLvoid s_standardMouse( eWinInfo _info ) {}
      
   public:
      eInit();
      ~eInit() {closeWindow();shutdown();}

      int    init();
      int    shutdown();
      int    startMainLoop( bool _wait = true );

      void   quitMainLoop();
      
      void   pauseMainLoop();
      void   continueMainLoop();
      
      void   restart( bool _runInNewThread = false );
      void   restartIfNeeded( bool _runInNewThread = false );

      /*!
       * \brief Quit the main loop and close the window
       * \param waitUntilClosed Wait until window is closed  \c DEFAULT: \b false
       * \returns \c SUCCESS: \a 1 -- \C FAIL: \a 0
       */
      int    closeWindow( bool _waitUntilClosed = false );
      

      GLvoid setRenderFunc( RENDER_FUNC _f )            {fRender = _f;}
};

//    #########################
// ######## BEGIN DOXYGEN ########
//    #########################


namespace e_engine_internal {

/*!
 * \class __eInit_Pointer
 * \brief Need this for the static signal Callback
 *
 * \todo Remove this workaround / hack
 *
 */
class __eInit_Pointer {
   private:
      eInit *pointer;
      bool  is_set;
   public:
      __eInit_Pointer() {
         pointer = 0;
         is_set  = false;
      }
      ~__eInit_Pointer() {pointer = 0;}
      bool set( eInit *_THIS ) {
         if ( is_set == true ) {return false;}
         pointer = _THIS;
         is_set = true;
         return true;
      }
      eInit *get() {return pointer;}
};
extern __eInit_Pointer __eInit_Pointer_OBJ;
}

}

#endif // E_INIT_HPP

// kate: indent-mode cstyle; indent-width 3; replace-tabs on; 
