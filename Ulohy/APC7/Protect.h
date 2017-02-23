#pragma once

#include <iostream>

#ifdef WIN32
#include <Windows.h>

#define SECURE_MEMORY_BLOCK CRYPTPROTECTMEMORY_BLOCK_SIZE
#else
#define SECURE_MEMORY_BLOCK 8
#endif

bool ProtectMemory(void* data, size_t len);
bool UnprotectMemory(void* data, size_t len);
void CleanMemory(void* data, size_t len);

void Protect(void* data, size_t len);
void Unprotect(void* data, size_t len);
