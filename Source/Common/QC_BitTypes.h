#pragma once

// Bits (#TODO Experimental)

// #TODO Also communicate unsigned state
// Unsigned
struct bits1
{
    bits1(unsigned char a_Value)
    {
        value = a_Value;
    }
    unsigned char value : 1; // 1 value unsigned integer
    static constexpr char BITS = 2;
    static constexpr u8 MAX = 1;
    static constexpr u8 SIZE = MAX + 1;
};
using u1 = bits1;

struct bits2
{
    bits2(unsigned char a_Value)
    {
        value = a_Value;
    }
    unsigned char value : 2; // 2 value unsigned integer
    static constexpr char BITS = 2;
    static constexpr u8 MAX = 3;
    static constexpr u8 SIZE = MAX + 1;
};
using u2 = bits2;

struct bits3
{
    bits3(unsigned char a_Value)
    {
        value = a_Value;
    }
    unsigned char value : 3; // 3 value unsigned integer
    static constexpr char BITS = 3;
    static constexpr u8 MAX = 7;
    static constexpr u8 SIZE = MAX + 1;
};
using u3 = bits3;

struct bits4
{
    bits4(unsigned char a_Value)
    {
        value = a_Value;
    }
    unsigned char value : 4; // 4 value unsigned integer
    static constexpr char BITS = 4;
    static constexpr u8 MAX = 15;
    static constexpr u8 SIZE = MAX + 1;
};
using u4 = bits4;

struct bits5
{
    bits5(unsigned char a_Value)
    {
        value = a_Value;
    }
    unsigned char value : 5; // 5 value unsigned integer
    static constexpr char BITS = 5;
    static constexpr u8 MAX = 31;
    static constexpr u8 SIZE = MAX + 1;
};
using u5 = bits5;

struct bits6
{
    bits6(unsigned char a_Value)
    {
        value = a_Value;
    }
    unsigned char value : 6; // 6 value unsigned integer
    static constexpr char BITS = 6;
    static constexpr u8 MAX = 63;
    static constexpr u8 SIZE = MAX + 1;
};
using u6 = bits6;

struct bits7
{
    bits7(unsigned char a_Value)
    {
        value = a_Value;
    }
    unsigned char value : 7; // 7 value unsigned integer
    static constexpr char BITS = 7;
    static constexpr u8 MAX = 127;
    static constexpr u8 SIZE = MAX + 1;
};
using u6 = bits6;

struct ubits9
{
    ubits9(unsigned char a_Value)
    {
        value = a_Value;
    }
    unsigned short value : 9; // 9 value unsigned integer
    static constexpr char BITS = 9;
    static constexpr u8 MAX = 511;
    static constexpr u8 SIZE = MAX + 1;
};
using u9 = ubits9;

struct sbits9
{
    sbits9(unsigned char a_Value)
    {
        value = a_Value;
    }
    unsigned short value : 9; // 9 value unsigned integer
    static constexpr char BITS = 9;
    static constexpr u8 MAX = 511;
    static constexpr u8 SIZE = MAX + 1;
};
using s9 = sbits9;
