#ifndef apiardancegame_h__
#define apiardancegame_h__

#include "orangefilter.h"

/************************************************************************/
/* Module export definition.                                            */
/************************************************************************/
#if defined(OF_WIN32) || defined(_WIN32) || defined(WIN32)
#   ifdef ARDANCEGAME_LIB
#       define ARDANCEGAME_API __declspec(dllexport)
#   else
#       define ARDANCEGAME_API __declspec(dllimport)
#   endif
#else
#   define ARDANCEGAME_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define KFilterGameARDance "GameARDanceFilter"
#define KGameARDance	   "ARDanceGame"

typedef enum _OF_ARDanceGameEvent
{
	OF_ARDanceGameEvent_RhythmMatch,
	OF_ARDanceGameEvent_PlayMusic,
	OF_ARDanceGameEvent_PauseMusic,
	OF_ARDanceGameEvent_ResumeMusic,
	OF_ARDanceGameEvent_StopMusic,
	OF_ARDanceGameEvent_Restart,
} OF_ARDanceGameEvent;

typedef struct _OF_ARDanceGameMatch
{
	char rhythmId[128];
	OFInt32 rhythmType;
	OFInt32 rhythmScore;
	float rhythmMatchTimeError;
} OF_ARDanceGameMatch;

typedef struct _OF_ARDanceGameEventData
{
	OF_ARDanceGameMatch* matchRhythms;
	OFInt32 matchRhythmCount;
	void* userData;
	char songFile[256];
} OF_ARDanceGameEventData;

typedef enum _OF_ARDanceGameEventIn
{
	OF_ARDanceGameEventIn_NewRhythm,
	
	OF_ARDanceGameEventIn_UpdateTime,
    OF_ARDanceGameEventIn_UpdateMusicTime,
    OF_ARDanceGameEventIn_MusicEnd,

    OF_ARDanceGameEventIn_UpdateScore,
	OF_ARDanceGameEventIn_ShowBingo,
	OF_ARDanceGameEventIn_ShowMiss,
    
} OF_ARDanceGameEventIn;

typedef struct _OF_ARDanceGameEventDataIn
{
	char rhythmId[128];
	OFInt32 rhythmType;
	OFInt32 rhythmScore;
	//float moveSpeed;
	//OFInt32 multipleCount;
	OFInt32 showScore;
	OFInt32 showTime;
    float musicTime;
} OF_ARDanceGameEventDataIn;

typedef void(*ARDANCE_GAME_CALLBACK)(OF_ARDanceGameEvent, OF_ARDanceGameEventData);
typedef struct _OF_ARDanceGameData
{
	OFInt32 gameMode; // must be 0
	OFInt32 topScore;
	OFInt32 isLowDevice;
	OFInt32 isHideReady;
	OFInt32 initScore;
	OFInt32 initTime;
	OFPath rhythmFile;
	ARDANCE_GAME_CALLBACK gameCallbackFunc;
	void* userData;
} OF_ARDanceGameData;

typedef OF_ARDanceGameData OF_GameARDanceFilterExtraData;

ARDANCEGAME_API OF_Result OF_InitARDanceGame(void);

ARDANCEGAME_API OF_Result OF_UnInitARDanceGame(void);

#ifdef __cplusplus
}
#endif

#endif // apiardancegame_h__
