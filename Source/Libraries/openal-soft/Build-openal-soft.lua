project "openal-soft"
	kind "StaticLib"
	location ""
	-- staticruntime "off" -- https://premake.github.io/docs/staticruntime
	language "C++"
	cppdialect "C++20"
	
	filter "configurations:*"
		architecture "x64"
	
	files
	{
		"build/default_hrtf.txt",
		
		-- gsl/include
		"gsl/include/gsl/algorithm",
		"gsl/include/gsl/assert",
		"gsl/include/gsl/byte",
		"gsl/include/gsl/gsl",
		"gsl/include/gsl/narrow",
		"gsl/include/gsl/pointers",
		"gsl/include/gsl/span",
		"gsl/include/gsl/span_ext",
		"gsl/include/gsl/util",
		"gsl/include/gsl/zstring",
		
		-- .h files
		-- .h al
		"al/auxeffectslot.h",
		"al/buffer.h",
		"al/debug.h",
		"al/direct_defs.h",
		"al/effect.h",
		"al/event.h",		
		"al/filter.h",
		"al/listener.h",
		"al/source.h",
		
		"al/eax/api.h",		
		"al/eax/call.h",
		"al/eax/exception.h",
		"al/eax/effect.h",
		"al/eax/fx_slot_index.h",
		"al/eax/fx_slots.h",
		"al/eax/globals.h",
		"al/eax/utils.h",
		"al/eax/x_ram.h",
		
		"al/effects/effects.h",
		
		-- .h alc
		"alc/alconfig.h",
		"alc/alu.h",
		"alc/context.h",
		"alc/device.h",		
		"alc/events.h",		
		"alc/export_list.h",
		"alc/inprogext.h",
		
		"alc/backends/base.h",
		"alc/backends/dsound.h",
		"alc/backends/loopback.h",
		"alc/backends/null.h",
		"alc/backends/wasapi.h",
		"alc/backends/wave.h",
		"alc/backends/winmm.h",
		
		"alc/effects/base.h",		
		
		-- .h common
		"common/albit.h",
		"common/alcomplex.h",
		"common/almalloc.h",		
		"common/alnumeric.h",
		"common/alstring.h",
		"common/althrd_setname.h",
		"common/althreads.h",
		"common/atomic.h",
		"common/comptr.h",
		"common/dynload.h",
		"common/expected.hpp",
		"common/filesystem.h",
		"common/flexarray.h",
		"common/intrusive_ptr.h",
		"common/opthelpers.h",
		"common/pffft.h",
		"common/phase_shifter.h",
		"common/polyphase_resampler.h",
		"common/pragmadefs.h",
		"common/ringbuffer.h",
		"common/strutils.hpp",
		"common/vecmat.h",
		"common/vector.h",
		
		-- .h core
		"core/ambdec.h",
		"core/ambidefs.h",
		"core/async_event.h",
		"core/bformatdec.h",
		"core/bs2b.h",
		"core/bsinc_defs.h",
		"core/bsinc_tables.h",
		"core/buffer_storage.h",
		"core/bufferline.h",
		"core/context.h",
		"core/converter.h",
		"core/cpu_caps.h",
		"core/cubic_defs.h",
		"core/cubic_tables.h",
		"core/devformat.h",
		"core/device.h",
		"core/effectslot.h",
		"core/except.h",
		"core/fmt_traits.h",
		"core/fpu_ctrl.h",
		"core/front_stablizer.h",
		"core/helpers.h",
		"core/hrtf.h",
		"core/logging.h",
		"core/mastering.h",
		"core/resampler_limits.h",
		"core/storage_formats.h",
		"core/uhjfilter.h",
		"core/voice.h",
		"core/voice_change.h",
		
		"core/effects/base.h",
		
		"core/filters/biquad.h",
		"core/filters/nfc.h",
		"core/filters/splitter.h",
		
		-- .h core mixer
		"core/mixer.h",
		"core/mixer/defs.h",
		"core/mixer/hrtfbase.h",
		"core/mixer/hrtfdefs.h",
		
		-- .cpp files
		-- .cpp al
		"al/auxeffectslot.cpp",
		"al/buffer.cpp",
		"al/debug.cpp",
		"al/effect.cpp",
		"al/error.cpp",
		"al/event.cpp",
		"al/extension.cpp",
		"al/filter.cpp",
		"al/listener.cpp",
		"al/source.cpp",
		"al/state.cpp",
		
		"al/eax/api.cpp",
		"al/eax/call.cpp",
		"al/eax/exception.cpp",
		"al/eax/fx_slot_index.cpp",
		"al/eax/fx_slots.cpp",
		"al/eax/utils.cpp",
		
		"al/effects/autowah.cpp",
		"al/effects/compressor.cpp",
		"al/effects/chorus.cpp",
		"al/effects/convolution.cpp",
		"al/effects/dedicated.cpp",
		"al/effects/distortion.cpp",
		"al/effects/echo.cpp",
		"al/effects/effects.cpp",
		"al/effects/equalizer.cpp",
		"al/effects/fshifter.cpp",
		"al/effects/modulator.cpp",
		"al/effects/null.cpp",
		"al/effects/pshifter.cpp",
		"al/effects/reverb.cpp",
		"al/effects/vmorpher.cpp",
		
		-- .cpp alc
		"alc/alc.cpp",
		"alc/alconfig.cpp",
		"alc/alu.cpp",
		"alc/context.cpp",
		"alc/device.cpp",
		"alc/events.cpp",
		"alc/panning.cpp",
		
		"alc/backends/base.cpp",
		"alc/backends/dsound.cpp",
		-- "alc/backends/loopback.cpp",
		"alc/backends/null.cpp",
		-- "alc/backends/wasapi.cpp",
		-- "alc/backends/wave.cpp",
		-- "alc/backends/winmm.cpp",
		
		"alc/effects/autowah.cpp",
		"alc/effects/chorus.cpp",
		"alc/effects/compressor.cpp",
		"alc/effects/convolution.cpp",
		"alc/effects/dedicated.cpp",
		"alc/effects/distortion.cpp",
		"alc/effects/echo.cpp",
		"alc/effects/equalizer.cpp",
		"alc/effects/fshifter.cpp",
		"alc/effects/modulator.cpp",
		"alc/effects/null.cpp",
		"alc/effects/pshifter.cpp",
		"alc/effects/reverb.cpp",
		"alc/effects/vmorpher.cpp",
		
		-- .cpp common
		"common/alcomplex.cpp",
		"common/alstring.cpp",
		"common/althrd_setname.cpp",
		"common/dynload.cpp",
		"common/filesystem.cpp",
		"common/pffft.cpp",
		"common/polyphase_resampler.cpp",
		"common/strutils.cpp",
		
		-- .cpp core
		"core/ambdec.cpp",
		"core/ambidefs.cpp",
		"core/bformatdec.cpp",
		"core/bs2b.cpp",
		"core/bsinc_tables.cpp",
		"core/buffer_storage.cpp",
		"core/context.cpp",
		"core/converter.cpp",
		"core/cpu_caps.cpp",
		"core/cubic_tables.cpp",
		"core/devformat.cpp",
		"core/device.cpp",
		"core/effectslot.cpp",
		"core/except.cpp",
		"core/fpu_ctrl.cpp",
		"core/helpers.cpp",
		"core/hrtf.cpp",
		"core/logging.cpp",
		"core/mastering.cpp",
		"core/storage_formats.cpp",
		"core/uhjfilter.cpp",
		"core/uiddefs.cpp",
		"core/voice.cpp",
		
		"core/filters/biquad.cpp",
		"core/filters/nfc.cpp",
		"core/filters/splitter.cpp",
		
		-- .cpp core mixer
		"core/mixer.cpp",
		"core/mixer/mixer_c.cpp",
		"core/mixer/mixer_sse.cpp",
		"core/mixer/mixer_sse2.cpp",
		"core/mixer/mixer_sse3.cpp",
		"core/mixer/mixer_sse41.cpp",
		
		-- "**.h",
		-- "**.cpp",
		-- "**.txt",
		
		-- "include/**.h",
		-- "include/**.cpp",
		-- 
		-- "build/**.h",
		-- "build/**.cpp",
		-- "common/**.h",
		-- "common/**.cpp",
		-- "core/**.h",
		-- "core/**.cpp",
		-- "gsl/include/**.h",
		-- "gsl/include/**.cpp",
		-- "fmt-11.1.4/include/**.h",
		-- "fmt-11.1.4/include/**.cpp",
	}
	
	removefiles -- excludes
	{
	}

	includedirs
	{
		"include",
		"build",
		"common",
		"core",
		"gsl/include",
		"fmt-11.1.4/include",
		
		"%{wks.location}/Source/Libraries/openal-soft",
		
		"%{wks.location}/Source/Libraries"
	}
	
	links
	{
		"openal-softfmt",
		"winmm.lib",
		"Avrt.lib"
	}
	
	filter "action:vs*"
		buildoptions { "/w14640 /utf-8"}

	filter "configurations:Debug"
		defines
		{
			"WIN32",
			"_WINDOWS",
			"AL_LIBTYPE_STATIC",
			"AL_BUILD_LIBRARY",
			"AL_ALEXT_PROTOTYPES",
			"ALC_API=",
			"AL_API=",
			"_WIN32",
			"NOMINMAX",
			"WIN32_LEAN_AND_MEAN",
			"_CRT_SECURE_NO_WARNINGS",
			"_DISABLE_CONSTEXPR_MUTEX_CONSTRUCTOR",
			"CMAKE_INTDIR=\"Debug\"",
		}
		
	filter "configurations:Release"
		defines
		{
			"WIN32",
			"_WINDOWS",
			"NDEBUG",
			"AL_LIBTYPE_STATIC",
			"AL_BUILD_LIBRARY",
			"AL_ALEXT_PROTOTYPES",
			"ALC_API=",
			"AL_API=",
			"_WIN32",
			"NOMINMAX",
			"WIN32_LEAN_AND_MEAN",
			"_CRT_SECURE_NO_WARNINGS",
			"_DISABLE_CONSTEXPR_MUTEX_CONSTRUCTOR",
			"CMAKE_INTDIR=\"Release\"",
		}
		
	filter "configurations:Retail"
		defines
		{
			"WIN32",
			"_WINDOWS",
			"NDEBUG",
			"AL_LIBTYPE_STATIC",
			"AL_BUILD_LIBRARY",
			"AL_ALEXT_PROTOTYPES",
			"ALC_API=",
			"AL_API=",
			"_WIN32",
			"NOMINMAX",
			"WIN32_LEAN_AND_MEAN",
			"_CRT_SECURE_NO_WARNINGS",
			"_DISABLE_CONSTEXPR_MUTEX_CONSTRUCTOR",
			"CMAKE_INTDIR=\"Release\"",
		}
