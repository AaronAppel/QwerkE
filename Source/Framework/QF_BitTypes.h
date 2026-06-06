#pragma once

typedef uint8_t u8;         // 0 to 255
typedef uint16_t u16;       // 0 to 65,535
typedef uint32_t u32;       // 0 to 65,535

// Unsigned
struct ubits1 {
    ubits1() = default;
    static constexpr unsigned int MAX = 1u;
    ubits1(unsigned int aValue) { value = aValue & MAX; }
    unsigned char value : 1;
    static constexpr char BITS = 1;
    static constexpr unsigned int MIN = 0;
    static constexpr unsigned int RANGE = MAX + 1u;
};

struct ubits2 {
    ubits2() = default;
    static constexpr unsigned int MAX = 3u;
    ubits2(unsigned int aValue) { value = aValue & MAX; }
    unsigned char value : 2;
    static constexpr char BITS = 2;
    static constexpr unsigned int MIN = 0;
    static constexpr unsigned int RANGE = MAX + 1u;
};

struct ubits3 {
    ubits3() = default;
    static constexpr unsigned int MAX = 7u;
    ubits3(unsigned int aValue) { value = aValue & MAX; }
    unsigned char value : 3;
    static constexpr char BITS = 3;
    static constexpr unsigned int MIN = 0;
    static constexpr unsigned int RANGE = MAX + 1u;
};

struct ubits4 {
    ubits4() = default;
    static constexpr unsigned int MAX = 15u;
    ubits4(unsigned int aValue) { value = aValue & MAX; }
    unsigned char value : 4;
    static constexpr char BITS = 4;
    static constexpr unsigned int MIN = 0;
    static constexpr unsigned int RANGE = MAX + 1u;
};

struct ubits5 {
    ubits5() = default;
    static constexpr unsigned int MAX = 31u;
    ubits5(unsigned int aValue) { value = aValue & MAX; }
    unsigned char value : 5;
    static constexpr char BITS = 5;
    static constexpr unsigned int MIN = 0;
    static constexpr unsigned int RANGE = MAX + 1u;
};

struct ubits6 {
    ubits6() = default;
    static constexpr unsigned int MAX = 63u;
    ubits6(unsigned int aValue) { value = aValue & MAX; }
    unsigned char value : 6;
    static constexpr char BITS = 6;
    static constexpr unsigned int MIN = 0;
    static constexpr unsigned int RANGE = MAX + 1u;
};

struct ubits7 {
    ubits7() = default;
    static constexpr unsigned int MAX = 127u;
    ubits7(unsigned int aValue) { value = aValue & MAX; }
    unsigned char value : 7;
    static constexpr char BITS = 7;
    static constexpr unsigned int MIN = 0;
    static constexpr unsigned int RANGE = MAX + 1u;
};

struct ubits8 {
    ubits8() = default;
    static constexpr unsigned int MAX = 255u;
    ubits8(unsigned int aValue) { value = aValue & MAX; }
    unsigned short value : 8;
    static constexpr char BITS = 8;
    static constexpr unsigned int MIN = 0;
    static constexpr unsigned int RANGE = MAX + 1u;
};

struct ubits9 {
    ubits9() = default;
    static constexpr unsigned int MAX = 511u;
    ubits9(unsigned int aValue) { value = aValue & MAX; }
    unsigned short value : 9;
    static constexpr char BITS = 9;
    static constexpr unsigned int MIN = 0;
    static constexpr unsigned int RANGE = MAX + 1u;
};

struct ubits10 {
    ubits10() = default;
    static constexpr unsigned int MAX = 1023u;
    ubits10(unsigned int aValue) { value = aValue & MAX; }
    unsigned short value : 10;
    static constexpr char BITS = 10;
    static constexpr unsigned int MIN = 0;
    static constexpr unsigned int RANGE = MAX + 1u;
};

struct ubits11 {
    ubits11() = default;
    static constexpr unsigned int MAX = 2047u;
    ubits11(unsigned int aValue) { value = aValue & MAX; }
    unsigned short value : 11;
    static constexpr char BITS = 11;
    static constexpr unsigned int MIN = 0;
    static constexpr unsigned int RANGE = MAX + 1u;
};

struct ubits12 {
    ubits12() = default;
    static constexpr unsigned int MAX = 4095u;
    ubits12(unsigned int aValue) { value = aValue & MAX; }
    unsigned short value : 12;
    static constexpr char BITS = 12;
    static constexpr unsigned int MIN = 0;
    static constexpr unsigned int RANGE = MAX + 1u;
};

struct ubits13 {
    ubits13() = default;
    static constexpr unsigned int MAX = 8191u;
    ubits13(unsigned int aValue) { value = aValue & MAX; }
    unsigned short value : 13;
    static constexpr char BITS = 13;
    static constexpr unsigned int MIN = 0;
    static constexpr unsigned int RANGE = MAX + 1u;
};

struct ubits14 {
    ubits14() = default;
    static constexpr unsigned int MAX = 16383u;
    ubits14(unsigned int aValue) { value = aValue & MAX; }
    unsigned short value : 14;
    static constexpr char BITS = 14;
    static constexpr unsigned int MIN = 0;
    static constexpr unsigned int RANGE = MAX + 1u;
};

struct ubits15 {
    ubits15() = default;
    static constexpr unsigned int MAX = 32767u;
    ubits15(unsigned int aValue) { value = aValue & MAX; }
    unsigned short value : 15;
    static constexpr char BITS = 15;
    static constexpr unsigned int MIN = 0;
    static constexpr unsigned int RANGE = MAX + 1u;
};

