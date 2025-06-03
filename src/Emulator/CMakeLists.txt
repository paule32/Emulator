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
project(Emulator)

# ---------------------------------------------------------------------------
# add include path for pch.h ...
# ---------------------------------------------------------------------------
add_executable (Emulator
	Emulator.cpp
	Emulator.h
    misc.cpp
)

# ---------------------------------------------------------------------------
# activate / enabled PCH ...
# ---------------------------------------------------------------------------
configure_pch(Emulator)
