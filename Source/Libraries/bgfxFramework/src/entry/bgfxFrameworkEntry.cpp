/*
 * Copyright 2011-2024 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx/blob/master/LICENSE
 */

#include <bx/bx.h>
#include <bx/file.h>
#include <bx/sort.h>
#include <bgfx/bgfx.h>

#include <time.h>

#if BX_PLATFORM_EMSCRIPTEN
#	include <emscripten.h>
#endif // BX_PLATFORM_EMSCRIPTEN

#include "entry_p.h"

namespace entry
{
	static uint32_t s_debug = BGFX_DEBUG_NONE;
	static uint32_t s_reset = BGFX_RESET_NONE;
	static uint32_t s_width = ENTRY_DEFAULT_WIDTH;
	static uint32_t s_height = ENTRY_DEFAULT_HEIGHT;

	static bx::FileReaderI* s_fileReader = NULL;
	static bx::FileWriterI* s_fileWriter = NULL;

	extern bx::AllocatorI* getDefaultAllocator();
	bx::AllocatorI* g_allocator = getDefaultAllocator();

	typedef bx::StringT<&g_allocator> String;

	static String s_currentDir;

	class FileReader : public bx::FileReader
	{
		typedef bx::FileReader super;

	public:
		virtual bool open(const bx::FilePath& _filePath, bx::Error* _err) override
		{
			String filePath(s_currentDir);
			filePath.append(_filePath);
			return super::open(filePath.getPtr(), _err);
		}
	};

	class FileWriter : public bx::FileWriter
	{
		typedef bx::FileWriter super;

	public:
		virtual bool open(const bx::FilePath& _filePath, bool _append, bx::Error* _err) override
		{
			String filePath(s_currentDir);
			filePath.append(_filePath);
			return super::open(filePath.getPtr(), _append, _err);
		}
	};

	void setCurrentDir(const char* _dir)
	{
		s_currentDir.set(_dir);
	}

#if ENTRY_CONFIG_IMPLEMENT_DEFAULT_ALLOCATOR
	bx::AllocatorI* getDefaultAllocator()
	{
BX_PRAGMA_DIAGNOSTIC_PUSH();
BX_PRAGMA_DIAGNOSTIC_IGNORED_MSVC(4459); // warning C4459: declaration of 's_allocator' hides global declaration
BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC("-Wshadow");
		static bx::DefaultAllocator s_allocator;
		return &s_allocator;
BX_PRAGMA_DIAGNOSTIC_POP();
	}
#endif // ENTRY_CONFIG_IMPLEMENT_DEFAULT_ALLOCATOR

	bool setOrToggle(uint32_t& _flags, const char* _name, uint32_t _bit, int _first, int _argc, char const* const* _argv)
	{
		if (0 == bx::strCmp(_argv[_first], _name) )
		{
			int arg = _first+1;
			if (_argc > arg)
			{
				_flags &= ~_bit;

				bool set = false;
				bx::fromString(&set, _argv[arg]);

				_flags |= set ? _bit : 0;
			}
			else
			{
				_flags ^= _bit;
			}

			return true;
		}

		return false;
	}

	void Initialize()
	{
		s_fileReader = BX_NEW(g_allocator, FileReader);
		s_fileWriter = BX_NEW(g_allocator, FileWriter);
	}

	void Shutdown()
	{
		bx::deleteObject(g_allocator, s_fileReader);
		s_fileReader = NULL;

		bx::deleteObject(g_allocator, s_fileWriter);
		s_fileWriter = NULL;
	}

	bx::FileReaderI* getFileReader() // #TODO Also defined in entry.cpp
	{
		return s_fileReader;
	}

	bx::FileWriterI* getFileWriter()
	{
		return s_fileWriter;
	}

	bx::AllocatorI* getAllocator()
	{
		if (NULL == g_allocator)
		{
			g_allocator = getDefaultAllocator();
		}

		return g_allocator;
	}

	void* TinyStlAllocator::static_allocate(size_t _bytes)
	{
		return bx::alloc(getAllocator(), _bytes);
	}

	void TinyStlAllocator::static_deallocate(void* _ptr, size_t /*_bytes*/)
	{
		if (NULL != _ptr)
		{
			bx::free(getAllocator(), _ptr);
		}
	}

} // namespace entry
