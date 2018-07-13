#CMake file for skeletonize src
cmake_minimum_required(VERSION 3.5)

set(QT_DIR /opt/Trolltech/Qt4.8.5)
set(Neuroproof_minimalDir /home/brian/software/Neuroproof_minimal)
SET(Neuroproof_minimalBuildDir ${Neuroproof_minimalDir}/build)
SET(minicondaDir /home/brian/software/miniconda2/envs/neuroproof_env/lib)

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG_")
SET(NeurolabiDir ../neurolabi)
SET(GuiDir ${NeurolabiDir}/gui)
SET(NeurolabiLibDir ${NeurolabiDir}/c/lib)
SET(NeutubeLibDir ${NeurolabiDir}/cpp/lib/build)
SET(EXTLIB_DIR ${NeurolabiDir}/lib)

#SET(jsonLib jansson)
set(CMAKE_PREFIX_PATH ${EXTLIB_DIR}/jansson)
find_library(jsonLib LIBRARY NAMES jansson)

set(CMAKE_PREFIX_PATH ${minicondaDir}/..)
set(CMAKE_PREFIX_PATH ${QT_DIR}/lib)
set(CMAKE_PREFIX_PATH ${minicondaDir})

set(CMAKE_PREFIX_PATH ${EXTLIB_DIR}/fftw3)
find_library(fftw3Lib LIBRARY NAMES fftw3)
find_library(fftw3fLib LIBRARY NAMES fftw3f)
set(CMAKE_PREFIX_PATH ${EXTLIB_DIR}/xml)
find_library(xmlLib LIBRARY NAMES xml2)
set(CMAKE_PREFIX_PATH ${EXTLIB_DIR}/png)
find_library(pngLib LIBRARY NAMES png)
set(CMAKE_PREFIX_PATH ${EXTLIB_DIR}/hdf5-1.8.12/src/.libs)
find_library(hdfLib LIBRARY NAMES hdf5)
set(CMAKE_PREFIX_PATH ${NeurolabiLibDir})
find_library(neuLib LIBRARY NAMES neurolabi)
set(CMAKE_PREFIX_PATH ${NeutubeLibDir})
find_library(neutubeLib LIBRARY NAMES neutube)
set(CMAKE_PREFIX_PATH ${Neuroproof_minimalBuildDir})
find_library(neuroproofLib LIBRARY NAMES Neuroproof)
set(CMAKE_PREFIX_PATH ${minicondaDir})
find_library(boost_systemLib LIBRARY NAMES boost_system)
find_library(boost_threadLib LIBRARY NAMES boost_thread)
find_library(boost_chronoLib LIBRARY NAMES boost_chrono)
find_library(opencv_coreLib LIBRARY NAMES opencv_core)
find_library(opencv_mlLib LIBRARY NAMES opencv_ml)
find_library(vigraLib LIBRARY NAMES vigraimpex)
find_library(hdf5Lib LIBRARY NAMES hdf5)
find_library(hdf5_hlLib LIBRARY NAMES hdf5_hl)

find_library(zLib LIBRARY NAMES z)
find_library(dlLib LIBRARY NAMES dl)
find_library(QtCLuceneLib LIBRARY NAMES QtCLucene)
find_library(QtCoreLib LIBRARY NAMES QtCore)
find_library(QtDeclarativeLib LIBRARY NAMES QtDeclarative)
find_library(QtDesignerComponentsLib LIBRARY NAMES QtDesignerComponents)
find_library(QtDesignerLib LIBRARY NAMES QtDesigner)
find_library(QtGuiLib LIBRARY NAMES QtGui)
find_library(QtHelpLib LIBRARY NAMES QtHelp)
find_library(QtMultimediaLib LIBRARY NAMES QtMultimedia)
find_library(QtNetworkLib LIBRARY NAMES QtNetwork)
find_library(QtOpenGLLib LIBRARY NAMES QtOpenGL)
find_library(QtScriptLib LIBRARY NAMES QtScript)
find_library(QtScriptToolsLib LIBRARY NAMES QtScriptTools)
find_library(QtSqlLib LIBRARY NAMES QtSql)
find_library(QtSvgLib LIBRARY NAMES QtSvg)
find_library(QtTestLib LIBRARY NAMES QtTest)
find_library(QtXmlPatternsLib LIBRARY NAMES QtXmlPatterns)
find_library(QtXmlLib LIBRARY NAMES QtXml)


