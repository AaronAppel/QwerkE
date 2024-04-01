/*
 * Copyright 2011-2024 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx/blob/master/LICENSE
 */

#ifndef ENTRY_H_HEADER_GUARD
#define ENTRY_H_HEADER_GUARD

#include "../../../src/entry/dbg.h"
#include <bgfx/bgfx.h>
#include <bx/bx.h>
#include <bx/filepath.h>
#include <bx/string.h>

namespace bx { struct FileReaderI; struct FileWriterI; struct AllocatorI; }

extern "C" int _main_(int _argc, char** _argv);

#define ENTRY_WINDOW_FLAG_NONE         UINT32_C(0x00000000)
#define ENTRY_WINDOW_FLAG_ASPECT_RATIO UINT32_C(0x00000001)
#define ENTRY_WINDOW_FLAG_FRAME        UINT32_C(0x00000002)

#ifndef ENTRY_CONFIG_IMPLEMENT_MAIN
#	define ENTRY_CONFIG_IMPLEMENT_MAIN 0
#endif // ENTRY_CONFIG_IMPLEMENT_MAIN

#if ENTRY_CONFIG_IMPLEMENT_MAIN
#define ENTRY_IMPLEMENT_MAIN(_app, ...)                 \
	int _main_(int _argc, char** _argv)                 \
	{                                                   \
			_app app(__VA_ARGS__);                      \
			return entry::runApp(&app, _argc, _argv);   \
	}
#else
#define ENTRY_IMPLEMENT_MAIN(_app, ...) \
	_app s_ ## _app ## App(__VA_ARGS__)
#endif // ENTRY_CONFIG_IMPLEMENT_MAIN

///
#define ENTRY_HANDLE(_name)                                                \
	struct _name { uint16_t idx; };                                        \
	inline bool isValid(_name _handle) { return UINT16_MAX != _handle.idx; }

namespace entry
{
	ENTRY_HANDLE(WindowHandle);
	ENTRY_HANDLE(GamepadHandle);

	void Initialize();
	void Shutdown();

	///
	constexpr WindowHandle kDefaultWindowHandle = { 0 };

	///
	bx::FileReaderI* getFileReader();

	///
	bx::FileWriterI* getFileWriter();

	///
	bx::AllocatorI*  getAllocator();

	void* getNativeDisplayHandle()
	{
		return NULL;
	}

	bgfx::NativeWindowHandleType::Enum getNativeWindowHandleType()
	{
		return bgfx::NativeWindowHandleType::Default;
	}

	///
	void setCurrentDir(const char* _dir);

	///
	struct MouseButton
	{
		enum Enum
		{
			None,
			Left,
			Middle,
			Right,

			Count
		};
	};

	///
	struct MouseState
	{
		MouseState()
			: m_mx(0)
			, m_my(0)
			, m_mz(0)
		{
			for (uint32_t ii = 0; ii < entry::MouseButton::Count; ++ii)
			{
				m_buttons[ii] = entry::MouseButton::None;
			}
		}

		int32_t m_mx;
		int32_t m_my;
		int32_t m_mz;
		uint8_t m_buttons[entry::MouseButton::Count];
	};

} // namespace entry

#endif // ENTRY_H_HEADER_GUARD