struct ubits16 {
    ubits16() = default;
    static constexpr unsigned int MAX = 65535u;
    ubits16(unsigned int aValue) { value = aValue & MAX; }
    unsigned short value : 16;
    static constexpr char BITS = 16;
    static constexpr unsigned int MIN = 0;
    static constexpr unsigned int RANGE = MAX + 1u;
};

struct ubits17 {
    ubits17() = default;
    static constexpr unsigned int MAX = 131071u;
    ubits17(unsigned int aValue) { value = aValue & MAX; }
    unsigned int value : 17;
    static constexpr char BITS = 17;
    static constexpr unsigned int MIN = 0;
    static constexpr unsigned int RANGE = MAX + 1u;
};

struct ubits18 {
    ubits18() = default;
    static constexpr unsigned int MAX = 262143u;
    ubits18(unsigned int v) { value = v; }
    unsigned int value : 18;
    static constexpr char BITS = 18;
    static constexpr unsigned int MIN = 0;
    static constexpr unsigned int RANGE = MAX + 1u;
};

struct ubits19 {
    ubits19() = default;
    static constexpr unsigned int MAX = 524287u;
    ubits19(unsigned int v) { value = v; }
    unsigned int value : 19;
    static constexpr char BITS = 19;
    static constexpr unsigned int MIN = 0;
    static constexpr unsigned int RANGE = MAX + 1u;
};

struct ubits20 {
    ubits20() = default;
    static constexpr unsigned int MAX = 1048575u;
    ubits20(unsigned int v) { value = v; }
    unsigned int value : 20;
    static constexpr char BITS = 20;
    static constexpr unsigned int MIN = 0;
    static constexpr unsigned int RANGE = MAX + 1u;
};

struct ubits21 {
    ubits21() = default;
    static constexpr unsigned int MAX = 2097151u;
    ubits21(unsigned int v) { value = v; }
    unsigned int value : 21;
    static constexpr char BITS = 21;
    static constexpr unsigned int MIN = 0;
    static constexpr unsigned int RANGE = MAX + 1u;
};

struct ubits22 {
    ubits22() = default;
    static constexpr unsigned int MAX = 4194303u;
    ubits22(unsigned int v) { value = v; }
    unsigned int value : 22;
    static constexpr char BITS = 22;
    static constexpr unsigned int MIN = 0;
    static constexpr unsigned int RANGE = MAX + 1u;
};

struct ubits23 {
    ubits23() = default;
    static constexpr unsigned int MAX = 8388607u;
    ubits23(unsigned int v) { value = v; }
    unsigned int value : 23;
    static constexpr char BITS = 23;
    static constexpr unsigned int MIN = 0;
    static constexpr unsigned int RANGE = MAX + 1u;
};

struct ubits24 {
    ubits24() = default;
    static constexpr unsigned int MAX = 16777215u;
    ubits24(unsigned int v) { value = v; }
    unsigned int value : 24;
    static constexpr char BITS = 24;
    static constexpr unsigned int MIN = 0;
    static constexpr unsigned int RANGE = MAX + 1u;
};

struct ubits25 {
    ubits25() = default;
    static constexpr unsigned int MAX = 33554431u;
    ubits25(unsigned int v) { value = v; }
    unsigned int value : 25;
    static constexpr char BITS = 25;
    static constexpr unsigned int MIN = 0;
    static constexpr unsigned int RANGE = MAX + 1u;
};

struct ubits26 {
    ubits26() = default;
    static constexpr unsigned int MAX = 67108863u;
    ubits26(unsigned int v) { value = v; }
    unsigned int value : 26;
    static constexpr char BITS = 26;
    static constexpr unsigned int MIN = 0;
    static constexpr unsigned int RANGE = MAX + 1u;
};

struct ubits27 {
    ubits27() = default;
    static constexpr unsigned int MAX = 134217727u;
    ubits27(unsigned int v) { value = v; }
    unsigned int value : 27;
    static constexpr char BITS = 27;
    static constexpr unsigned int MIN = 0;
    static constexpr unsigned int RANGE = MAX + 1u;
};

struct ubits28 {
    ubits28() = default;
    static constexpr unsigned int MAX = 268435455u;
    ubits28(unsigned int v) { value = v; }
    unsigned int value : 28;
    static constexpr char BITS = 28;
    static constexpr unsigned int MIN = 0;
    static constexpr unsigned int RANGE = MAX + 1u;
};

struct ubits29 {
    ubits29() = default;
    static constexpr unsigned int MAX = 536870911u;
    ubits29(unsigned int v) { value = v; }
    unsigned int value : 29;
    static constexpr char BITS = 29;
    static constexpr unsigned int MIN = 0;
    static constexpr unsigned int RANGE = MAX + 1u;
};

struct ubits30 {
    ubits30() = default;
    static constexpr unsigned int MAX = 1073741823u;
    ubits30(unsigned int v) { value = v; }
    unsigned int value : 30;
    static constexpr char BITS = 30;
    static constexpr unsigned int MIN = 0;
    static constexpr unsigned int RANGE = MAX + 1u;
};

struct ubits31 {
    ubits31() = default;
    static constexpr unsigned int MAX = 2147483647u;
    ubits31(unsigned int v) { value = v; }
    unsigned int value : 31;
    static constexpr char BITS = 31;
    static constexpr unsigned int MIN = 0;
    static constexpr unsigned int RANGE = MAX + 1u;
};

struct ubits32 {
    ubits32() = default;
    static constexpr unsigned long long MAX = 4294967295ull;
    ubits32(unsigned long long v) { value = v; }
    unsigned long long value : 32;
    static constexpr char BITS = 32;
    static constexpr unsigned long long MIN = 0;
    static constexpr unsigned long long RANGE = MAX + 1ull;
};

