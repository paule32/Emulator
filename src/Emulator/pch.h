// ---------------------------------------------------------------------------
// File:   pch.h
// Author: Jens Kallup - paule32
//
// This file is part of Emulator.
//
// (c) Copyright 2025 Jens Kallup - paule32
// all rights reserved.
// 
// only for non-profit usage !!!
// ---------------------------------------------------------------------------
#pragma once

# include <stdio.h>
# include <stdint.h>
# include <stdlib.h>
# include <string.h>

#if defined(_WIN32)
  # include <windows.h>
#elif defined(__linux__) || defined(__APPLE__)
  # include <unistd.h>
  # include <limits.h>
#endif

# include <iostream>
# include <vector>
# include <string>

# include <filesystem> // C++17
