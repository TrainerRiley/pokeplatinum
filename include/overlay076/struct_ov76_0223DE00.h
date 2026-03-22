#ifndef POKEPLATINUM_STRUCT_OV76_0223DE00_H
#define POKEPLATINUM_STRUCT_OV76_0223DE00_H

#include "struct_decls/font_oam.h"
#include "struct_decls/struct_02012744_decl.h"
#include "struct_decls/struct_02015920_decl.h"
#include "struct_defs/seal_case.h"
#include "struct_defs/struct_02097F18.h"

#include "battle_anim/struct_ball_rotation_decl.h"
#include "battle_anim/struct_ov12_02235FE0_decl.h"
#include "overlay022/struct_ov22_02258A48.h"

#include "bg_window.h"
#include "char_transfer.h"
#include "g3d_pipeline.h"
#include "menu.h"
#include "narc.h"
#include "palette.h"
#include "pokemon_anim.h"
#include "pokemon_sprite.h"
#include "sprite_system.h"
#include "string_list.h"
#include "touch_screen.h"
#include "touch_screen_actions.h"

#define SEALS_PER_PAGE                   8
#define BALL_CAPSULE_INVALID_PARTY_INDEX 0xff

#define CAPSULES_PER_ROW    4
#define CAPSULES_PER_COLUMN 3

enum BallCapsuleStatus {
    BALL_CAPSULE_EMPTY = 0,
    BALL_CAPSULE_EDITED,
    BALL_CAPSULE_EDITED_AND_ASSIGNED,
    BALL_CAPSULE_EMPTY_AND_ASSIGNED,
};

// Static buttons (seal selectors, seal pages, etc.)
enum BallCapsuleButtons {
    BALL_CAPSULE_BUTTON_SEAL1 = 0,
    BALL_CAPSULE_BUTTON_SEAL2,
    BALL_CAPSULE_BUTTON_SEAL3,
    BALL_CAPSULE_BUTTON_SEAL4,
    BALL_CAPSULE_BUTTON_SEAL5,
    BALL_CAPSULE_BUTTON_SEAL6,
    BALL_CAPSULE_BUTTON_SEAL7,
    BALL_CAPSULE_BUTTON_SEAL8,
    BALL_CAPSULE_BUTTON_PREVPAGE,
    BALL_CAPSULE_BUTTON_NEXTPAGE,
    BALL_CAPSULE_BUTTON_CONFIRM,
    BALL_CAPSULE_BUTTON_DECIDE,
    BALL_CAPSULE_BUTTON_CANCEL,

    BALL_CAPSULE_BUTTON_MAX,
};

// Buttons for the movable seals on the ball
enum BallCapsuleSealButtons {

    BALL_CAPSULE_BUTTON_PLACED_SEAL1 = BALL_CAPSULE_BUTTON_MAX,
    BALL_CAPSULE_BUTTON_PLACED_SEAL2,
    BALL_CAPSULE_BUTTON_PLACED_SEAL3,
    BALL_CAPSULE_BUTTON_PLACED_SEAL4,
    BALL_CAPSULE_BUTTON_PLACED_SEAL5,
    BALL_CAPSULE_BUTTON_PLACED_SEAL6,
    BALL_CAPSULE_BUTTON_PLACED_SEAL7,
    BALL_CAPSULE_BUTTON_PLACED_SEAL8,

    BALL_CAPSULE_BUTTON_PLACED_SEAL_MAX,
};

enum BallCapsuleEditorWindows {
    BALL_CAPSULE_WINDOW_TOP_SCREEN_MAIN = 0,
    BALL_CAPSULE_WINDOW_TOP_SCREEN_SELECT,

    BALL_CAPSULE_WINDOW_BOTTOM_SCREEN_MAIN,

    BALL_CAPSULE_WINDOW_BOTTOM_SCREEN_SEAL_COUNT1,
    BALL_CAPSULE_WINDOW_BOTTOM_SCREEN_SEAL_COUNT2,
    BALL_CAPSULE_WINDOW_BOTTOM_SCREEN_SEAL_COUNT3,
    BALL_CAPSULE_WINDOW_BOTTOM_SCREEN_SEAL_COUNT4,
    BALL_CAPSULE_WINDOW_BOTTOM_SCREEN_SEAL_COUNT5,
    BALL_CAPSULE_WINDOW_BOTTOM_SCREEN_SEAL_COUNT6,
    BALL_CAPSULE_WINDOW_BOTTOM_SCREEN_SEAL_COUNT7,
    BALL_CAPSULE_WINDOW_BOTTOM_SCREEN_SEAL_COUNT8,

    BALL_CAPSULE_WINDOW_MAX,
};

typedef struct {
    BOOL tapped;
    u8 type;

    u8 x;
    u8 y;

    u8 padding_07;

    ManagedSprite *sealSprite;
    TouchScreenRect *rect;
} PlacedSeal;

typedef struct {
    ManagedSprite *sprites[BALL_CAPSULE_BUTTON_MAX];
} BallCapsuleButtonSprites;

typedef struct {
    int unk_00;
    BOOL touchScreenActive;
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    BgConfig *bgConfig;
    PaletteData *paletteData;
    Window windows[BALL_CAPSULE_WINDOW_MAX];
    StringList *unk_C8;
    Menu *menu;
    PokemonSpriteManager *monSpriteMan;
    PokemonSprite *monSprite;
    int yOffset;
    UnkStruct_ov22_02258A48 unk_DC;
    TouchScreenActions *touchScreenActions;
    TouchScreenRect buttonRects[BALL_CAPSULE_BUTTON_PLACED_SEAL_MAX];
    UnkStruct_02015920 *unk_150;
    UnkStruct_ov12_02235FE0 *unk_154;
    BallRotation *ballRotation;
    G3DPipelineBuffers *pipelineBuffers;
    UnkStruct_02012744 *unk_160;
    FontOAM *unk_164[2];
    CharTransferAllocation unk_16C[2];
    BOOL unk_184;
    PokemonAnimManager *animManager;
    BOOL unk_18C;
} BallCapsuleEditor;

typedef struct {
    int partyIndex;
    BallCapsule *ballCapsule;
} BallCapsuleData;

typedef struct {
    int partyIndex;
    int hasSeals;
    ManagedSprite *sprite;
} BallCapsuleEditData;

typedef struct {
    int currentPage;
    int totalPages;
    u8 currentPageSeals[SEALS_PER_PAGE];
} SealCasePages;

typedef struct {
    BallCapsuleAppData *appData;
    BallCapsuleData capsuleData[TOTAL_CAPSULES];
    SealCounts *sealCounts;
    BallCapsule selectedCapsule;
    u8 currentSealCounts[SEAL_ID_MAX];

    u8 padding_D1[3];

    BallCapsuleEditor ballCapsuleEditor;
    BallCapsuleEditData editData[TOTAL_CAPSULES];

    ManagedSprite *unk_2F4[2];
    ManagedSprite *partyIcons[MAX_PARTY_SIZE];
    ManagedSprite *unk_314[4];

    PlacedSeal placedSeals[SEALS_PER_CAPSULE];

    // unused
    int unk_3A4[8];

    // Second element is only used for "shift" feature
    int selectedCapsules[2];
    int unk_3CC;

    u8 padding_3D0[4];

    int state;
    int unk_3D8;
    int unk_3DC;
    int unk_3E0;
    BallCapsuleButtonSprites buttonSprites;
    SealCasePages sealCasePages;
    Pokemon *mon;
    NARC *narc;
} BallCapsuleSystem;

#endif // POKEPLATINUM_STRUCT_OV76_0223DE00_H