struct ubits33 {
    ubits33() = default;
    static constexpr unsigned long long MAX = 8589934591ull;
    ubits33(unsigned long long aValue) { value = aValue & MAX; }
    unsigned long long value : 33;
    static constexpr char BITS = 33;
    static constexpr unsigned long long MIN = 0;
    static constexpr unsigned long long RANGE = MAX + 1ull;
};

struct ubits34 {
    ubits34() = default;
    static constexpr unsigned long long MAX = 17179869183ull;
    ubits34(unsigned long long v) { value = v; }
    unsigned long long value : 34;
    static constexpr char BITS = 34;
    static constexpr unsigned long long MIN = 0;
    static constexpr unsigned long long RANGE = MAX + 1ull;
};

struct ubits35 {
    ubits35() = default;
    static constexpr unsigned long long MAX = 34359738367ull;
    ubits35(unsigned long long v) { value = v; }
    unsigned long long value : 35;
    static constexpr char BITS = 35;
    static constexpr unsigned long long MIN = 0;
    static constexpr unsigned long long RANGE = MAX + 1ull;
};

struct ubits36 {
    ubits36() = default;
    static constexpr unsigned long long MAX = 68719476735ull;
    ubits36(unsigned long long v) { value = v; }
    unsigned long long value : 36;
    static constexpr char BITS = 36;
    static constexpr unsigned long long MIN = 0;
    static constexpr unsigned long long RANGE = MAX + 1ull;
};

struct ubits37 {
    ubits37() = default;
    static constexpr unsigned long long MAX = 137438953471ull;
    ubits37(unsigned long long v) { value = v; }
    unsigned long long value : 37;
    static constexpr char BITS = 37;
    static constexpr unsigned long long MIN = 0;
    static constexpr unsigned long long RANGE = MAX + 1ull;
};

struct ubits38 {
    ubits38() = default;
    static constexpr unsigned long long MAX = 274877906943ull;
    ubits38(unsigned long long v) { value = v; }
    unsigned long long value : 38;
    static constexpr char BITS = 38;
    static constexpr unsigned long long MIN = 0;
    static constexpr unsigned long long RANGE = MAX + 1ull;
};

struct ubits39 {
    ubits39() = default;
    static constexpr unsigned long long MAX = 549755813887ull;
    ubits39(unsigned long long v) { value = v; }
    unsigned long long value : 39;
    static constexpr char BITS = 39;
    static constexpr unsigned long long MIN = 0;
    static constexpr unsigned long long RANGE = MAX + 1ull;
};

struct ubits40 {
    ubits40() = default;
    static constexpr unsigned long long MAX = 1099511627775ull;
    ubits40(unsigned long long v) { value = v; }
    unsigned long long value : 40;
    static constexpr char BITS = 40;
    static constexpr unsigned long long MIN = 0;
    static constexpr unsigned long long RANGE = MAX + 1ull;
};

struct ubits41 {
    ubits41() = default;
    static constexpr unsigned long long MAX = 2199023255551ull;
    ubits41(unsigned long long v) { value = v; }
    unsigned long long value : 41;
    static constexpr char BITS = 41;
    static constexpr unsigned long long MIN = 0;
    static constexpr unsigned long long RANGE = MAX + 1ull;
};

struct ubits42 {
    ubits42() = default;
    static constexpr unsigned long long MAX = 4398046511103ull;
    ubits42(unsigned long long v) { value = v; }
    unsigned long long value : 42;
    static constexpr char BITS = 42;
    static constexpr unsigned long long MIN = 0;
    static constexpr unsigned long long RANGE = MAX + 1ull;
};

struct ubits43 {
    ubits43() = default;
    static constexpr unsigned long long MAX = 8796093022207ull;
    ubits43(unsigned long long v) { value = v; }
    unsigned long long value : 43;
    static constexpr char BITS = 43;
    static constexpr unsigned long long MIN = 0;
    static constexpr unsigned long long RANGE = MAX + 1ull;
};

struct ubits44 {
    ubits44() = default;
    static constexpr unsigned long long MAX = 17592186044415ull;
    ubits44(unsigned long long v) { value = v; }
    unsigned long long value : 44;
    static constexpr char BITS = 44;
    static constexpr unsigned long long MIN = 0;
    static constexpr unsigned long long RANGE = MAX + 1ull;
};

struct ubits45 {
    ubits45() = default;
    static constexpr unsigned long long MAX = 35184372088831ull;
    ubits45(unsigned long long v) { value = v; }
    unsigned long long value : 45;
    static constexpr char BITS = 45;
    static constexpr unsigned long long MIN = 0;
    static constexpr unsigned long long RANGE = MAX + 1ull;
};

struct ubits46 {
    ubits46() = default;
    static constexpr unsigned long long MAX = 70368744177663ull;
    ubits46(unsigned long long v) { value = v; }
    unsigned long long value : 46;
    static constexpr char BITS = 46;
    static constexpr unsigned long long MIN = 0;
    static constexpr unsigned long long RANGE = MAX + 1ull;
};

struct ubits47 {
    ubits47() = default;
    static constexpr unsigned long long MAX = 140737488355327ull;
    ubits47(unsigned long long v) { value = v; }
    unsigned long long value : 47;
    static constexpr char BITS = 47;
    static constexpr unsigned long long MIN = 0;
    static constexpr unsigned long long RANGE = MAX + 1ull;
};

struct ubits48 {
    ubits48() = default;
    static constexpr unsigned long long MAX = 281474976710655ull;
    ubits48(unsigned long long v) { value = v; }
    unsigned long long value : 48;
    static constexpr char BITS = 48;
    static constexpr unsigned long long MIN = 0;
    static constexpr unsigned long long RANGE = MAX + 1ull;
};

