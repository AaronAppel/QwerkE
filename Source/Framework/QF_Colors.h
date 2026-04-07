#pragma once

#include <cmath>

#include "QF_Color.h"

namespace QwerkE {

    namespace Colors {

        const float defaultSaturation = 0.65f; // #TODO Default saturation and value?
        const float defaultValue = 0.95f;

        // #TODO Helpers:
        // - RGB to HSV conversion helpers, and vice versa
        // - Int to Color by range vec4f IntToColor(min, value, max), and vec4f IntToColor(float percentage);
        // - Color categories and palettes: Complementary, pastels, saturated, dark/light, etc

        // Constants
        inline vec3f White()        { return { 255, 255, 255 }; }
        inline vec3f Gray()         { return { 128, 128, 128 }; }
        inline vec3f Black()        { return {   0,   0,   0 }; }

        inline vec3f Red()          { return { 255,   0,   0 }; };
        inline vec3f Green()        { return {   0, 255,   0 }; };
        inline vec3f Blue()         { return {   0,   0, 255 }; };

        inline vec3f Magenta()      { return { 255,  0, 255 }; }
        inline vec3f Yellow()       { return { 255, 255,   0 }; }

        inline vec3f Lime()         { return {   0, 255,   0 }; }
        inline vec3f Cyan()         { return {   0, 255, 255 }; }
        inline vec3f Silver()       { return { 192, 192, 192 }; }
        inline vec3f Maroon()       { return { 128,   0,   0 }; }
        inline vec3f Olive()        { return { 128, 128,   0 }; }
        inline vec3f Purple()       { return { 128,   0, 128 }; }
        inline vec3f Teal()         { return {   0, 128, 128 }; }
        inline vec3f Navy()         { return {   0,   0, 128 }; }
        inline vec3f Orange()       { return { 255, 165,   0 }; }
        inline vec3f Pink()         { return { 255, 192, 203 }; }
        inline vec3f Brown()        { return { 165,  42,  42 }; }
        inline vec3f Gold()         { return { 255, 215,   0 }; }
        inline vec3f Salmon()       { return { 250, 128, 114 }; }
        inline vec3f Coral()        { return { 255, 127,  80 }; }
        inline vec3f Indigo()       { return {  75,   0, 130 }; }
        inline vec3f Violet()       { return { 238, 130, 238 }; }
        inline vec3f Turquoise()    { return {  64, 224, 208 }; }
        inline vec3f Beige()        { return { 245, 245, 220 }; }
        inline vec3f Mint()         { return { 189, 252, 201 }; }
        inline vec3f Lavender()     { return { 230, 230, 250 }; }
        inline vec3f CornFlower()   { return { 100, 149, 237 }; }

        // #TODO vec4 entries
        // static vec4f White4 = { 1.f, 1.f, 1.f, 1.f };

        // Math helpers
        inline float Normalize(int value, int min, int max)
        {
            if (max == min) return 0.0f;
            return (value - min) / float(max - min);
        }

        inline float Clamp01(float v)
        {
            return (v < 0.f) ? 0.f : (v > 1.f ? 1.f : v);
        }

        inline Color4 Lerp(const Color4& a, const Color4& b, float t)
        {
            t = Clamp01(t);
            return {
                a.r + (b.r - a.r) * t,
                a.g + (b.g - a.g) * t,
                a.b + (b.b - a.b) * t,
                a.a + (b.a - a.a) * t
            };
        }

        template <typename T>
        inline double UInt64ToUnitDouble(T id)
        {
            const uint64_t x = static_cast<uint64_t>(id);
            return (x >> 11) * (1.0 / (1ULL << 53));
        }

        // Color space conversion
        // #TODO Test
        inline Color4 RgbaToHsva(const Color4& in)
        {
            const float maxVal = std::max({ in.r, in.g, in.b });
            const float minVal = std::min({ in.r, in.g, in.b });
            const float delta = maxVal - minVal;

            float saturation;
            if (maxVal == 0.0f)
                saturation = 0.0f;
            else
                saturation = delta / maxVal;

            float hue;
            if (delta == 0.0f) {
                hue = 0.0f; // undefined, default to 0
            }
            else {
                if (maxVal == in.r) {
                    hue = (in.g - in.b) / delta;
                }
                else if (maxVal == in.g) {
                    hue = 2.0f + (in.b - in.r) / delta;
                }
                else {
                    hue = 4.0f + (in.r - in.g) / delta;
                }

                hue /= 6.0f;
                if (hue < 0.0f)
                    hue += 1.0f;
            }
            return { hue, saturation, maxVal, in.a };
        }

        // #TODO Test
        inline Color4 RgbaToHsva(float r, float g, float b, float a = 1.0f)
        {
            Color4 temp { r, g, b, a, };
            return RgbaToHsva(temp);
        }