message("Json library: ${jsonLib}")
message("fftw3 library: ${fftw3Lib}")
message("fftw3f library: ${fftw3fLib}")
message("xml library: ${xmlLib}")
message("png library: ${pngLib}")
message("png library: ${hdfLib}")
message("neurolabi library: ${neuLib}")
message("neutube library: ${neutubeLib}")
message("libz library: ${zLib}")
message("dl library: ${dlLib}")

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${NeurolabiDir}/c 
  ${NeurolabiDir}/c/include 
  ${NeurolabiDir}/gui/ext 
  ${NeurolabiDir}/gui 
  ${EXTLIB_DIR}/genelib/src
  ${EXTLIB_DIR}/xml/include/libxml2
  ${EXTLIB_DIR}/fftw3/include
  ${EXTLIB_DIR}/png/include
  ${EXTLIB_DIR}/jansson/include
  ${EXTLIB_DIR}/hdf5-1.8.12/src
  ${EXTLIB_DIR}/hdf5-1.8.12/hl/src
  ${minicondaDir}/../include/
  ${Neuroproof_minimalDir}
  ${QT_DIR}/include
  ${QT_DIR}/include/Qt
  ${QT_DIR}/include/Qt3Support
  ${QT_DIR}/include/QtCore
  ${QT_DIR}/include/QtDeclarative
  ${QT_DIR}/include/QtDesigner
  ${QT_DIR}/include/QtGui
  ${QT_DIR}/include/QtHelp
  ${QT_DIR}/include/QtMultimedia
  ${QT_DIR}/include/QtNetwork
  ${QT_DIR}/include/QtOpenGL
  ${QT_DIR}/include/QtScript
  ${QT_DIR}/include/QtScriptTools
  ${QT_DIR}/include/QtSql
  ${QT_DIR}/include/QtSvg
  ${QT_DIR}/include/QtTest
  ${QT_DIR}/include/QtUiTools
  ${QT_DIR}/include/QtXml
  ${QT_DIR}/include/QtXmlPatterns
  ${Neuroproof_minimalDir}/FeatureManager
  ${Neuroproof_minimalDir}/Classifier
  ${Neuroproof_minimalDir}/Algorithms
  ${Neuroproof_minimalDir}/DataStructures
  ${Neuroproof_minimalDir}
  )

add_executable(get_skeleton myskeletonwrapper.cpp segmentation_interface.cpp get_skeleton.cpp)
TARGET_LINK_LIBRARIES(get_skeleton 
 ${neutubeLib}  ${neuLib}  ${neuroproofLib}
   ${QtCLuceneLib} ${QtCoreLib}
  ${QtDeclarativeLib} ${QtDesignerComponentsLib} ${QtDesignerLib}
  ${QtGuiLib} ${QtHelpLib} ${QtMultimediaLib} ${QtNetworkLib}
  ${QtOpenGLLib} ${QtScriptLib} ${QtScriptToolsLib} ${QtSqlLib}
  ${QtSvgLib} ${QtTestLib} ${QtXmlPatternsLib} ${QtXmlLib}
  ${jsonLib} ${fftw3Lib} ${fftw3fLib} ${xmlLib}
  ${opencv_coreLib} ${opencv_mlLib} ${vigraLib}
  ${pngLib} ${zLib} ${dlLib} ${hdfLib} ${hdf5Lib}
  ${hdf5_hlLib} ${boost_threadLib} ${boost_systemLib} ${boost_chronoLib}
  )