struct ubits49 {
    ubits49() = default;
    static constexpr unsigned long long MAX = 562949953421311ull;
    ubits49(unsigned long long v) { value = v; }
    unsigned long long value : 49;
    static constexpr char BITS = 49;
    static constexpr unsigned long long MIN = 0;
    static constexpr unsigned long long RANGE = MAX + 1ull;
};

struct ubits50 {
    ubits50() = default;
    static constexpr unsigned long long MAX = 1125899906842623ull;
    ubits50(unsigned long long v) { value = v; }
    unsigned long long value : 50;
    static constexpr char BITS = 50;
    static constexpr unsigned long long MIN = 0;
    static constexpr unsigned long long RANGE = MAX + 1ull;
};

struct ubits51 {
    ubits51() = default;
    static constexpr unsigned long long MAX = 2251799813685247ull;
    ubits51(unsigned long long v) { value = v; }
    unsigned long long value : 51;
    static constexpr char BITS = 51;
    static constexpr unsigned long long MIN = 0;
    static constexpr unsigned long long RANGE = MAX + 1ull;
};

struct ubits52 {
    ubits52() = default;
    static constexpr unsigned long long MAX = 4503599627370495ull;
    ubits52(unsigned long long v) { value = v; }
    unsigned long long value : 52;
    static constexpr char BITS = 52;
    static constexpr unsigned long long MIN = 0;
    static constexpr unsigned long long RANGE = MAX + 1ull;
};

struct ubits53 {
    ubits53() = default;
    static constexpr unsigned long long MAX = 9007199254740991ull;
    ubits53(unsigned long long v) { value = v; }
    unsigned long long value : 53;
    static constexpr char BITS = 53;
    static constexpr unsigned long long MIN = 0;
    static constexpr unsigned long long RANGE = MAX + 1ull;
};

struct ubits54 {
    ubits54() = default;
    static constexpr unsigned long long MAX = 18014398509481983ull;
    ubits54(unsigned long long v) { value = v; }
    unsigned long long value : 54;
    static constexpr char BITS = 54;
    static constexpr unsigned long long MIN = 0;
    static constexpr unsigned long long RANGE = MAX + 1ull;
};

struct ubits55 {
    ubits55() = default;
    static constexpr unsigned long long MAX = 36028797018963967ull;
    ubits55(unsigned long long v) { value = v; }
    unsigned long long value : 55;
    static constexpr char BITS = 55;
    static constexpr unsigned long long MIN = 0;
    static constexpr unsigned long long RANGE = MAX + 1ull;
};

struct ubits56 {
    ubits56() = default;
    static constexpr unsigned long long MAX = 72057594037927935ull;
    ubits56(unsigned long long v) { value = v; }
    unsigned long long value : 56;
    static constexpr char BITS = 56;
    static constexpr unsigned long long MIN = 0;
    static constexpr unsigned long long RANGE = MAX + 1ull;
};

struct ubits57 {
    ubits57() = default;
    static constexpr unsigned long long MAX = 144115188075855871ull;
    ubits57(unsigned long long v) { value = v; }
    unsigned long long value : 57;
    static constexpr char BITS = 57;
    static constexpr unsigned long long MIN = 0;
    static constexpr unsigned long long RANGE = MAX + 1ull;
};

struct ubits58 {
    ubits58() = default;
    static constexpr unsigned long long MAX = 288230376151711743ull;
    ubits58(unsigned long long v) { value = v; }
    unsigned long long value : 58;
    static constexpr char BITS = 58;
    static constexpr unsigned long long MIN = 0;
    static constexpr unsigned long long RANGE = MAX + 1ull;
};

struct ubits59 {
    ubits59() = default;
    static constexpr unsigned long long MAX = 576460752303423487ull;
    ubits59(unsigned long long v) { value = v; }
    unsigned long long value : 59;
    static constexpr char BITS = 59;
    static constexpr unsigned long long MIN = 0;
    static constexpr unsigned long long RANGE = MAX + 1ull;
};

struct ubits60 {
    ubits60() = default;
    static constexpr unsigned long long MAX = 1152921504606846975ull;
    ubits60(unsigned long long v) { value = v; }
    unsigned long long value : 60;
    static constexpr char BITS = 60;
    static constexpr unsigned long long MIN = 0;
    static constexpr unsigned long long RANGE = MAX + 1ull;
};

struct ubits61 {
    ubits61() = default;
    static constexpr unsigned long long MAX = 2305843009213693951ull;
    ubits61(unsigned long long v) { value = v; }
    unsigned long long value : 61;
    static constexpr char BITS = 61;
    static constexpr unsigned long long MIN = 0;
    static constexpr unsigned long long RANGE = MAX + 1ull;
};

struct ubits62 {
    ubits62() = default;
    static constexpr unsigned long long MAX = 4611686018427387903ull;
    ubits62(unsigned long long v) { value = v; }
    unsigned long long value : 62;
    static constexpr char BITS = 62;
    static constexpr unsigned long long MIN = 0;
    static constexpr unsigned long long RANGE = MAX + 1ull;
};

struct ubits63 {
    ubits63() = default;
    static constexpr unsigned long long MAX = 9223372036854775807ull;
    ubits63(unsigned long long v) { value = v; }
    unsigned long long value : 63;
    static constexpr char BITS = 63;
    static constexpr unsigned long long MIN = 0;
    static constexpr unsigned long long RANGE = MAX + 1ull;
};

// struct ubits64 { }; // UnSupported

// Signed
struct sbits1 {
    sbits1() = default;
    static constexpr unsigned int MAX = 0u;
    static constexpr int MIN = -1;
    sbits1(int aValue) { value = aValue & 1u; }