        // #TODO Test
        inline Color4 HsvaToRgba(const Color4& in)
        {
            float hue = in.h - std::floor(in.h); // wrap [0,1)

            const int i = int(hue * 6.0f);
            const float f = hue * 6.0f - i;
            const float p = in.v * (1.0f - in.s);
            const float q = in.v * (1.0f - f * in.s);
            const float t = in.v * (1.0f - (1.0f - f) * in.s);

            float r, g, b;
            switch (i % 6) {
            case 0: r = in.v; g = t; b = p; break;
            case 1: r = q; g = in.v; b = p; break;
            case 2: r = p; g = in.v; b = t; break;
            case 3: r = p; g = q; b = in.v; break;
            case 4: r = t; g = p; b = in.v; break;
            default: r = in.v; g = p; b = q; break;
            }

            return { r, g, b, in.a };
        }

        // #TODO Test
        inline Color4 HsvaToRgba(float h, float s, float v, float a = 1.0f)
        {
            Color4 temp{ h, s, v, a, };
            return HsvaToRgba(temp);
        }

        // Normalized value to spectrum color
        inline Color4 Spectrum(float normalizedRange)
        {
            return HsvaToRgba(Clamp01(normalizedRange), 1.f, 1.f, 1.f);
        }

        inline Color4 Spectrum(float value, float min, float max)
        {
            const float range = max - min;
            if (min < max || range == 0.f)
            {
                return { 0.f, 0.f, 0.f, 0.f };
            }

            value = Math::Clamp(value, min, max);
            const float t = (value - min) / range;

            return HsvaToRgba(t, 1.f, 1.f, 1.f);
        }

        // Gradients
        struct ColorStop
        {
            float t;     // must be in [0,1]
            Color4 color;
        };

        // #TODO Offer variation with equal spacing. For N stops, each is equal to 1/N. No need for specifying t value
        template <size_t N>
        inline Color4 Gradient(float t, const std::array<ColorStop, N>& stops)
        {
            static_assert(N >= 2, "Need at least 2 color stops");

            t = Clamp01(t);

            // Handle edge cases
            if (t <= stops[0].t)
                return stops[0].color;

            if (t >= stops[N - 1].t)
                return stops[N - 1].color;

            // Find segment
            for (size_t i = 0; i < N - 1; ++i)
            {
                const auto& a = stops[i];
                const auto& b = stops[i + 1];

                if (t >= a.t && t <= b.t)
                {
                    float localT = (t - a.t) / (b.t - a.t);
                    return Lerp(a.color, b.color, localT);
                }
            }

            // Should never happen if stops cover [0,1]
            return stops[N - 1].color;
        }

        // #TODO Doesn't need to be pre-defined. Let users create their own gradient ranges
        inline Color4 Heatmap(float t)
        {
            // Blue → Cyan → Green → Yellow → Red
            // 0%      25%     50%    75%    100%
            t = Clamp01(t);

            if (t < 0.25f) return Lerp(Color4(0.f, 0.f, 1, 1.0f), Color4(0.f, 1, 1, 1.0f), t / 0.25f);
            if (t < 0.5f)  return Lerp(Color4(0.f, 1, 1, 1.0f), Color4(0.f, 1, 0.f, 1.0f), (t - 0.25f) / 0.25f);
            if (t < 0.75f) return Lerp(Color4(0.f, 1.f, 0.f, 1.0f), Color4(1.f, 1.f, 0.f, 1.0f), (t - 0.5f) / 0.25f);
            return Lerp(Color4(1.f, 1.f, 0.f, 1.0f), Color4(1.f, 0.f, 0.f, 1.0f), (t - 0.75f) / 0.25f);
        }

        // Integer to spectrum color
        // #TODO Test all id to color conversions
        template <typename T>
        inline Color4 IdToHsv(T id)
        {
            const float goldenRatio = 0.61803398875f;
            const float hue = std::fmod(id * goldenRatio, 1.0f);
            return Color4(hue, defaultSaturation, defaultValue, 1.f);
        }

        template <>
        inline Color4 IdToHsv<u64>(u64 id)
        {
            const double hue = UInt64ToUnitDouble(id);
            return Color4(hue, defaultSaturation, defaultValue, 1.f);
        }

        template <typename T>
        inline Color4 IdToRgb(T id)
        {
            const float goldenRatio = 0.61803398875f;
            const float hue = std::fmod(id * goldenRatio, 1.0f);
            return HsvaToRgba(hue, defaultSaturation, defaultValue);
        }

        template <>
        inline Color4 IdToRgb<u64>(u64 id)
        {
            const double hue = UInt64ToUnitDouble(id);
            return HsvaToRgba(hue, defaultSaturation, defaultValue);
        }

    } // namespace Colors

} // namespace QwerkE
