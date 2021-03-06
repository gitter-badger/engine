/*!
 * \file rObjectBase.cpp
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

#include "rObjectBase.hpp"
#include "uLog.hpp"
#include "rLoader_3D_f_OBJ.hpp"
#include "iInit.hpp"
#include <regex>
#include <boost/filesystem.hpp>

namespace e_engine {

rObjectBase::DATA_FILE_TYPE rObjectBase::detectFileTypeFromEnding( const std::string &_str ) {
   std::regex lDataEndingOBJ_ex( "\\.obj" );

   if ( std::regex_match( _str.end() - 4, _str.end(), lDataEndingOBJ_ex ) ) {
      return OBJ_FILE;
   }

   return AUTODETECT; // failed
}

/*!
 * \note This function does NOT need a working OpenGL context
 */
void rObjectBase::clearRAMData() {
   if ( vLoaderData ) {
      vLoaderData->unLoad();
      delete vLoaderData;
      vLoaderData = nullptr;
   }
}

/*!
 * \brief Clears all Data from the object
 *
 * \warning This function needs a running OpenGL context!
 *
 * \returns the return value of clearOGLData();
 */
int rObjectBase::clearAllData() {
   clearRAMData();
   return clearOGLData();
}


/*!
 * \brief Loads the data
 *
 * This function loads all relavent data from the set file.
 *
 * \warning This function wont load the data into the OpenGL context
 *
 * \note This function does NOT need a working OpenGL context
 *
 * \returns 1   - on success
 * \returns 100 - if data is already loaded
 * \returns 101 - if the file type is SET_DATA_MANUALLY
 * \returns 102 - if it was impossible to autodetect the file tye
 * \returns the return value of the loader if something went wrong during the load process
 */
int rObjectBase::loadData() {
   if ( vLoaderData ) {
      wLOG( "Object '", vName_str, "' is already loaded! You need to clear the data first" );
      return 100;
   }

   if ( vFileType == SET_DATA_MANUALLY ) {
      eLOG( "loadData was called for object '",
            vName_str,
            "' but the data file type for this object is SET_DATA_MANUALLY; return 100" );
      return 101;
   }

   if ( vFileType == AUTODETECT )
      vFileType = detectFileTypeFromEnding( vFile_str );

   if ( vFileType == AUTODETECT ) {
      eLOG( "Failed to autodetect file type from suffix! (",
            vFile_str,
            ") [OBJECT: '",
            vName_str,
            "']" );
      return 102;
   }

   boost::filesystem::path lPath( vFile_str.c_str() );

   if ( !boost::filesystem::exists( lPath ) ) {
      eLOG( "Data File '", vFile_str, "' does not exist [OBJECT: '", vName_str, "']" );
      return false;
   }

   if ( !boost::filesystem::is_regular_file( lPath ) ) {
      eLOG( "Data File '", vFile_str, "' is not a regular file [OBJECT: '", vName_str, "']" );
      return false;
   }

   switch ( vFileType ) {
      case OBJ_FILE: {
         vLoaderData = new rLoader_3D_f_OBJ( vFile_str );
         int lRet = vLoaderData->load();
         if ( lRet != 1 ) {
            delete vLoaderData;
            vLoaderData = nullptr;
            return lRet;
         }
      } break;
      case AUTODETECT:
      case SET_DATA_MANUALLY:
         eLOG( "You should never ever see this line. Please report a bug. [OBJECT: '",
               vName_str,
               "']" );
         return 1002;
   }

   vLoaderData->reindex();

   auto *lData = vLoaderData->getData();

   vObjectHints[NUM_VERTICES] = lData->vVertexData.size();
   vObjectHints[NUM_INDEXES] = lData->vIndex.size();
   vObjectHints[NUM_NORMALS] = lData->vNormalesData.size();

   return 1;
}

/*!
 * \brief Clears the content of the object
 *
 * This function clears the content of the object and prepares it for
 * rendering.
 *
 * \warning This function needs an \b ACTIVE OpenGL context for THIS THREAD
 *
 * \returns 1  if everything went fine
 * \returns 100 if data is already loaded
 * \returns 101 if there is no OpenGL context current for this thead
 * \returns the result of clearOGLData__();
 */
