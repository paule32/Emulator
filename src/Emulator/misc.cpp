// ---------------------------------------------------------------------------
// File:   misc.cpp
// Author: Jens Kallup - paule32
//
// This file is part of Emulator.
//
// (c) Copyright 2025 Jens Kallup - paule32
// all rights reserved.
// 
// only for non-profit usage !!!
// ---------------------------------------------------------------------------
# include "Emulator.h"

std::string getExecutablePath()
{
#if defined(_WIN32)
    char buffer[MAX_PATH];
    DWORD length = GetModuleFileNameA(NULL, buffer, MAX_PATH);
    if (length == 0 || length == MAX_PATH)
        return {};
    return std::string(buffer, length);

#elif defined(__linux__)
    char buffer[PATH_MAX];
    ssize_t length = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    if (length == -1)
        return {};
    buffer[length] = '\0';
    return std::string(buffer);

#elif defined(__APPLE__)
    char buffer[PATH_MAX];
    uint32_t size = sizeof(buffer);
    if (_NSGetExecutablePath(buffer, &size) != 0)
        return {};
    return std::string(buffer);

#else
    return {};
#endif
}