    unsigned char value : 1;
    static constexpr char BITS = 1;
    static constexpr unsigned int RANGE = 2u;
};

struct sbits2 {
    sbits2() = default;
    static constexpr unsigned int MAX = 1u;
    static constexpr int MIN = -2;
    sbits2(int aValue) { value = aValue & 3u; }

    unsigned char value : 2;
    static constexpr char BITS = 2;
    static constexpr unsigned int RANGE = 4u;
};

struct sbits3 {
    sbits3() = default;
    static constexpr unsigned int MAX = 3u;
    static constexpr int MIN = -4;
    sbits3(int aValue) { value = aValue & 7u; }

    unsigned char value : 3;
    static constexpr char BITS = 3;
    static constexpr unsigned int RANGE = 8u;
};

struct sbits4 {
    sbits4() = default;
    static constexpr unsigned int MAX = 7u;
    static constexpr int MIN = -8;
    sbits4(int aValue) { value = aValue & 15u; }

    unsigned char value : 4;
    static constexpr char BITS = 4;
    static constexpr unsigned int RANGE = 16u;
};

struct sbits5 {
    sbits5() = default;
    static constexpr unsigned int MAX = 15u;
    static constexpr int MIN = -16;
    sbits5(int aValue) { value = aValue & 31u; }

    unsigned char value : 5;
    static constexpr char BITS = 5;
    static constexpr unsigned int RANGE = 32u;
};

struct sbits6 {
    sbits6() = default;
    static constexpr unsigned int MAX = 31u;
    static constexpr int MIN = -32;
    sbits6(int aValue) { value = aValue & 63u; }

    unsigned char value : 6;
    static constexpr char BITS = 6;
    static constexpr unsigned int RANGE = 64u;
};

struct sbits7 {
    sbits7() = default;
    static constexpr unsigned int MAX = 63u;
    static constexpr int MIN = -64;
    sbits7(int aValue) { value = aValue & 127u; }

    unsigned char value : 7;
    static constexpr char BITS = 7;
    static constexpr unsigned int RANGE = 128u;
};

struct sbits8 {
    sbits8() = default;
    static constexpr unsigned int MAX = 127u;
    static constexpr int MIN = -128;
    sbits8(int aValue) { value = aValue & 255u; }

    unsigned char value;

    static constexpr char BITS = 8;
    static constexpr unsigned int RANGE = 256u;
};

struct sbits9 {
    sbits9() = default;
    static constexpr unsigned int MAX = 255u;
    static constexpr int MIN = -256;
    sbits9(int aValue) { value = aValue & 511u; }

    unsigned short value : 9;

    static constexpr char BITS = 9;
    static constexpr unsigned int RANGE = 512u;
};

struct sbits10 {
    sbits10() = default;
    static constexpr unsigned int MAX = 511u;
    static constexpr int MIN = -512;
    sbits10(int aValue) { value = aValue & 1023u; }

    unsigned short value : 10;

    static constexpr char BITS = 10;
    static constexpr unsigned int RANGE = 1024u;
};

struct sbits11 {
    sbits11() = default;
    static constexpr unsigned int MAX = 1023u;
    static constexpr int MIN = -1024;
    sbits11(int aValue) { value = aValue & 2047u; }

    unsigned short value : 11;

    static constexpr char BITS = 11;
    static constexpr unsigned int RANGE = 2048u;
};

struct sbits12 {
    sbits12() = default;
    static constexpr unsigned int MAX = 2047u;
    static constexpr int MIN = -2048;
    sbits12(int aValue) { value = aValue & 4095u; }

    unsigned short value : 12;

    static constexpr char BITS = 12;
    static constexpr unsigned int RANGE = 4096u;
};

struct sbits13 {
    sbits13() = default;
    static constexpr unsigned int MAX = 4095u;
    static constexpr int MIN = -4096;
    sbits13(int aValue) { value = aValue & 8191u; }

    unsigned short value : 13;

    static constexpr char BITS = 13;
    static constexpr unsigned int RANGE = 8192u;
};

struct sbits14 {
    sbits14() = default;
    static constexpr unsigned int MAX = 8191u;
    static constexpr int MIN = -8192;
    sbits14(int aValue) { value = aValue & 16383u; }

    unsigned short value : 14;

    static constexpr char BITS = 14;
    static constexpr unsigned int RANGE = 16384u;
};

struct sbits15 {
    sbits15() = default;
    static constexpr unsigned int MAX = 16383u;
    static constexpr int MIN = -16384;
    sbits15(int aValue) { value = aValue & 32767u; }

    unsigned short value : 15;

    static constexpr char BITS = 15;
    static constexpr unsigned int RANGE = 32768u;
};

struct sbits16 {
    sbits16() = default;
    static constexpr unsigned int MAX = 32767u;
    static constexpr int MIN = -32768;
    sbits16(int aValue) { value = aValue & 65535u; }

    unsigned short value;

    static constexpr char BITS = 16;
    static constexpr unsigned int RANGE = 65536u;
};

struct sbits17 {
    sbits17() = default;
    static constexpr unsigned int MAX = 65535u;
    static constexpr int MIN = -65536;
    sbits17(int aValue) { value = aValue & 131071u; }

    unsigned int value : 17;

    static constexpr char BITS = 17;
    static constexpr unsigned int RANGE = 131072u;
};

struct sbits18 {
    sbits18() = default;
    static constexpr unsigned int MAX = 131071u;
    static constexpr int MIN = -131072;
    sbits18(int aValue) { value = aValue & 262143u; }

    unsigned int value : 18;

