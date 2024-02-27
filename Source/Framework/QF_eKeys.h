#pragma once

// #TODO Rename to "QF_eInputKeys" or more input related, instead of auth keys, or something like that

// #TODO Maybe deprecate the current implementation. Create keys that represent actions like:
// Camera_Move, Editor_KeyPressW, Scene_Save, and any other action that can be abstracted from the keyboard key value (ascii or otherwise).
// Maybe eQwerkEKeyAction or something

namespace QwerkE {

    enum eKeyState
    {
        eKeyState_Release = 0,
        eKeyState_Press,
        // eKeyState_Held, ? affects GetIsKeyDown()

        eKeyState_Max
    };

    enum eKeys // #TODO Review having values match an ASCII table. This makes config file key re-mapping easier to implement
    {
        eKeys_NULL_KEY = 0,

        // #TODO Load with ascii chars
        /*
        NUL	    00	00000000	0
        SOH	    01	00000001	1
        STX	    02	00000010	2
        ETX	    03	00000011	3
        EOT	    04	00000100	4
        ENQ	    05	00000101	5
        ACK	    06	00000110	6
        BEL	    07	00000111	7
        BS	    08	00001000	8
        HT	    09	00001001	9
        LF	    0A	00001010	10
        VT	    0B	00001011	11
        FF	    0C	00001100	12
        CR	    0D	00001101	13
        SO	    0E	00001110	14
        SI	    0F	00001111	15
        DLE	    10	00010000	16
        DC1	    11	00010001	17
        DC2	    12	00010010	18
        DC3	    13	00010011	19
        DC4	    14	00010100	20
        NAK	    15	00010101	21
        SYN	    16	00010110	22
        ETB	    17	00010111	23
        CAN	    18	00011000	24
        EM	    19	00011001	25
        SUB	    1A	00011010	26
        ESC	    1B	00011011	27
        FS	    1C	00011100	28
        GS	    1D	00011101	29
        RS	    1E	00011110	30
        US	    1F	00011111	31
        Space	20	00100000	32
        !	    21	00100001	33
        "	    22	00100010	34
        #	    23	00100011	35
        $	    24	00100100	36
        %	    25	00100101	37
        &	    26	00100110	38
        '	    27	00100111	39
        (	    28	00101000	40
        )	    29	00101001	41
        *	    2A	00101010	42
        +	    2B	00101011	43
        ,	    2C	00101100	44
        -	    2D	00101101	45
        .	    2E	00101110	46
        /	    2F	00101111	47
        0	    30	00110000	48
        1	    31	00110001	49
        2	    32	00110010	50
        3	    33	00110011	51
        4	    34	00110100	52
        5	    35	00110101	53
        6	    36	00110110	54
        7	    37	00110111	55
        8	    38	00111000	56
        9	    39	00111001	57
        :	    3A	00111010	58
        ;	    3B	00111011	59
        <	    3C	00111100	60
        =	    3D	00111101	61
        >	    3E	00111110	62
        ?	    3F	00111111	63
        @	    40	01000000	64
        A	    41	01000001	65
        B	    42	01000010	66
        C	    43	01000011	67
        D	    44	01000100	68
        E	    45	01000101	69
        F	    46	01000110	70
        G	    47	01000111	71
        H	    48	01001000	72
        I	    49	01001001	73
        J	    4A	01001010	74
        K	    4B	01001011	75
        L	    4C	01001100	76
        M	    4D	01001101	77
        N	    4E	01001110	78
        O	    4F	01001111	79
        P	    50	01010000	80
        Q	    51	01010001	81
        R	    52	01010010	82
        S	    53	01010011	83
        T	    54	01010100	84
        U	    55	01010101	85
        V	    56	01010110	86
        W	    57	01010111	87
        X	    58	01011000	88
        Y	    59	01011001	89
        Z	    5A	01011010	90
        [	    5B	01011011	91
        \	    5C	01011100	92
        ]	    5D	01011101	93
        ^	    5E	01011110	94
        _	    5F	01011111	95
        `	    60	01100000	96
        a	    61	01100001	97
        b	    62	01100010	98
        c	    63	01100011	99
        d	    64	01100100	100
        e	    65	01100101	101
        f	    66	01100110	102
        g	    67	01100111	103
        h	    68	01101000	104
        i	    69	01101001	105
        j	    6A	01101010	106
        k	    6B	01101011	107
        l	    6C	01101100	108
        m	    6D	01101101	109
        n	    6E	01101110	110
        o	    6F	01101111	111
        p	    70	01110000	112
        q	    71	01110001	113
        r	    72	01110010	114
        s	    73	01110011	115
        t	    74	01110100	116
        u	    75	01110101	117
        v	    76	01110110	118
        w	    77	01110111	119
        x	    78	01111000	120
        y	    79	01111001	121
        z	    7A	01111010	122
        {	    7B	01111011	123
        |	    7C	01111100	124
        }	    7D	01111101	125
        ~	    7E	01111110	126
        DEL	    7F	01111111	127
        */

