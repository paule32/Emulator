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
  # define WIN32_LEAN_AND_MEAN
  # define _AFXDLL
  # include <winsock2.h>
  # include <ws2tcpip.h>
  # include <windows.h>
  # include <afxwin.h>
#elif defined(__linux__) || defined(__APPLE__)
  # include <unistd.h>
  # include <limits.h>
#endif

# include <iostream>
# include <vector>
# include <string>

# include <filesystem> // C++17