    static constexpr char BITS = 18;
    static constexpr unsigned int RANGE = 262144u;
};

struct sbits19 {
    sbits19() = default;
    static constexpr unsigned int MAX = 262143u;
    static constexpr int MIN = -262144;
    sbits19(int aValue) { value = aValue & 524287u; }

    unsigned int value : 19;

    static constexpr char BITS = 19;
    static constexpr unsigned int RANGE = 524288u;
};

struct sbits20 {
    sbits20() = default;
    static constexpr unsigned int MAX = 524287u;
    static constexpr int MIN = -524288;
    sbits20(int aValue) { value = aValue & 1048575u; }

    unsigned int value : 20;

    static constexpr char BITS = 20;
    static constexpr unsigned int RANGE = 1048576u;
};

struct sbits21 {
    sbits21() = default;
    static constexpr unsigned int MAX = 1048575u;
    static constexpr int MIN = -1048576;
    sbits21(int aValue) { value = aValue & 2097151u; }

    unsigned int value : 21;

    static constexpr char BITS = 21;
    static constexpr unsigned int RANGE = 2097152u;
};

struct sbits22 {
    sbits22() = default;
    static constexpr unsigned int MAX = 2097151u;
    static constexpr int MIN = -2097152;
    sbits22(int aValue) { value = aValue & 4194303u; }

    unsigned int value : 22;

    static constexpr char BITS = 22;
    static constexpr unsigned int RANGE = 4194304u;
};

struct sbits23 {
    sbits23() = default;
    static constexpr unsigned int MAX = 4194303u;
    static constexpr int MIN = -4194304;
    sbits23(int aValue) { value = aValue & 8388607u; }

    unsigned int value : 23;

    static constexpr char BITS = 23;
    static constexpr unsigned int RANGE = 8388608u;
};

struct sbits24 {
    sbits24() = default;
    static constexpr unsigned int MAX = 8388607u;
    static constexpr int MIN = -8388608;
    sbits24(int aValue) { value = aValue & 16777215u; }

    unsigned int value : 24;

    static constexpr char BITS = 24;
    static constexpr unsigned int RANGE = 16777216u;
};

struct sbits25 {
    sbits25() = default;
    static constexpr unsigned int MAX = 16777215u;
    static constexpr int MIN = -16777216;
    sbits25(int aValue) { value = aValue & 33554431u; }

    unsigned int value : 25;

    static constexpr char BITS = 25;
    static constexpr unsigned int RANGE = 33554432u;
};

struct sbits26 {
    sbits26() = default;
    static constexpr unsigned int MAX = 33554431u;
    static constexpr int MIN = -33554432;
    sbits26(int aValue) { value = aValue & 67108863u; }

    unsigned int value : 26;

    static constexpr char BITS = 26;
    static constexpr unsigned int RANGE = 67108864u;
};

struct sbits27 {
    sbits27() = default;
    static constexpr unsigned int MAX = 67108863u;
    static constexpr int MIN = -67108864;
    sbits27(int aValue) { value = aValue & 134217727u; }

    unsigned int value : 27;

    static constexpr char BITS = 27;
    static constexpr unsigned int RANGE = 134217728u;
};

struct sbits28 {
    sbits28() = default;
    static constexpr unsigned int MAX = 134217727u;
    static constexpr int MIN = -134217728;
    sbits28(int aValue) { value = aValue & 268435455u; }

    unsigned int value : 28;

    static constexpr char BITS = 28;
    static constexpr unsigned int RANGE = 268435456u;
};

struct sbits29 {
    sbits29() = default;
    static constexpr unsigned int MAX = 268435455u;
    static constexpr int MIN = -268435456;
    sbits29(int aValue) { value = aValue & 536870911u; }

    unsigned int value : 29;

    static constexpr char BITS = 29;
    static constexpr unsigned int RANGE = 536870912u;
};

struct sbits30 {
    sbits30() = default;
    static constexpr unsigned int MAX = 536870911u;
    static constexpr int MIN = -536870912;
    sbits30(int aValue) { value = aValue & 1073741823u; }

    unsigned int value : 30;

    static constexpr char BITS = 30;
    static constexpr unsigned int RANGE = 1073741824u;
};

struct sbits31 {
    sbits31() = default;
    static constexpr unsigned int MAX = 1073741823u;
    static constexpr int MIN = -1073741824;
    sbits31(int aValue) { value = aValue & 2147483647u; }

    unsigned int value : 31;

    static constexpr char BITS = 31;
    static constexpr unsigned int RANGE = 2147483648u;
};

struct sbits32 {
    sbits32() = default;
    static constexpr unsigned long long MAX = 2147483647ull;
    static constexpr long long MIN = -2147483648ll;
    sbits32(long long aValue) { value = aValue & 4294967295ull; }

    unsigned long long value : 32;

    static constexpr char BITS = 32;
    static constexpr unsigned long long RANGE = 4294967296ull;
};

struct sbits33 {
    sbits33() = default;
    static constexpr unsigned long long MAX = 4294967295ull;
    static constexpr long long MIN = -4294967296ll;
    sbits33(long long aValue) { value = aValue & 8589934591ull; }

    unsigned long long value : 33;

    static constexpr char BITS = 33;
    static constexpr unsigned long long RANGE = 8589934592ull;
};

struct sbits34 {
    sbits34() = default;
    static constexpr unsigned long long MAX = 17179869183ull;
    static constexpr long long MIN = -17179869184ll;
    sbits34(long long aValue) { value = aValue & 17179869183ull; }

    unsigned long long value : 34;

    static constexpr char BITS = 34;
    static constexpr unsigned long long RANGE = 17179869184ull;
};

