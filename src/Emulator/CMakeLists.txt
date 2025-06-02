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
set(CMAKE_CXX_STANDARD 20)

add_executable (Emulator
	Emulator.cpp
	Emulator.h
	pch.h
 "misc.cpp")

# add pre-compiled header
target_precompile_headers(Emulator PRIVATE pch.h)
