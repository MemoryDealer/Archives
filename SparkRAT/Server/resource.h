// ID of Dialog
#define DLG_MAIN            51
#define DLG_MAIN_IDC_STATUS 52

// ID's of menu
#define IDR_MENU            201
#define IDR_FILE_QUIT       202
#define IDR_HELP_ABOUT      203

// ID's of icons
#define IDI_MAIN            301
#define IDI_GO              302

// ID's of Controls
#define IDC_EDT_HOST        901
#define IDC_BTN_CONNECT     902
#define IDC_BTN_VERIFY      9021
#define IDC_EDT_PORT        903
#define IDC_EDT_STATUS      904
#define IDC_BTN_BEEP        905
#define IDC_BTN_MESSAGE     906
#define IDC_BTN_CMD         907
#define IDC_BTN_RUN         908
#define IDC_BTN_SHUTDOWN    909
#define IDC_BTN_MONITOR     910
#define IDC_BTN_REG         911
#define IDC_BTN_KEY         912
#define IDC_BTN_KILLPROC    913
#define IDC_BTN_FILES       914
#define IDC_BTN_FUN         915
#define IDC_BTN_BLOCKWEB    916
#define IDC_BTN_VOICE       917
#define IDC_BTN_CHAT        918
#define IDC_BTN_DISPLAY     919
#define IDC_BTN_PRINT       920

#define IDC_BTN_UNINST      950
#define IDC_BTN_SERVER      975

// Message Dialog
#define DLG_MSG                     102
#define DLG_MSG_IDC_EDT_MSG         1021
#define DLG_MSG_IDC_EDT_TITLE       1022
#define DLG_MSG_IDC_CMB_TYPE        1023
#define DLG_MSG_IDC_CMB_ICON        1024
#define DLG_MSG_IDC_CHK_RESULT      1025
#define DLG_MSG_IDC_BTN_SEND        1026

// Beep Dialog
#define DLG_BEEP                    103
#define DLG_BEEP_IDC_EDT_FREQ       1031
#define DLG_BEEP_IDC_EDT_DUR        1032
#define DLG_BEEP_IDC_BTN_SEND       1033

// CMD Dialog
#define DLG_CMD                     104
#define DLG_CMD_IDC_EDT_CMD         1041
#define DLG_CMD_IDC_CHK_HIDE        1042
#define DLG_CMD_IDC_CHK_PAUSE       1043
#define DLG_CMD_IDC_BTN_SEND        1044

// Run Dialog
#define DLG_RUN                     105
#define DLG_RUN_IDC_EDT_RUN         1051
#define DLG_RUN_IDC_BTN_RUN         1052

// Shutdown Dialog
#define DLG_SHUTDOWN                106
#define DLG_SHUTDOWN_IDC_RDO_DOWN   1061
#define DLG_SHUTDOWN_IDC_RDO_RES    1062
#define DLG_SHUTDOWN_IDC_RDO_LOGOFF 1063
#define DLG_SHUTDOWN_IDC_EDT_TIME   1064
#define DLG_SHUTDOWN_IDC_EDT_REASON 1065
#define DLG_SHUTDOWN_IDC_BTN_SEND   1066

// Monitor Dialog
#define DLG_MON                     107
#define DLG_MON_IDC_RDO_OFF         1071
#define DLG_MON_IDC_RDO_ON          1072
#define DLG_MON_IDC_EDT_TIMES       1073
#define DLG_MON_IDC_EDT_SECONDS     1074
#define DLG_MON_IDC_EDT_INTERVAL    1075
#define DLG_MON_IDC_BTN_SEND        1076

