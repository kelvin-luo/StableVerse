if (NOT CMAKE_BUILD_TYPE STREQUAL "Release")
    find_program(CLANGTIDY clang-tidy PATHS "D:/LLVM/bin" "D:\\binWin10\\LLVM\\bin")
    message({clang-tidy_VERSION}= ${clang-tidy_VERSION})
    message("CXX_CLANG_TIDY_EXPORT_FIXES_DIR= ${CXX_CLANG_TIDY_EXPORT_FIXES_DIR}")
    if (CLANGTIDY)
        message(STATUS "Using clang-tidy, found ${CLANGTIDY}")
        set(CMAKE_CXX_CLANG_TIDY "${CLANGTIDY};-extra-arg=-Wno-unknown-warning-option")
        # Explicitly enable exceptions on Windows
        if (WIN32)
            set(CMAKE_CXX_CLANG_TIDY "${CMAKE_CXX_CLANG_TIDY};--extra-arg=/EHsc")
        endif ()

    else ()
        message(SEND_ERROR "clang-tidy requested but executable not found")
    endif ()

    # This will gradually increase memory usage of the program,
    # running on Apple M1, 13.0.
    message(STATUS "Using address sanitizer")


    if (NOT WIN32)
        message(STATUS "Using address sanitizer")
        add_compile_options(-O0 -fsanitize=address -g)
        add_link_options(-O0 -fsanitize=address -g)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fsanitize=address -g")
    endif ()

endif ()
