#pragma once
enum class CommandType {
    NONE = -1,             // No command assigned
    CMD_SM_MENU = 0,       // Key "Menu"
    CMD_SM_FIT = 1,        // Key "Fit"
    CMD_SM_T = 2,          // Key "Top"
    CMD_SM_R = 4,          // Key "Right"
    CMD_SM_F = 5,          // Key "Front"
    CMD_SM_RCW = 8,        // Key "Roll 90°CW"
    CMD_SM_1 = 12,         // Key "1"
    CMD_SM_2 = 13,         // Key "2"
    CMD_SM_3 = 14,         // Key "3"
    CMD_SM_4 = 15,         // Key "4"
    CMD_SM_ESC = 22,       // Key "ESC"
    CMD_SM_ALT = 23,       // Key "ALT"
    CMD_SM_SHFT = 24,      // Key "SHIFT"
    CMD_SM_CTRL = 25,      // Key "CTRL"
    CMD_SM_ROT = 26,       // Key "Rotate"
    KILLROTATION = 100,    // Key "Kill Rotation"
    KILLTRANSLATION = 101, // Key "Kill Translation"
};
