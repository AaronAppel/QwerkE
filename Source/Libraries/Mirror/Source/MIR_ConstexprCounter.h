#pragma once

namespace ConstexprCounter // ConstexprCounter
{
    // Taken from : https://b.atch.se/posts/constexpr-counter/

    // #NOTE If MSVC compiler option /permissive is not set, problems can occur.
    // See Rough Edges section in README.md for more information.

    template<int N>
    struct flag {
        friend constexpr int adl_flag(flag<N>);
    };

    template<int N>
    struct writer {
        friend constexpr int adl_flag(flag<N>) {
            return N;
        }

        static constexpr int value = N;
    };

    int constexpr reader(float, flag<0>) {
        return 0;
    }

#if defined(_MSC_VER) && _MSC_VER == 1938

    template<int N, class = char[noexcept(adl_flag(flag<N>())) ? +1 : -1]>
    int constexpr reader(int, flag<N>) {
        return N;
    }

    template<int N>
    int constexpr reader(float, flag<N>, int R = reader(0, flag<N - 1>())) {
        return R;
    }

    template<int N = 1, int C = reader(0, flag<32>())>
    int constexpr next(int R = writer<C + N>::value) {
        return R;
    }

#else // !_MSC_VER || !1938

    template<int N, int = adl_flag(flag<N> {}) >
    int constexpr reader(int, flag<N>) {
        return N;
    }

    template<int N>
    int constexpr reader(float, flag<N>, int R = reader(0, flag<N - 1> {})) {
        return R;
    }

    template<int N = 1>
    int constexpr next(int R = writer < reader(0, flag<32> {}) + N > ::value) {
        return R;
    }

#endif
}
