# Target name
set(TARGET_TARGETNAME OsgAquarium)

#####################################################################
# Create executable
#####################################################################

add_executable(${TARGET_TARGETNAME} 
  aquarium.cpp
  fish.cpp
  headtracker.cpp
  headpositionmanipulator.cpp
  main.cpp
  screeninformation.cpp
  
  aquarium.h
  fish.h
  headtracker.h
  headpositionmanipulator.h
  screeninformation.h
)

target_compile_definitions(${TARGET_TARGETNAME}
  PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:WIN32_LEAN_AND_MEAN>
    $<$<CXX_COMPILER_ID:MSVC>:NOMINMAX>
)

target_include_directories(${TARGET_TARGETNAME}
  PRIVATE
    ${OPENSCENEGRAPH_INCLUDE_DIRS}
    ${OPENCV_INCLUDE_DIRS}
)

#####################################################################
# Linker options
#####################################################################

# Link to open gl libs
target_link_libraries(${TARGET_TARGETNAME} 
  PRIVATE
    ${OPENGL_LIBRARIES}
    ${OPENSCENEGRAPH_LIBRARIES}
    ${OPENCV_LIBRARIES} 
)

####################################################################
# Create user file for correct environment string
#####################################################################
if(MSVC)
  if(OPENSCENEGRAPH_FOUND AND OPENCV_FOUND)
    # Build path
    set(USERFILE_ENVIRONMENT_DEBUG "PATH=")
    set(USERFILE_ENVIRONMENT_RELEASE "PATH=")

    # Add OpenSceneGraph to path
    if(EXISTS "${OSG_DIR}/bin")
      set(OSG_BIN_DIR "${OSG_DIR}/bin" CACHE PATH "Path where to find OpenSceneGraph binaries")
    else()
      SET (OSG_BIN_DIR CACHE PATH "Path where to find OpenSceneGraph binaries")
    endif()
    set(USERFILE_ENVIRONMENT_DEBUG "${USERFILE_ENVIRONMENT_DEBUG}${OSG_BIN_DIR}")
    set(USERFILE_ENVIRONMENT_RELEASE "${USERFILE_ENVIRONMENT_RELEASE}${OSG_BIN_DIR}")
    
    if(OSG_THIRD_PARTY_DIR)
      # Add third party to path
      set(USERFILE_ENVIRONMENT_DEBUG "${USERFILE_ENVIRONMENT_DEBUG};${OSG_THIRD_PARTY_DIR}/bin")
      set(USERFILE_ENVIRONMENT_RELEASE "${USERFILE_ENVIRONMENT_RELEASE};${OSG_THIRD_PARTY_DIR}/bin")
    else()
      message("Warning: OpenSceneGraph 3rd Party Directory not found.")
    endif()
    
    if(OPENCV_BIN_DIR)
      # Add third party to path
      set(USERFILE_ENVIRONMENT_DEBUG "${USERFILE_ENVIRONMENT_DEBUG};${OPENCV_BIN_DIR}")
      set(USERFILE_ENVIRONMENT_RELEASE "${USERFILE_ENVIRONMENT_RELEASE};${OPENCV_BIN_DIR}")
    else()
      message("Warning: OpenCV binary Directory not found.")
    endif()

    configure_file(osgaquarium.vcxproj.template ${TARGET_TARGETNAME}.vcxproj.user @ONLY)

  endif()
endif()
