
#include "Protect.h"

#ifdef WIN32
#include <Windows.h>

#define SECURE_MEMORY_BLOCK CRYPTPROTECTMEMORY_BLOCK_SIZE
#else
#define SECURE_MEMORY_BLOCK 8
#endif

bool ProtectMemory(void* data, size_t len)
{
#ifdef WIN32
	return CryptProtectMemory(data, static_cast<DWORD>(len), CRYPTPROTECTMEMORY_SAME_PROCESS) != FALSE;
#else
	if (len % SECURE_MEMORY_BLOCK != 0)
		return false;

	const uint64_t x = 0xdeadbee5f00dbeef;
	uint64_t* p = reinterpret_cast<uint64_t*>(data);

	for (size_t i = 0; i < len / SECURE_MEMORY_BLOCK; ++i)
	{
		*p = *p ^ x;
	}
	return true;
#endif
}

bool UnprotectMemory(void* data, size_t len)
{
#ifdef WIN32
	return CryptUnprotectMemory(data, static_cast<DWORD>(len), CRYPTPROTECTMEMORY_SAME_PROCESS) != FALSE;
#else
	return ProtectMemory(data, len);
#endif
}

void CleanMemory(void* data, size_t len)
{
#ifdef WIN32
	SecureZeroMemory(data, len);
#else
	volatile char* p = reinterpret_cast<char*>(data);
	for (size_t i = 0; i < len; ++i)
	{
		*p = 0;
	}
#endif
}

void Protect(void * data, size_t len)
{
	if (!ProtectMemory(data, len))
	{
		std::cerr << "Error: String protection failed\n";
		std::terminate();
	}
}

void Unprotect(void * data, size_t len)
{
	if (!UnprotectMemory(data, len))
	{
		std::cerr << "Error: String unprotection failed\n";
		std::terminate();
	}
}