int rObjectBase::clearOGLData() {
   if ( !vIsLoaded_B ) {
      wLOG( "Data is already cleared [OBJECT: '", vName_str, "']" );
      return 100;
   }

   if ( !iInit::isAContextCurrentForThisThread() ) {
      eLOG( "Can NOT FREE data because no OpenGL context is current for this thread!\nThis "
            "function may be called from the destructor!  [OBJECT: '",
            vName_str,
            "']" );
      return 101;
   }

   int lRet = clearOGLData__();

   if ( lRet < 0 ) {
      wLOG( "There where errors while clearing the OpenGL data, but data is successfully cleard "
            "[OBJECT: '",
            vName_str,
            "']" );
      vIsLoaded_B = false;
      return lRet;
   }

   if ( lRet > 1 ) {
      eLOG( "There where errors while clearing the OpenGL data, and the data was NOT cleard and "
            "can still be used or cleard later [OBJECT: '",
            vName_str,
            "']" );
      return lRet;
   }

   if ( lRet == 0 ) {
      eLOG( "OBJECT '", vName_str, "' IS COMPLETELY BROKEN!" );
      return lRet;
   }

   iLOG( "Cleared data for object '", vName_str, "'" );

   vIsLoaded_B = false;
   return lRet;
}


/*!
 * \brief Loads the content of the object
 *
 * This function loads the content of the object and prepares it for
 * rendering.
 *
 * \warning This function needs an \b ACTIVE OpenGL context for THIS THREAD
 *
 * \returns 1  if everything went fine
 * \returns 100 if there is no OpenGL context current for this thead
 * \returns 101 if data is already loaded
 * \returns 102 if data is not in RAM
 * \returns the result of setOGLData__();
 */
int rObjectBase::setOGLData() {
   if ( vIsLoaded_B ) {
      wLOG( "Data is already present in the OpenGL buffers [OBJECT: '", vName_str, "']" );
      return 101;
   }

   if ( !iInit::isAContextCurrentForThisThread() ) {
      eLOG( "Cannot init data because no OpenGL context is current for this thread!" );
      return 100;
   }

   if ( !vLoaderData ) {
      eLOG( "The OpenGL Data is not present in RAM! Cannot copy to OpenGL buffers! [OBJECT: '",
            vName_str,
            "']" );
      eLOG( "Use loadData() to load the data." );
      return 102;
   }

   int lRet = setOGLData__();

   if ( lRet < 0 ) {
      wLOG( "There where errors while setting the OpenGL data, but data is successfully set "
            "[OBJECT: '",
            vName_str,
            "']" );
      vIsLoaded_B = true;
      return lRet;
   }

   if ( lRet > 1 ) {
      eLOG( "There where errors while setting the OpenGL data, and the data was NOT set and can "
            "still be used or set later [OBJECT: '",
            vName_str,
            "']" );
      return lRet;
   }

   if ( lRet == 0 ) {
      eLOG( "OBJECT '", vName_str, "' IS COMPLETELY BROKEN!" );
      return lRet;
   }

   iLOG( "Loaded data for object '", vName_str, "'" );

   vIsLoaded_B = true;

   if ( !vKeepDataInRAM_B )
      clearRAMData();

   return lRet;
}

#if COMPILER_CLANG
#pragma clang diagnostic push // This warning is irrelevant here
#pragma clang diagnostic ignored "-Wunused-parameter"
#endif

#if COMPILER_GCC
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

/*!
 * \brief Get the _n'th VBO
 *
 * You can query the number of VBO's from the hints
 *
 * \param[in,out] _n In: which (starting at 0) VBO; Out: the VBO
 * \returns 0 if the requested VBO exists and ERROR_FLAGS flags if not
 *
 */
uint32_t rObjectBase::getVBO( GLuint &_n ) { return FUNCTION_NOT_VALID_FOR_THIS_OBJECT; }

/*!
 * \brief Get the _n'th IBO
 *
 * You can query the number of IBO's from the hints
 *
 * \param[in,out] _n In: which (starting at 0) IBO; Out: the IBO
 * \returns 0 if the requested IBO exists and ERROR_FLAGS flags if not
 */
uint32_t rObjectBase::getIBO( GLuint &_n ) { return FUNCTION_NOT_VALID_FOR_THIS_OBJECT; }

/*!
 * \brief Get the _n'th NBO
 *
 * You can query the number of NBO's from the hints
 *
 * \param[in,out] _n In: which (starting at 0) NBO; Out: the NBO
 * \returns 0 if the requested NBO exists and ERROR_FLAGS flags if not
 */
