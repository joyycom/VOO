#ifndef apifacedancegame_h__
#define apifacedancegame_h__

#include "orangefilter.h"

/************************************************************************/
/* Module export definition.                                            */
/************************************************************************/
#if defined(OF_WIN32) || defined(_WIN32) || defined(WIN32)
#   ifdef FACEDANCEGAME_LIB
#       define FACEDANCEGAME_API __declspec(dllexport)
#   else
#       define FACEDANCEGAME_API __declspec(dllimport)
#   endif
#else
#   define FACEDANCEGAME_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Face Dance Game
#define KFilterGameFaceDance "GameFaceDanceFilter"
#define KGameFaceDance	     "FaceDanceGame"

typedef enum _OF_FaceGameEvent
{
    OF_FaceGameEvent_FaceMatch,
} OF_FaceGameEvent;

typedef struct _OF_FaceGameMatchFace
{
    char faceId[128];
    OFInt32 faceType;
    OFInt32 faceScore;
    float faceMatchTime;
} OF_FaceGameMatchFace;

typedef struct _OF_FaceGameEventData
{
    OF_FaceGameMatchFace* matchFaces;
    OFInt32 matchFaceCount;
    void* userData;
} OF_FaceGameEventData;

typedef enum _OF_FaceGameEventIn
{
    OF_FaceGameEventIn_NewFace,
    OF_FaceGameEventIn_UpdateScore,
    OF_FaceGameEventIn_UpdateTime,
    OF_FaceGameEventIn_ShowBingo,
    OF_FaceGameEventIn_ShowMiss,
    OF_FaceGameEventIn_MatchEnable,
    OF_FaceGameEventIn_GameOver,
	OF_FaceGameEventIn_Combo,
	OF_FaceGameEventIn_TimeOver,
} OF_FaceGameEventIn;

typedef struct _OF_FaceGameEventDataIn
{
    char faceId[128];
    OFInt32 faceType;
    OFInt32 faceScore;
    float position; // 0.0 ~ 1.0
    float moveSpeed;
    OFInt32 multipleCount;
    OFInt32 showScore;
    OFInt32 showTime;
    OFBool matchEnable; // default OF_TRUE
	int combo;
} OF_FaceGameEventDataIn;

typedef void(*FACE_GAME_CALLBACK)(OF_FaceGameEvent, OF_FaceGameEventData);
typedef struct _OF_FaceGameData
{
    OFInt32 gameMode; // must be 0
    OFInt32 topScore;
    OFInt32 isLowDevice;
    OFInt32 isHideReady;
    OFInt32 initScore;
    OFInt32 initTime;
    FACE_GAME_CALLBACK gameCallbackFunc;
    void* userData;
} OF_FaceGameData;

typedef OF_FaceGameData OF_GameFaceDanceFilterExtraData;

// Wolf Face Dance Game
#define KFilterWolfGameFaceDance "WolfGameFaceDanceFilter"
#define KGameWolfFaceDance	     "WolfFaceDanceGame"

typedef enum _OF_WolfFaceGameEvent
{
    OF_WolfFaceGameEvent_FaceMatch,
    OF_WolfFaceGameEvent_FaceMiss,
} OF_WolfFaceGameEvent;

typedef struct _OF_WolfFaceGameFaces
{
    char faceId[128];
    OFInt32 faceType;
    OFInt32 itemType;
    OFInt32 faceScore;
    float faceMatchTime;
} OF_WolfFaceGameFace;

typedef struct _OF_WolfFaceGameEventData
{
    OF_WolfFaceGameFace* faces;
    OFInt32 faceCount;
    void* userData;
} OF_WolfFaceGameEventData;

typedef enum _OF_WolfFaceGameEventIn
{
    OF_WolfFaceGameEventIn_NewFace,
    OF_WolfFaceGameEventIn_UpdateScore,
    OF_WolfFaceGameEventIn_UpdateTime,
    OF_WolfFaceGameEventIn_ShowBingo,
    OF_WolfFaceGameEventIn_ShowMiss,
    OF_WolfFaceGameEventIn_MatchEnable,
    OF_WolfFaceGameEventIn_GameOver,
    OF_WolfFaceGameEventIn_ShowDouble,
    OF_WolfFaceGameEventIn_CleanUp,
    OF_WolfFaceGameEventIn_ClearFaces,
} OF_WolfFaceGameEventIn;

typedef struct _OF_WolfFaceGameEventDataIn
{
    char faceId[128];
    OFInt32 faceType;
    OFInt32 itemType;
    OFInt32 faceStyle;
    OFInt32 faceScore;
    float position; // 0.0 ~ 1.0
    float moveSpeed;
    OFInt32 multipleCount;
    OFInt32 showScore;
    OFInt32 showTime;
    OFBool matchEnable; // default OF_TRUE
    OFBool showDoubleEnable; // default OF_FALSE
    OFBool isCleanUp;// default OF_FALSE
} OF_WolfFaceGameEventDataIn;

typedef void(*WOLF_FACE_GAME_CALLBACK)(OF_WolfFaceGameEvent, OF_WolfFaceGameEventData);
typedef struct _OF_WolfFaceGameData
{
    OFInt32 gameMode; // must be 0
    OFInt32 topScore;
    OFInt32 isLowDevice;
    OFInt32 isHideReady;
    OFInt32 initScore;
    OFInt32 initTime;
    OFInt32 contentWidth;
    OFInt32 contentHeight;
    OFInt32 isShowDebugFace;
    WOLF_FACE_GAME_CALLBACK gameCallbackFunc;
    void* userData;
} OF_WolfFaceGameData;

typedef OF_WolfFaceGameData OF_WolfGameFaceDanceFilterExtraData;

FACEDANCEGAME_API OF_Result OF_InitFaceDanceGame(void);

FACEDANCEGAME_API OF_Result OF_UnInitFaceDanceGame(void);

#ifdef __cplusplus
}
#endif

#endif // apifacedancegame_h__
