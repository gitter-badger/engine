/*!
 * \file uConfig.cpp
 * \brief \b Classes: \a uConfig
 */
/*
 * Copyright (C) 2015 EEnginE project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "defines.hpp"

#if UNIX_X11
#include <GL/glxew.h>
#endif

#if WINDOWS
#include <GL/glew.h>
#include <GL/wglext.h>
#endif

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <math.h>


#include "uConfig.hpp"

using std::string;

namespace e_engine {


_uConfig::__uConfig_FBA::__uConfig_FBA() { reset(); }

_uConfig::__uConfig_Versions::__uConfig_Versions() { reset(); }

_uConfig::__uConfig_Window::__uConfig_Window() { reset(); }

_uConfig::__uLogData_Config::__uLogData_Config() { reset(); }

_uConfig::__uConfig_Config::__uConfig_Config() { reset(); }

_uConfig::__uConfig_OpenGL::__uConfig_OpenGL() { reset(); }

_uConfig::__uConfig_Camera::__uConfig_Camera() { reset(); }




void _uConfig::__uConfig_FBA::reset() {
#if UNIX
   FBA_RENDER_TYPE = GLX_RGBA_BIT;
   FBA_RENDERABLE = GL_TRUE;
   FBA_DRAWABLE_TYPE = GLX_WINDOW_BIT;
   FBA_DOUBLEBUFFER = GL_TRUE;
#endif              // UNIX
   FBA_RED = 8;     // 8
   FBA_GREEN = 8;   // 8
   FBA_BLUE = 8;    // 8
   FBA_ALPHA = 8;   // 8
   FBA_DEPTH = 24;  // 24
   FBA_STENCIL = 8; // 8
#if UNIX
   FBA_VISUAL_TYPE = GLX_TRUE_COLOR;
#endif
// FBA_STEREO                = GL_TRUE;

#if WINDOWS
   FBA_DRAW_TO_WINDOW = 1;
   FBA_ACCELERATION = WGL_FULL_ACCELERATION_ARB;
   FBA_RENDER_TYPE = WGL_TYPE_RGBA_ARB;
#endif // WINDOWS
}

void _uConfig::__uConfig_Versions::reset() {
   minGlxMajorVer = 1;
   minGlxMinorVer = 3;
   glMajorVersion = 4;
   glMinorVersion = 5;
}

void _uConfig::__uConfig_Window::reset() {
   width = 800;
   height = 600;
   minWidth = 50;
   minHeight = 50;
   posX = 0;
   posY = 0;

   mousePosX = 0;
   mousePosY = 0;

   mouseIsInWindow = false;
   windowHasFocus = true;

   fullscreen = false;
   VSync = true;
   windowDecoration = true;

   winType = NORMAL;

   windowName = "E Engine Window";
   xlibWindowName = "E Engine Window";
   iconName = "E Engine Window";

   restoreOldScreenRes = true;
   // iconPath.clear();
}

void _uConfig::__uLogData_Config::reset() {
   standardTimeColor = 'M';

   maxFilenameSize = 20;
   maxFunctionNameSize = 20;

   threadNameWidth = 6;

   standardShowTime = true;
   standardShowFile = true;
   standardShowLine = true;

   standardWarningsToStdErr = false;

   logDefaultInit = true;

   useHistory = true;

   waitUntilLogEntryPrinted = false;

   width = -1;

   logOUT.colors = DISABLED;
   logOUT.Time = LEFT_REDUCED;
   logOUT.File = RIGHT_REDUCED;
   logOUT.ErrorType = LEFT_FULL;
   logOUT.Thread = RIGHT_FULL;

   logERR.colors = REDUCED;
   logERR.Time = LEFT_REDUCED;
   logERR.File = RIGHT_REDUCED;
   logERR.ErrorType = LEFT_FULL;
   logERR.Thread = RIGHT_FULL;

   logFILE.Time = LEFT_FULL;
   logFILE.File = LEFT_FULL;
   logFILE.ErrorType = LEFT_FULL;
   logFILE.Thread = LEFT_FULL;
   logFILE.logFileName.clear();
}


void _uConfig::__uConfig_Config::reset() {
   appName = "e-engine";
   configSubFolder.clear();
   logSubFolder = "log";

   useTimeAtCMD = false;
   useTimeAtLog = true;

   useCMDColor = true;

   unixPathType = true;

   maxNumOfLogFileBackshift = 10;
}

void _uConfig::__uConfig_OpenGL::reset() {
   shaderInfoQueryType = 0;
   useShaders = true;
}


void _uConfig::__uConfig_Camera::reset() {
   movementSpeed = 0.2;
   mouseSensitivity = 0.001;
   angleHorizontal = M_PI;
   angleVertical = 0;
}



_uConfig::_uConfig() {
   handleSIGINT = true;
   handleSIGTERM = true;
   timeoutForMainLoopThread_mSec = 1000; // 1 second
}

_uConfig GlobConf;
}
// kate: indent-mode cstyle; indent-width 3; replace-tabs on; line-numbers on;
