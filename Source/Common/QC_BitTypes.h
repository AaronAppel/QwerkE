#pragma once

// Bits (#TODO Experimental)

// Unsigned
struct ubits1
{
    ubits1(unsigned char a_Value)
    {
        value = a_Value;
    }
    unsigned char value : 1; // 1 value unsigned integer
    static constexpr char BITS = 2;
    static constexpr u8 MAX = 1;
    static constexpr u8 SIZE = MAX + 1;
};

struct ubits2
{
    ubits2(unsigned char a_Value)
    {
        value = a_Value;
    }
    unsigned char value : 2; // 2 value unsigned integer
    static constexpr char BITS = 2;
    static constexpr u8 MAX = 3;
    static constexpr u8 SIZE = MAX + 1;
};

struct ubits3
{
    ubits3(unsigned char a_Value)
    {
        value = a_Value;
    }
    unsigned char value : 3; // 3 value unsigned integer
    static constexpr char BITS = 3;
    static constexpr u8 MAX = 7;
    static constexpr u8 SIZE = MAX + 1;
};

struct ubits4
{
    ubits4(unsigned char a_Value)
    {
        value = a_Value;
    }
    unsigned char value : 4; // 4 value unsigned integer
    static constexpr char BITS = 4;
    static constexpr u8 MAX = 15;
    static constexpr u8 SIZE = MAX + 1;
};

struct ubits5
{
    ubits5(unsigned char a_Value)
    {
        value = a_Value;
    }
    unsigned char value : 5; // 5 value unsigned integer
    static constexpr char BITS = 5;
    static constexpr u8 MAX = 31;
    static constexpr u8 SIZE = MAX + 1;
};

struct ubits6
{
    ubits6(unsigned char a_Value)
    {
        value = a_Value;
    }
    unsigned char value : 6; // 6 value unsigned integer
    static constexpr char BITS = 6;
    static constexpr u8 MAX = 63;
    static constexpr u8 SIZE = MAX + 1;
};

struct ubits7
{
    ubits7(unsigned char a_Value)
    {
        value = a_Value;
    }
    unsigned char value : 7; // 7 value unsigned integer
    static constexpr char BITS = 7;
    static constexpr u8 MAX = 127;
    static constexpr u8 SIZE = MAX + 1;
};

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

// Signed
struct sbits1
{
    sbits1(unsigned char a_Value)
    {
        value = a_Value;
    }
    unsigned char value : 1; // 1 value unsigned integer
    static constexpr char BITS = 2;
    static constexpr u8 MAX = 1;
    static constexpr u8 SIZE = MAX + 1;
};

struct sbits2
{
    sbits2(unsigned char a_Value)
    {
        value = a_Value;
    }
    unsigned char value : 2; // 2 value unsigned integer
    static constexpr char BITS = 2;
    static constexpr u8 MAX = 3;
    static constexpr u8 SIZE = MAX + 1;
};

struct sbits3
{
    sbits3(unsigned char a_Value)
    {
        value = a_Value;
    }
    unsigned char value : 3; // 3 value unsigned integer
    static constexpr char BITS = 3;
    static constexpr u8 MAX = 7;
    static constexpr u8 SIZE = MAX + 1;
};

struct sbits4
{
    sbits4(unsigned char a_Value)
    {
        value = a_Value;
    }
    unsigned char value : 4; // 4 value unsigned integer
    static constexpr char BITS = 4;
    static constexpr u8 MAX = 15;
    static constexpr u8 SIZE = MAX + 1;
};

struct sbits5
{
    sbits5(unsigned char a_Value)
    {
        value = a_Value;
    }
    unsigned char value : 5; // 5 value unsigned integer
    static constexpr char BITS = 5;
    static constexpr u8 MAX = 31;
    static constexpr u8 SIZE = MAX + 1;
};

struct sbits6
{
    sbits6(unsigned char a_Value)
    {
        value = a_Value;
    }
    unsigned char value : 6; // 6 value unsigned integer
    static constexpr char BITS = 6;
    static constexpr u8 MAX = 63;
    static constexpr u8 SIZE = MAX + 1;
};

struct sbits7
{
    sbits7(unsigned char a_Value)
    {
        value = a_Value;
    }
    unsigned char value : 7; // 7 value unsigned integer
    static constexpr char BITS = 7;
    static constexpr u8 MAX = 127;
    static constexpr u8 SIZE = MAX + 1;
};

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
