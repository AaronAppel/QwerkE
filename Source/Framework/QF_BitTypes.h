#pragma once

typedef uint8_t u8;         // 0 to 255
typedef uint16_t u16;       // 0 to 65,535

// Unsigned
struct ubits1 // 1 bit unsigned integer 0 to 1
{
    ubits1() = default;
    ubits1(unsigned char a_Value)
    {
        value = a_Value;
    }
    unsigned char value : 1; // #TODO If I reverse the order, larger ubits values can be used to store value
    static constexpr char BITS = 2;
    static constexpr u8 MAX = 1;
    static constexpr u8 MIN = 0;
    static constexpr u8 SIZE = MAX + 1; // #TODO Rename to RANGE?
};

struct ubits2 // 2 bits unsigned integer 0 to 3
{
    ubits2() = default;
    ubits2(unsigned char a_Value)
    {
        value = a_Value;
    }
    unsigned char value : 2;
    static constexpr char BITS = 2;
    static constexpr u8 MAX = 3;
    static constexpr u8 MIN = 0;
    static constexpr u8 SIZE = MAX + 1;
};

struct ubits3 // 3 bits unsigned integer 0 to 7
{
    ubits3() = default;
    ubits3(unsigned char a_Value)
    {
        value = a_Value;
    }
    unsigned char value : 3;
    static constexpr char BITS = 3;
    static constexpr u8 MAX = 7;
    static constexpr u8 MIN = 0;
    static constexpr u8 SIZE = MAX + 1;
};

struct ubits4 // 4 bits unsigned integer 0 to 15
{
    ubits4() = default;
    ubits4(unsigned char a_Value)
    {
        value = a_Value;
    }
    unsigned char value : 4;
    static constexpr char BITS = 4;
    static constexpr u8 MAX = 15;
    static constexpr u8 MIN = 0;
    static constexpr u8 SIZE = MAX + 1;
};

struct ubits5 // 5 bits unsigned integer 0 to 31
{
    ubits5() = default;
    ubits5(unsigned char a_Value)
    {
        value = a_Value;
    }
    unsigned char value : 5;
    static constexpr char BITS = 5;
    static constexpr u8 MAX = 31;
    static constexpr u8 MIN = 0;
    static constexpr u8 SIZE = MAX + 1;
};

struct ubits6 // 6 bits unsigned integer 0 to 63
{
    ubits6() = default;
    ubits6(unsigned char a_Value)
    {
        value = a_Value;
    }
    unsigned char value : 6;
    static constexpr char BITS = 6;
    static constexpr u8 MAX = 63;
    static constexpr u8 MIN = 0;
    static constexpr u8 SIZE = MAX + 1;
};

struct ubits7 // 7 bits unsigned integer 0 to 127
{
    ubits7() = default;
    ubits7(unsigned char a_Value)
    {
        value = a_Value;
    }
    unsigned char value : 7;
    static constexpr char BITS = 7;
    static constexpr u8 MAX = 127;
    static constexpr u8 MIN = 0;
    static constexpr u8 SIZE = MAX + 1;
};

// struct ubits8 // 8 bits unsigned integer 0 to 255

struct ubits9 // 9 bits unsigned integer 0 to 511
{
    ubits9() = default;
    ubits9(unsigned char a_Value)
    {
        value = a_Value;
    }
    unsigned short value : 9;
    static constexpr char BITS = 9;
    static constexpr u16 MAX = 511;
    static constexpr u8 MIN = 0;
    static constexpr u16 SIZE = MAX + 1;
};

struct ubits10 // 10 bits unsigned integer 0 to 1023
{
    ubits10() = default;
    ubits10(unsigned char a_Value)
    {
        value = a_Value;
    }
    unsigned short value : 10;
    static constexpr char BITS = 10;
    static constexpr u16 MAX = 1023;
    static constexpr u8 MIN = 0;
    static constexpr u16 SIZE = MAX + 1;
};

struct ubits11 // 11 bits unsigned integer 0 to 2047
{
    ubits11() = default;
    ubits11(unsigned char a_Value)
    {
        value = a_Value;
    }
    unsigned short value : 11;
    static constexpr char BITS = 11;
    static constexpr u16 MAX = 2047;
    static constexpr u8 MIN = 0;
    static constexpr u16 SIZE = MAX + 1;
};

struct ubits12 // 12 bits unsigned integer 0 to 4095
{
    ubits12() = default;
    ubits12(unsigned char a_Value)
    {
        value = a_Value;
    }
    unsigned short value : 12;
    static constexpr char BITS = 12;
    static constexpr u16 MAX = 4095;
    static constexpr u8 MIN = 0;
    static constexpr u16 SIZE = MAX + 1;
};

struct ubits13 // 13 bits unsigned integer 0 to 8191
{
    ubits13() = default;
    ubits13(unsigned char a_Value)
    {
        value = a_Value;
    }
    unsigned short value : 13;
    static constexpr char BITS = 13;
    static constexpr u16 MAX = 8191;
    static constexpr u8 MIN = 0;
    static constexpr u16 SIZE = MAX + 1;
};

struct ubits14 // 14 bits unsigned integer 0 to 16383
{
    ubits14() = default;
    ubits14(unsigned char a_Value)
    {
        value = a_Value;
    }
    unsigned short value : 14;
    static constexpr char BITS = 14;
    static constexpr u16 MAX = 16383;
    static constexpr u8 MIN = 0;
    static constexpr u16 SIZE = MAX + 1;
};

struct ubits15 // 15 bits unsigned integer 0 to 32767
{
    ubits15() = default;
    ubits15(unsigned char a_Value)
    {
        value = a_Value;
    }
    unsigned short value : 15;
    static constexpr char BITS = 15;
    static constexpr u16 MAX = 32767;
    static constexpr u8 MIN = 0;
    static constexpr u16 SIZE = MAX + 1;
};

