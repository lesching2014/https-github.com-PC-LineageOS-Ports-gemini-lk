#include "platform/partition.h"


struct part_name_map g_part_name_map[PART_MAX_COUNT] = {
	{"preloader",   "preloader",    "raw data", 0,  0,  0},
	{"preloader2",  "preloader2",   "raw data", 1,  0,  0},
	{"proinfo", "proinfo",  "raw data", 2,  0,  0},
	{"nvram",   "nvram",    "raw data", 3,  0,  0},
	{"protect1",    "protect1", "ext4",         4,  0,  0},
	{"protect2",    "protect2", "ext4",         5,  0,  0},
	{"lk",      "lk",       "raw data", 6,  1,  1},
	{"lk2",     "lk2",      "raw data", 7,  1,  1},
	{"para",    "para",     "raw data", 8,  0,  0},
	{"boot",    "boot",     "raw data", 9,  1,  1},
	{"recovery",    "recovery", "raw data", 10,  1,  1},
	{"logo",    "logo",     "raw data", 11,  0,  0},
	{"expdb",   "expdb",    "raw data", 12, 0,  0},
	{"seccfg",  "seccfg",   "raw data", 13, 0,  0},
	{"oemkeystore", "oemkeystore",  "raw data",     14,     0,      0},
	{"keystore",    "keystore",     "raw data",     15,     1,      1},
	{"tee1",        "tee1",         "raw data",     16,     0,      0},
	{"tee2",        "tee2",          "raw data",    17,     0,      0},
	{"persist", "persist",  "ext4",     18, 0,  0},
	{"custom",  "custom",   "ext4",     19, 0,  0},
	{"efuse",       "efuse",        "raw data",     20,     0,      0},
	{"frp",         "frp",          "raw data",     21,     0,      0},
	{"scp1",         "scp1",          "raw data",     22,     0,      0},
	{"scp2",         "scp2",          "raw data",     23,     0,      0},
	{"md1img",         "md1img",          "raw data",     24,     0,      0},
	{"md1dsp",         "md1dsp",          "raw data",     25,     0,      0},
	{"md1arm7",         "md1arm7",          "raw data",     26,     0,      0},
	{"md3img",         "md3img",          "raw data",     27,     0,      0},
	{"nvcfg",        "nvcfg",       "ext4",         28,     0,      0},
	{"nvdata",       "nvdata",      "ext4",         29,     0,      0},
	{"metadata",     "metadata",    "raw data",     30,     0,      0},
	{"system",  "system",   "ext4",     31, 1,  1},
	{"cache",   "cache",    "ext4",     32, 1,  1},
	{"userdata",    "userdata", "ext4",     33, 1,  1},
	{"intsd",   "intsd",    "fat",      34, 0,  0},
	{"otp",     "otp",      "raw data", 35, 0,  0},
	{"flashinfo",   "flashinfo",    "raw data", 36, 0,  0},
	{""},
};
