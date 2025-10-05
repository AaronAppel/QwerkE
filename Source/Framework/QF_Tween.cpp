#pragma once

#include <algorithm>
#include <cmath>
#include <iostream>

namespace QwerkE {

    namespace Tween {

        constexpr float PI = 3.14159265f;

        // Linear
        inline float Linear(float t) {
            return t;
        }

        // Quadratic
        inline float QuadIn(float t) {
            return t * t;
        }

        inline float QuadOut(float t) {
            return t * (2.0f - t);
        }

        inline float QuadInOut(float t) {
            return (t < 0.5f) ? (2.0f * t * t) : (-1.0f + (4.0f - 2.0f * t) * t);
        }

        // Cubic
        inline float CubicIn(float t) {
            return t * t * t;
        }

        inline float CubicOut(float t) {
            t -= 1.0f;
            return t * t * t + 1.0f;
        }

        inline float CubicInOut(float t) {
            if (t < 0.5f) return 4.0f * t * t * t;
            t -= 1.0f;
            return 1.0f + 4.0f * t * t * t;
        }

        // Quartic
        inline float QuartIn(float t) {
            return t * t * t * t;
        }

        inline float QuartOut(float t) {
            t -= 1.0f;
            return 1.0f - t * t * t * t;
        }

        inline float QuartInOut(float t) {
            if (t < 0.5f) return 8.0f * t * t * t * t;
            t -= 1.0f;
            return 1.0f - 8.0f * t * t * t * t;
        }

        // Quintic
        inline float QuintIn(float t) {
            return t * t * t * t * t;
        }

        inline float QuintOut(float t) {
            t -= 1.0f;
            return t * t * t * t * t + 1.0f;
        }

        inline float QuintInOut(float t) {
            if (t < 0.5f) return 16.0f * t * t * t * t * t;
            t -= 1.0f;
            return 1.0f + 16.0f * t * t * t * t * t;
        }

        // Sine
        inline float SineIn(float t) {
            return 1.0f - std::cos((t * PI) / 2.0f);
        }

        inline float SineOut(float t) {
            return std::sin((t * PI) / 2.0f);
        }

        inline float SineInOut(float t) {
            return -0.5f * (std::cos(PI * t) - 1.0f);
        }

        // Exponential
        inline float ExpoIn(float t) {
            return (t <= 0.0f) ? 0.0f : std::pow(2.0f, 10.0f * (t - 1.0f));
        }

        inline float ExpoOut(float t) {
            return (t >= 1.0f) ? 1.0f : 1.0f - std::pow(2.0f, -10.0f * t);
        }

        inline float ExpoInOut(float t) {
            if (t <= 0.0f) return 0.0f;
            if (t >= 1.0f) return 1.0f;
            t *= 2.0f;
            if (t < 1.0f)
                return 0.5f * std::pow(2.0f, 10.0f * (t - 1.0f));
            return 0.5f * (2.0f - std::pow(2.0f, -10.0f * (t - 1.0f)));
        }

        // Circular
        inline float CircIn(float t) {
            return 1.0f - std::sqrt(1.0f - t * t);
        }

        inline float CircOut(float t) {
            t -= 1.0f;
            return std::sqrt(1.0f - t * t);
        }

        inline float CircInOut(float t) {
            t *= 2.0f;
            if (t < 1.0f)
                return -0.5f * (std::sqrt(1.0f - t * t) - 1.0f);
            t -= 2.0f;
            return 0.5f * (std::sqrt(1.0f - t * t) + 1.0f);
        }

        // Back
        inline float BackIn(float t, float s = 1.70158f) {
            return t * t * ((s + 1.0f) * t - s);
        }

        inline float BackOut(float t, float s = 1.70158f) {
            t -= 1.0f;
            return t * t * ((s + 1.0f) * t + s) + 1.0f;
        }

        inline float BackInOut(float t, float s = 1.70158f) {
            s *= 1.525f;
            t *= 2.0f;
            if (t < 1.0f)
                return 0.5f * (t * t * ((s + 1.0f) * t - s));
            t -= 2.0f;
            return 0.5f * (t * t * ((s + 1.0f) * t + s) + 2.0f);
        }

        // Elastic
        inline float ElasticIn(float t, float amplitude = 1.0f, float period = 0.3f) {
            if (t <= 0.0f || t >= 1.0f) return t;
            float s = period / 4.0f;
            t -= 1.0f;
            return -amplitude * std::pow(2.0f, 10.0f * t) * std::sin((t - s) * (2.0f * PI) / period);
        }

        inline float ElasticOut(float t, float amplitude = 1.0f, float period = 0.3f) {
            if (t <= 0.0f || t >= 1.0f) return t;
            float s = period / 4.0f;
            return amplitude * std::pow(2.0f, -10.0f * t) * std::sin((t - s) * (2.0f * PI) / period) + 1.0f;
        }

        inline float ElasticInOut(float t, float amplitude = 1.0f, float period = 0.45f) {
            if (t <= 0.0f || t >= 1.0f) return t;
            t *= 2.0f;
            float s = period / 4.0f;
            if (t < 1.0f) {
                t -= 1.0f;
                return -0.5f * amplitude * std::pow(2.0f, 10.0f * t) * std::sin((t - s) * (2.0f * PI) / period);
            }
            t -= 1.0f;
            return 0.5f * amplitude * std::pow(2.0f, -10.0f * t) * std::sin((t - s) * (2.0f * PI) / period) + 1.0f;
        }

        // Bounce (helper)
        inline float BounceOut(float t) {
            if (t < 1.0f / 2.75f) {
                return 7.5625f * t * t;
            }
            else if (t < 2.0f / 2.75f) {
                t -= 1.5f / 2.75f;
                return 7.5625f * t * t + 0.75f;
            }
            else if (t < 2.5f / 2.75f) {
                t -= 2.25f / 2.75f;
                return 7.5625f * t * t + 0.9375f;
            }
            else {
                t -= 2.625f / 2.75f;
                return 7.5625f * t * t + 0.984375f;
            }
        }

        inline float BounceIn(float t) {
            return 1.0f - BounceOut(1.0f - t);
        }

        inline float BounceInOut(float t) {
            if (t < 0.5f)
                return 0.5f * BounceIn(t * 2.0f);
            return 0.5f * BounceOut(t * 2.0f - 1.0f) + 0.5f;
        }
    }

}