        // Mouse
        eKeys_LeftClick,
        eKeys_RightClick,
        eKeys_MiddleClick,

        eKeys_MouseButton1,
        eKeys_MouseButton2,
        eKeys_MouseButton3,
        eKeys_MouseButton4,
        eKeys_MouseButton5,
        eKeys_MouseButton6,
        eKeys_MouseButton7,
        eKeys_MouseButton8,

        // #define 	GLFW_MOD_SHIFT   0x0001

        eKeys_CTRL,
        eKeys_SHIFT,
        eKeys_ALT,

        eKeys_LCTRL,
        eKeys_LSHIFT,
        eKeys_LALT,

        eKeys_RCTRL,
        eKeys_RSHIFT,
        eKeys_RALT,

        eKeys_Return,
        eKeys_Insert,
        eKeys_Delete,
        eKeys_PageUp,
        eKeys_PageDown,
        eKeys_Home,
        eKeys_End,
        eKeys_Tab,
        eKeys_Escape,
        eKeys_Backspace,
        eKeys_PrintScreen,
        eKeys_Pause,
        eKeys_Menu,
        eKeys_FN,

        eKeys_CapsLock,
        eKeys_NumLock,
        eKeys_ScrollLock,

        eKeys_Pad_Decimal,
        eKeys_Pad_Return,
        eKeys_Pad_Plus,
        eKeys_Pad_Minus,
        eKeys_Pad_Division,
        eKeys_Pad_Multiplication,

        eKeys_0 = 48,
        eKeys_1,
        eKeys_2,
        eKeys_3,
        eKeys_4,
        eKeys_5,
        eKeys_6,
        eKeys_7,
        eKeys_8,
        eKeys_9,

        eKeys_A = 65,
        eKeys_B,
        eKeys_C,
        eKeys_D,
        eKeys_E,
        eKeys_F,
        eKeys_G,
        eKeys_H,
        eKeys_I,
        eKeys_J,
        eKeys_K,
        eKeys_L,
        eKeys_M,
        eKeys_N,
        eKeys_O,
        eKeys_P,
        eKeys_Q,
        eKeys_R,
        eKeys_S,
        eKeys_T,
        eKeys_U,
        eKeys_V,
        eKeys_W,
        eKeys_X,
        eKeys_Y,
        eKeys_Z,

        eKeys_Pad_0,
        eKeys_Pad_1,
        eKeys_Pad_2,
        eKeys_Pad_3,
        eKeys_Pad_4,
        eKeys_Pad_5,
        eKeys_Pad_6,
        eKeys_Pad_7,
        eKeys_Pad_8,
        eKeys_Pad_9,

        eKeys_Space,
        eKeys_Apostrophe,
        eKeys_Comma,
        eKeys_Period,
        eKeys_ForwardSlash,
        eKeys_BackSlash,
        eKeys_SemiColon,
        eKeys_LeftBracket,
        eKeys_RightBracket,
        eKeys_Tilde, // 96
        eKeys_Minus,
        eKeys_Equal,

        eKeys_Up,
        eKeys_Down,
        eKeys_Left,
        eKeys_Right,

        eKeys_F1,
        eKeys_F2,
        eKeys_F3,
        eKeys_F4,
        eKeys_F5,
        eKeys_F6,
        eKeys_F7,
        eKeys_F8,
        eKeys_F9,
        eKeys_F10,
        eKeys_F11,
        eKeys_F12,
        eKeys_F13,
        eKeys_F14,
        eKeys_F15,
        eKeys_F16,
        eKeys_F17,
        eKeys_F18,
        eKeys_F19,
        eKeys_F20,
        eKeys_F21,
        eKeys_F22,
        eKeys_F23,
        eKeys_F24,
        eKeys_F25,

        // System Keys
        eKeys_System, // Window key, Command Key, etc

        // Media Keys? // Play/pause, previous, next, volume, mute/unmute

        // Macro Keys?

        eKeys_MAX
    };

}
