/*!
 * \file windows/iRandR.hpp
 * \brief \b Classes: \a iRandR
 */

#ifndef E_RANDR_WIN32_HPP
#define E_RANDR_WIN32_HPP

#include "iDisplays.hpp"
#include "engine_init_Export.hpp"

namespace e_engine {

namespace windows_win32 {

class init_EXPORT iRandR {
   private:
      unsigned int vScreenWidth_uI;
      unsigned int vScreenHeight_uI;

      std::vector<iDisplays> vCurrentConfig_eD;
      std::vector<iDisplays> vPreviousConfig_eD;
      std::vector<iDisplays> vDefaultConfig_eD;
      std::vector<iDisplays> vDisplaysToChange_eD;

      void reload();

   public:
      iRandR();
      virtual ~iRandR();

      void printRandRStatus();

      bool setGamma( iDisplays const &_disp, float _r, float _g, float _b, float _brightness = 1 );

      void getMostLeftRightTopBottomCRTC( unsigned int &_left, unsigned int &_right, unsigned int &_top, unsigned int &_bottom );
      int  getIndexOfDisplay( iDisplays const &_disp ) { return -1; }

      std::vector<iDisplays> getDisplayResolutions() { return vCurrentConfig_eD; }

      bool setDisplaySizes( iDisplays const &_disp );
      bool setPrimary( const e_engine::windows_win32::iDisplays &_disp );

      bool applyNewRandRSettings();

      bool restoreScreenDefaults();
      bool restoreScreenLatest();

      bool isRandRSupported() { return true; }

      void getRandRVersion( int &_vMajor, int &_vMinor )                      {_vMajor = -1; _vMinor = -1;}
      void getScreenResolution( unsigned int &_width, unsigned int &_height ) {_width  = vScreenWidth_uI; _height = vScreenHeight_uI;}

};

/*!
 * \fn iRandR::getRandRVersion
 * \brief Does nothing
 *
 * _vMajor and _vMinor will always be -1 because Windows has no
 * special version for window resolution stuff.
 *
 * \param[out] _vMajor Major version
 * \param[out] _vMinor Minor version
 */

/*!
 * \fn iRandR::getScreenResolution
 * \brief Get the current screen resolution
 *
 * \param[out] _width  The current width
 * \param[out] _height The current height
 */

} // windows_win32

} // e_engine

#endif
// kate: indent-mode cstyle; indent-width 3; replace-tabs on; line-numbers on; remove-trailing-spaces on;