struct sbits35 {
    sbits35() = default;
    static constexpr unsigned long long MAX = 34359738367ull;
    static constexpr long long MIN = -34359738368ll;
    sbits35(long long aValue) { value = aValue & 34359738367ull; }

    unsigned long long value : 35;

    static constexpr char BITS = 35;
    static constexpr unsigned long long RANGE = 34359738368ull;
};

struct sbits36 {
    sbits36() = default;
    static constexpr unsigned long long MAX = 68719476735ull;
    static constexpr long long MIN = -68719476736ll;
    sbits36(long long aValue) { value = aValue & 68719476735ull; }

    unsigned long long value : 36;

    static constexpr char BITS = 36;
    static constexpr unsigned long long RANGE = 68719476736ull;
};

struct sbits37 {
    sbits37() = default;
    static constexpr unsigned long long MAX = 137438953471ull;
    static constexpr long long MIN = -137438953472ll;
    sbits37(long long aValue) { value = aValue & 137438953471ull; }

    unsigned long long value : 37;

    static constexpr char BITS = 37;
    static constexpr unsigned long long RANGE = 137438953472ull;
};

struct sbits38 {
    sbits38() = default;
    static constexpr unsigned long long MAX = 274877906943ull;
    static constexpr long long MIN = -274877906944ll;
    sbits38(long long aValue) { value = aValue & 274877906943ull; }

    unsigned long long value : 38;

    static constexpr char BITS = 38;
    static constexpr unsigned long long RANGE = 274877906944ull;
};

struct sbits39 {
    sbits39() = default;
    static constexpr unsigned long long MAX = 549755813887ull;
    static constexpr long long MIN = -549755813888ll;
    sbits39(long long aValue) { value = aValue & 549755813887ull; }

    unsigned long long value : 39;

    static constexpr char BITS = 39;
    static constexpr unsigned long long RANGE = 549755813888ull;
};

struct sbits40 {
    sbits40() = default;
    static constexpr unsigned long long MAX = 1099511627775ull;
    static constexpr long long MIN = -1099511627776ll;
    sbits40(long long aValue) { value = aValue & 1099511627775ull; }

    unsigned long long value : 40;

    static constexpr char BITS = 40;
    static constexpr unsigned long long RANGE = 1099511627776ull;
};

struct sbits41 {
    sbits41() = default;
    static constexpr unsigned long long MAX = 2199023255551ull;
    static constexpr long long MIN = -2199023255552ll;
    sbits41(long long aValue) { value = aValue & 2199023255551ull; }

    unsigned long long value : 41;

    static constexpr char BITS = 41;
    static constexpr unsigned long long RANGE = 2199023255552ull;
};

struct sbits42 {
    sbits42() = default;
    static constexpr unsigned long long MAX = 4398046511103ull;
    static constexpr long long MIN = -4398046511104ll;
    sbits42(long long aValue) { value = aValue & 4398046511103ull; }

    unsigned long long value : 42;

    static constexpr char BITS = 42;
    static constexpr unsigned long long RANGE = 4398046511104ull;
};

struct sbits43 {
    sbits43() = default;
    static constexpr unsigned long long MAX = 8796093022207ull;
    static constexpr long long MIN = -8796093022208ll;
    sbits43(long long aValue) { value = aValue & 8796093022207ull; }

    unsigned long long value : 43;

    static constexpr char BITS = 43;
    static constexpr unsigned long long RANGE = 8796093022208ull;
};

struct sbits44 {
    sbits44() = default;
    static constexpr unsigned long long MAX = 17592186044415ull;
    static constexpr long long MIN = -17592186044416ll;
    sbits44(long long aValue) { value = aValue & 17592186044415ull; }

    unsigned long long value : 44;

    static constexpr char BITS = 44;
    static constexpr unsigned long long RANGE = 17592186044416ull;
};

struct sbits45 {
    sbits45() = default;
    static constexpr unsigned long long MAX = 35184372088831ull;
    static constexpr long long MIN = -35184372088832ll;
    sbits45(long long aValue) { value = aValue & 35184372088831ull; }

    unsigned long long value : 45;

    static constexpr char BITS = 45;
    static constexpr unsigned long long RANGE = 35184372088832ull;
};

struct sbits46 {
    sbits46() = default;
    static constexpr unsigned long long MAX = 70368744177663ull;
    static constexpr long long MIN = -70368744177664ll;
    sbits46(long long aValue) { value = aValue & 70368744177663ull; }

    unsigned long long value : 46;

    static constexpr char BITS = 46;
    static constexpr unsigned long long RANGE = 70368744177664ull;
};

struct sbits47 {
    sbits47() = default;
    static constexpr unsigned long long MAX = 140737488355327ull;
    static constexpr long long MIN = -140737488355328ll;
    sbits47(long long aValue) { value = aValue & 140737488355327ull; }

    unsigned long long value : 47;

    static constexpr char BITS = 47;
    static constexpr unsigned long long RANGE = 140737488355328ull;
};

struct sbits48 {
    sbits48() = default;
    static constexpr unsigned long long MAX = 281474976710655ull;
    static constexpr long long MIN = -281474976710656ll;
    sbits48(long long aValue) { value = aValue & 281474976710655ull; }

    unsigned long long value : 48;

    static constexpr char BITS = 48;
    static constexpr unsigned long long RANGE = 281474976710656ull;
};

struct sbits49 {
    sbits49() = default;
    static constexpr unsigned long long MAX = 562949953421311ull;
    static constexpr long long MIN = -562949953421312ll;
    sbits49(long long aValue) { value = aValue & 562949953421311ull; }

    unsigned long long value : 49;

    static constexpr char BITS = 49;
    static constexpr unsigned long long RANGE = 562949953421312ull;
};