// Registry Dialog
#define DLG_REG                     108
#define DLG_REG_IDC_CMB_HIVE        1081
#define DLG_REG_IDC_EDT_KEY         1082
#define DLG_REG_IDC_BTN_QUERY       1083
#define DLG_REG_IDC_BTN_ADD         1084
#define DLG_REG_IDC_LST_VALUES      1085
#define DLG_REG_IDC_BTN_DELVAL      1086
#define DLG_REG_IDC_BTN_EDITVAL     1087
#define DLG_REG_IDC_EDT_TYPE        1088
#define DLG_REG_IDC_EDT_DATA        1089
#define DLG_REG_IDC_CMB_ADDHIVE     8001 // use 8,000+ for extended items
#define DLG_REG_IDC_EDT_ADDSUBKEY   8002
#define DLG_REG_IDC_EDT_ADDVAL      8003
#define DLG_REG_IDC_CMB_ADDTYPE     8004
#define DLG_REG_IDC_EDT_ADDNAME     8005

// Keylogger Dialog
#define DLG_KEY                     109
#define DLG_KEY_IDC_BTN_START       1091
#define DLG_KEY_IDC_BTN_GETLOG      1092

// Kill Proc Dialog
#define DLG_KILL                    110
#define DLG_KILL_IDC_EDT_PROC       1101
#define DLG_KILL_IDC_BTN_KILL       1102

// Files Dialog
#define DLG_FILES                   132
#define DLG_FILES_IDC_BTN_DRIVES    1320
#define DLG_FILES_IDC_CMB_DRIVES    1321
#define DLG_FILES_IDC_EDT_PATH      1322
#define DLG_FILES_IDC_LST_DIRS      1323
#define DLG_FILES_IDC_LST_FILES     1324
#define DLG_FILES_IDC_BTN_DOWNLOAD  1325
#define DLG_FILES_IDC_BTN_UPLOAD    1326
#define DLG_FILES_IDC_BTN_GO        1327

// Fun Dialog
#define DLG_FUN                     111
#define DLG_FUN_IDC_BTN_CMOUSE      1111
#define DLG_FUN_IDC_BTN_START       1112
#define DLG_FUN_IDC_BTN_TASKBAR     1113
#define DLG_FUN_IDC_BTN_CDTRAY      1114
#define DLG_FUN_IDC_BTN_BLOCK       1115
#define DLG_FUN_IDC_BTN__CMOUSE     1116
#define DLG_FUN_IDC_BTN__START      1117
#define DLG_FUN_IDC_BTN__TASKBAR    1118
#define DLG_FUN_IDC_BTN__CDTRAY     1119
#define DLG_FUN_IDC_BTN__BLOCK      11111
#define DLG_FUN_IDC_EDT_TEXT        11112
#define DLG_FUN_IDC_BTN_TEXT        11113

// BlockWeb Dialog
#define DLG_BLOCKWEB                112
#define DLG_BLOCKWEB_IDC_EDT_WEB    1121
#define DLG_BLOCKWEB_IDC_BTN_BLOCK  1122

// Voice Dialog
#define DLG_VOICE                   113
#define DLG_VOICE_IDC_EDT_PHRASE    1131
#define DLG_VOICE_IDC_BTN_SEND      1132

// Chat Dialog
#define DLG_CHAT                    114
#define DLG_CHAT_IDC_LST_CHAT       1141
#define DLG_CHAT_IDC_EDT_SAY        1142
#define DLG_CHAT_IDC_BTN_SEND       1143

// Display Dialog
#define DLG_DISP                    115
#define DLG_DISP_IDC_CHK_BWREVERSE  1151
#define DLG_DISP_IDC_EDT_RANGE      1152
#define DLG_DISP_IDC_BTN_MELT       1153
#define DLG_DISP_IDC_BTN_TEST       1154

// Printer Dialog
#define DLG_PRINT                   116
#define DLG_PRINT_IDC_EDT_TEXT      1161
#define DLG_PRINT_IDC_BTN_PRINT     1162

//---

// Configure Server Dialog
#define DLG_SERVER                  175



// Uinstall Dialog
#define DLG_UNINST                  150
#define DLG_UNINST_IDC_BTN_LOCAL    1501
#define DLG_UNINST_IDC_BTN_REMOTE   1502

