# Check that people use the good file
if(NOT TOP_CMAKE_WAS_SOURCED)
    message(FATAL_ERROR "
    You did not 'cmake' the good CMakeLists.txt file. Use the one in the top dir.
    It is advice to delete all wrongly generated cmake stuff => CMakeFiles & CMakeCache.txt")
endif()


# plugin name (no version number to ease future version bump and bisect)
set(Output GSdx)

set(CommonFlags
    -fno-operator-names # because Xbyak uses and()/xor()/or()/not() function
    -Wno-unknown-pragmas
    -Wno-parentheses
    -Wunused-variable # __dummy variable need to be investigated
    )

# The next two need to be looked at, but spam really badly in gcc 8.
# Largely class alignment in GSDevice.h and memcpy in GSVector*.h.
if(GCC_VERSION VERSION_EQUAL "8.0" OR GCC_VERSION VERSION_GREATER "8.0")
    set(CommonFlags
	    ${CommonFlags}
        -Wno-class-memaccess
        -Wno-packed-not-aligned
    )
endif()

if(USE_ICC)
    set(CommonFlags "${CommonFlags} -restrict")
endif()

set(GSdxFinalFlags ${CommonFlags})

if(XDG_STD)
    set(GSdxFinalFlags ${GSdxFinalFlags} -DXDG_STD)
endif()

set(GSdxFinalLibs
    ${X11_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${EGL_LIBRARIES}
    ${X11_XCB_LIBRARIES}
    ${GTK2_LIBRARIES}
    ${LIBC_LIBRARIES}
    ${PNG_LIBRARIES}
    ${FREETYPE_LIBRARIES}
    ${LIBLZMA_LIBRARIES}
)
set(RESOURCE_FILES
    res/logo-ogl.bmp
    res/fxaa.fx
    res/glsl/common_header.glsl
    res/glsl/convert.glsl
    res/glsl/interlace.glsl
    res/glsl/merge.glsl
    res/glsl/shadeboost.glsl
    res/glsl/tfx_fs.glsl
    res/glsl/tfx_vgs.glsl)

add_custom_glib_res("GSdxResources" "gsdx-res.xml" "GSdx_res" ${RESOURCE_FILES})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

if(BUILTIN_GS)
    add_pcsx2_lib(${Output} "${GSdxFinalSources}" "${GSdxFinalLibs}" "${GSdxFinalFlags}")
else()
    if (DISABLE_ADVANCE_SIMD)
        # Don't append -SSE2 on the first build to keep same name as SIMD build
        add_pcsx2_plugin("${Output}" "${GSdxFinalSources}" "${GSdxFinalLibs}" "${GSdxFinalFlags} -msse3 -msse4 -msse4.1")
        add_pcsx2_plugin("${Output}-AVX2" "${GSdxFinalSources}" "${GSdxFinalLibs}" "${GSdxFinalFlags} -mavx -mavx2 -mbmi -mbmi2")
        target_compile_features("${Output}-AVX2" PRIVATE cxx_std_17)
    else()
        add_pcsx2_plugin(${Output} "${GSdxFinalSources}" "${GSdxFinalLibs}" "${GSdxFinalFlags}")
    endif()
endif()

target_compile_features(${Output} PRIVATE cxx_std_17)

################################### Replay Loader
if(BUILD_REPLAY_LOADERS)
    set(Replay pcsx2_GSReplayLoader)
    set(GSdxReplayLoaderFinalSources
        linux_replay.cpp
    )
    add_pcsx2_executable(${Replay} "${GSdxReplayLoaderFinalSources}" "${LIBC_LIBRARIES}" "${GSdxFinalFlags}")
    target_compile_features(${Replay} PRIVATE cxx_std_17)
endif(BUILD_REPLAY_LOADERS)