// Signed
struct sbits1 // 1 bit value signed integer -1 to 0
{
    sbits1() = default;
    sbits1(unsigned char a_Value)
    {
        value = a_Value;
    }
    signed char value : 1;
    static constexpr char BITS = 1;
    static constexpr u8 MAX = 0;
    static constexpr s8 MIN = -1;
    static constexpr u8 SIZE = MAX - MIN + 1;
};

struct sbits2 // 2 bits signed integer -2 to 1
{
    sbits2() = default;
    sbits2(unsigned char a_Value)
    {
        value = a_Value;
    }
    signed char value : 2;
    static constexpr char BITS = 2;
    static constexpr u8 MAX = 1;
    static constexpr s8 MIN = -2;
    static constexpr u8 SIZE = MAX - MIN + 1;
};

struct sbits3 // 3 bits signed integer -4 to 3
{
    sbits3() = default;
    sbits3(unsigned char a_Value)
    {
        value = a_Value;
    }
    signed char value : 3;
    static constexpr char BITS = 3;
    static constexpr u8 MAX = 3;
    static constexpr s8 MIN = -4;
    static constexpr u8 SIZE = MAX - MIN + 1;
};

struct sbits4 // 4 bits signed integer -8 to 7
{
    sbits4() = default;
    sbits4(unsigned char a_Value)
    {
        value = a_Value;
    }
    signed char value : 4;
    static constexpr char BITS = 4;
    static constexpr u8 MAX = 7;
    static constexpr s8 MIN = -8;
    static constexpr u8 SIZE = MAX - MIN + 1;
};

struct sbits5 // 5 bits signed integer -16 to 15
{
    sbits5() = default;
    sbits5(unsigned char a_Value)
    {
        value = a_Value;
    }
    signed char value : 5;
    static constexpr char BITS = 5;
    static constexpr u8 MAX = 15;
    static constexpr s8 MIN = -16;
    static constexpr u8 SIZE = MAX - MIN + 1;
};

struct sbits6 // 6 bits signed integer -32 to 31
{
    sbits6() = default;
    sbits6(unsigned char a_Value)
    {
        value = a_Value;
    }
    signed char value : 6;
    static constexpr char BITS = 6;
    static constexpr u8 MAX = 31;
    static constexpr s8 MIN = -32;
    static constexpr u8 SIZE = MAX - MIN + 1;
};

struct sbits7 // 7 bits signed integer -64 to 63
{
    sbits7() = default;
    sbits7(unsigned char a_Value)
    {
        value = a_Value;
    }
    signed char value : 7;
    static constexpr char BITS = 7;
    static constexpr u8 MAX = 63;
    static constexpr s8 MIN = -64;
    static constexpr u8 SIZE = MAX - MIN + 1;
};

struct sbits9 // 9 bits value unsigned integer -256 to 255
{
    sbits9() = default;
    sbits9(unsigned char a_Value)
    {
        value = a_Value;
    }
    signed short value : 9;
    static constexpr char BITS = 9;
    static constexpr u8 MAX = 255;
    static constexpr s8 MIN = -256;
    static constexpr u8 SIZE = MAX - MIN + 1;
};

struct sbits10 // 10 bits signed integer -512 to 511
{
    sbits10() = default;
    sbits10(unsigned char a_Value)
    {
        value = a_Value;
    }
    signed short value : 10;
    static constexpr char BITS = 10;
    static constexpr u8 MAX = 511;
    static constexpr s8 MIN = -512;
    static constexpr u8 SIZE = MAX - MIN + 1;
};

struct sbits11 // 11 bits signed integer -1024 to 1023
{
    sbits11() = default;
    sbits11(unsigned char a_Value)
    {
        value = a_Value;
    }
    signed short value : 11;
    static constexpr char BITS = 11;
    static constexpr u8 MAX = 1023;
    static constexpr s8 MIN = -1024;
    static constexpr u8 SIZE = MAX - MIN + 1;
};

struct sbits12 // 12 bits signed integer -2048 to 2047
{
    sbits12() = default;
    sbits12(unsigned char a_Value)
    {
        value = a_Value;
    }
    signed short value : 12;
    static constexpr char BITS = 12;
    static constexpr u8 MAX = 1023;
    static constexpr s8 MIN = -1024;
    static constexpr u8 SIZE = MAX - MIN + 1;
};

struct sbits13 // 13 bits signed integer -4096 to 4095
{
    sbits13() = default;
    sbits13(unsigned char a_Value)
    {
        value = a_Value;
    }
    signed short value : 13;
    static constexpr char BITS = 13;
    static constexpr u8 MAX = 4095;
    static constexpr s8 MIN = -4096;
    static constexpr u8 SIZE = MAX - MIN + 1;
};

struct sbits14 // 14 bits signed integer -8192 to 8191
{
    sbits14() = default;
    sbits14(unsigned char a_Value)
    {
        value = a_Value;
    }
    signed short value : 14;
    static constexpr char BITS = 14;
    static constexpr u8 MAX = 8191;
    static constexpr s8 MIN = -8192;
    static constexpr u8 SIZE = MAX - MIN + 1;
};

struct sbits15 // 15 bits signed integer -16384  to 16383
{
    sbits15() = default;
    sbits15(unsigned char a_Value)
    {
        value = a_Value;
    }
    signed short value : 15;
    static constexpr char BITS = 15;
    static constexpr u8 MAX = 16383;
    static constexpr s8 MIN = -16384;
    static constexpr u8 SIZE = MAX - MIN + 1;
};