uint32_t rObjectBase::getNBO( GLuint &_n ) { return FUNCTION_NOT_VALID_FOR_THIS_OBJECT; }

/*!
 * \brief Get the _type Matrix
 *
 * \param[out] _mat The Matrix pointer
 * \param[in]  _type The Matrix type
 * \returns 0 if the requested Matrix exists and ERROR_FLAGS flags if not
 */
uint32_t rObjectBase::getMatrix( rMat4d **_mat, rObjectBase::MATRIX_TYPES _type ) {
   *_mat = nullptr;
   return FUNCTION_NOT_VALID_FOR_THIS_OBJECT;
}

/*!
 * \brief Get the _type Matrix
 *
 * \param[out] _mat The Matrix pointer
 * \param[in]  _type The Matrix type
 * \returns 0 if the requested Matrix exists and ERROR_FLAGS flags if not
 */
uint32_t rObjectBase::getMatrix( rMat4f **_mat, rObjectBase::MATRIX_TYPES _type ) {
   *_mat = nullptr;
   return FUNCTION_NOT_VALID_FOR_THIS_OBJECT;
}

/*!
 * \brief Get the _type Matrix
 *
 * \param[out] _mat The Matrix pointer
 * \param[in]  _type The Matrix type
 * \returns 0 if the requested Matrix exists and ERROR_FLAGS flags if not
 */
uint32_t rObjectBase::getMatrix( rMat3d **_mat, rObjectBase::MATRIX_TYPES _type ) {
   *_mat = nullptr;
   return FUNCTION_NOT_VALID_FOR_THIS_OBJECT;
}

/*!
 * \brief Get the _type Matrix
 *
 * \param[out] _mat The Matrix pointer
 * \param[in]  _type The Matrix type
 * \returns 0 if the requested Matrix exists and ERROR_FLAGS flags if not
 */
uint32_t rObjectBase::getMatrix( rMat3f **_mat, rObjectBase::MATRIX_TYPES _type ) {
   *_mat = nullptr;
   return FUNCTION_NOT_VALID_FOR_THIS_OBJECT;
}



/*!
 * \brief Get the _type Vector
 *
 * \param[out] _vec The Vector pointer
 * \param[in]  _type The Vector type
 * \returns 0 if the requested Vector exists and ERROR_FLAGS flags if not
 */
uint32_t rObjectBase::getVector( rVec4d **_vec, rObjectBase::VECTOR_TYPES _type ) {
   *_vec = nullptr;
   return FUNCTION_NOT_VALID_FOR_THIS_OBJECT;
}

/*!
 * \brief Get the _type Vector
 *
 * \param[out] _vec The Vector pointer
 * \param[in]  _type The Vector type
 * \returns 0 if the requested Vector exists and ERROR_FLAGS flags if not
 */
uint32_t rObjectBase::getVector( rVec4f **_vec, rObjectBase::VECTOR_TYPES _type ) {
   *_vec = nullptr;
   return FUNCTION_NOT_VALID_FOR_THIS_OBJECT;
}


/*!
 * \brief Get the _type Vector
 *
 * \param[out] _vec The Vector pointer
 * \param[in]  _type The Vector type
 * \returns 0 if the requested Vector exists and ERROR_FLAGS flags if not
 */
uint32_t rObjectBase::getVector( rVec3d **_vec, rObjectBase::VECTOR_TYPES _type ) {
   *_vec = nullptr;
   return FUNCTION_NOT_VALID_FOR_THIS_OBJECT;
}

/*!
 * \brief Get the _type Vector
 *
 * \param[out] _vec The Vector pointer
 * \param[in]  _type The Vector type
 * \returns 0 if the requested Vector exists and ERROR_FLAGS flags if not
 */
uint32_t rObjectBase::getVector( rVec3f **_vec, rObjectBase::VECTOR_TYPES _type ) {
   *_vec = nullptr;
   return FUNCTION_NOT_VALID_FOR_THIS_OBJECT;
}

#if COMPILER_CLANG
#pragma clang diagnostic pop
#endif

#if COMPILER_GCC
#pragma GCC diagnostic pop
#endif



} // e_engine

// kate: indent-mode cstyle; indent-width 3; replace-tabs on; line-numbers on;
