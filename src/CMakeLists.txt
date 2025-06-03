# ---------------------------------------------------------------------------
# File:   CMakeLists.txt
# Author: Jens Kallup - paule32
#
# This file is part of Emulator.
#
# (c) Copyright 2025 Jens Kallup - paule32
# all rights reserved.
#
# only for non-profit usage !!!
# ---------------------------------------------------------------------------
cmake_minimum_required (VERSION 3.20)
project ("Emulator")

# ---------------------------------------------------------------------------
# activate / enable C++ standard 20
# ---------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# global PCH file (pre-compiled-headers)
# add pre-compiled header ...
# ---------------------------------------------------------------------------
set(PCH_HEADER "${CMAKE_SOURCE_DIR}/pch.h")
function(configure_pch target)
    target_precompile_headers(${target} PRIVATE ${PCH_HEADER})
endfunction()

# ---------------------------------------------------------------------------
# sub projects ...
# ---------------------------------------------------------------------------
add_subdirectory ("Emulator")
