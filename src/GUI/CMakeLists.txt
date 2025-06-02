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

add_executable (GUI
	GUI.cpp
	GUI.h
)

# ---------------------------------------------------------------------------
# activate / enabled PCH ...
# ---------------------------------------------------------------------------
configure_pch(GUI)