struct sbits50 {
    sbits50() = default;
    static constexpr unsigned long long MAX = 1125899906842623ull;
    static constexpr long long MIN = -1125899906842624ll;
    sbits50(long long aValue) { value = aValue & 1125899906842623ull; }

    unsigned long long value : 50;

    static constexpr char BITS = 50;
    static constexpr unsigned long long RANGE = 1125899906842624ull;
};

struct sbits51 {
    sbits51() = default;
    static constexpr unsigned long long MAX = 2251799813685247ull;
    static constexpr long long MIN = -2251799813685248ll;
    sbits51(long long aValue) { value = aValue & 2251799813685247ull; }

    unsigned long long value : 51;

    static constexpr char BITS = 51;
    static constexpr unsigned long long RANGE = 2251799813685248ull;
};

struct sbits52 {
    sbits52() = default;
    static constexpr unsigned long long MAX = 4503599627370495ull;
    static constexpr long long MIN = -4503599627370496ll;
    sbits52(long long aValue) { value = aValue & 4503599627370495ull; }

    unsigned long long value : 52;

    static constexpr char BITS = 52;
    static constexpr unsigned long long RANGE = 4503599627370496ull;
};

struct sbits53 {
    sbits53() = default;
    static constexpr unsigned long long MAX = 9007199254740991ull;
    static constexpr long long MIN = -9007199254740992ll;
    sbits53(long long aValue) { value = aValue & 9007199254740991ull; }

    unsigned long long value : 53;

    static constexpr char BITS = 53;
    static constexpr unsigned long long RANGE = 9007199254740992ull;
};

struct sbits54 {
    sbits54() = default;
    static constexpr unsigned long long MAX = 18014398509481983ull;
    static constexpr long long MIN = -18014398509481984ll;
    sbits54(long long aValue) { value = aValue & 18014398509481983ull; }

    unsigned long long value : 54;

    static constexpr char BITS = 54;
    static constexpr unsigned long long RANGE = 18014398509481984ull;
};

struct sbits55 {
    sbits55() = default;
    static constexpr unsigned long long MAX = 36028797018963967ull;
    static constexpr long long MIN = -36028797018963968ll;
    sbits55(long long aValue) { value = aValue & 36028797018963967ull; }

    unsigned long long value : 55;

    static constexpr char BITS = 55;
    static constexpr unsigned long long RANGE = 36028797018963968ull;
};

struct sbits56
{
    sbits56() = default;
    static constexpr unsigned long long MAX = 72057594037927935ull;
    static constexpr long long MIN = -72057594037927936ll;
    sbits56(long long aValue) { value = aValue & 72057594037927935ull; }

    unsigned long long value : 56;

    static constexpr char BITS = 56;
    static constexpr unsigned long long RANGE = 72057594037927936ull;
};

struct sbits57 {
    sbits57() = default;
    static constexpr unsigned long long MAX = 144115188075855871ull;
    static constexpr long long MIN = -144115188075855872ll;
    sbits57(long long aValue) { value = aValue & 144115188075855871ull; }

    unsigned long long value : 57;

    static constexpr char BITS = 57;
    static constexpr unsigned long long RANGE = 144115188075855872ull;
};

struct sbits58 {
    sbits58() = default;
    static constexpr unsigned long long MAX = 288230376151711743ull;
    static constexpr long long MIN = -288230376151711744ll;
    sbits58(long long aValue) { value = aValue & 288230376151711743ull; }

    unsigned long long value : 58;

    static constexpr char BITS = 58;
    static constexpr unsigned long long RANGE = 288230376151711744ull;
};

struct sbits59 {
    sbits59() = default;
    static constexpr unsigned long long MAX = 576460752303423487ull;
    static constexpr long long MIN = -576460752303423488ll;
    sbits59(long long aValue) { value = aValue & 576460752303423487ull; }

    unsigned long long value : 59;

    static constexpr char BITS = 59;
    static constexpr unsigned long long RANGE = 576460752303423488ull;
};

struct sbits60 {
    sbits60() = default;
    static constexpr unsigned long long MAX = 1152921504606846975ull;
    static constexpr long long MIN = -1152921504606846976ll;
    sbits60(long long aValue) { value = aValue & 1152921504606846975ull; }

    unsigned long long value : 60;

    static constexpr char BITS = 60;
    static constexpr unsigned long long RANGE = 1152921504606846976ull;
};

struct sbits61 {
    sbits61() = default;
    static constexpr unsigned long long MAX = 2305843009213693951ull;
    static constexpr long long MIN = -2305843009213693952ll;
    sbits61(long long aValue) { value = aValue & 2305843009213693951ull; }

    unsigned long long value : 61;

    static constexpr char BITS = 61;
    static constexpr unsigned long long RANGE = 2305843009213693952ull;
};

struct sbits62 {
    sbits62() = default;
    static constexpr unsigned long long MAX = 4611686018427387903ull;
    static constexpr long long MIN = -4611686018427387904ll;
    sbits62(long long aValue) { value = aValue & 4611686018427387903ull; }

    unsigned long long value : 62;

    static constexpr char BITS = 62;
    static constexpr unsigned long long RANGE = 4611686018427387904ull;
};

struct sbits63 {
    sbits63() = default;
    static constexpr unsigned long long MAX = 4611686018427387903ull;
    static constexpr long long MIN = -4611686018427387904ll;
    sbits63(long long aValue) { value = aValue & 9223372036854775807ull; }

    unsigned long long value : 63;

    static constexpr char BITS = 63;
    static constexpr unsigned long long RANGE = 9223372036854775808ull;
};

// struct sbits64 { }; // UnSupported
