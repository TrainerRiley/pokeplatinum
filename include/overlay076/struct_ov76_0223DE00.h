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
    BOOL unk_00;
    u8 unk_04;
    u8 unk_05;
    u8 unk_06;
    u8 padding_07;
    ManagedSprite *unk_08;
    TouchScreenRect *unk_0C;
} PlacedSeal;

typedef struct {
    ManagedSprite *unk_00[13];
} BallCapsuleEditorSprites;

typedef struct {
    int unk_00;
    BOOL unk_04;
    SpriteSystem *unk_08;
    SpriteManager *unk_0C;
    BgConfig *unk_10;
    PaletteData *unk_14;
    Window unk_18[11];
    StringList *unk_C8;
    Menu *unk_CC;
    PokemonSpriteManager *unk_D0;
    PokemonSprite *unk_D4;
    int unk_D8;
    UnkStruct_ov22_02258A48 unk_DC;
    TouchScreenActions *unk_F8;
    TouchScreenRect unk_FC[21];
    UnkStruct_02015920 *unk_150;
    UnkStruct_ov12_02235FE0 *unk_154;
    BallRotation *unk_158;
    G3DPipelineBuffers *unk_15C;
    UnkStruct_02012744 *unk_160;
    FontOAM *unk_164[2];
    CharTransferAllocation unk_16C[2];
    BOOL unk_184;
    PokemonAnimManager *unk_188;
    BOOL unk_18C;
} UnkStruct_ov76_0223C398;

typedef struct {
    int unk_00;
    BallCapsule *unk_04;
} BallCapsuleData;

typedef struct {
    int unk_00;
    int unk_04;
    ManagedSprite *unk_08;
} BallCapsuleEditData;

typedef struct {
    int unk_00;
    int unk_04;
    u8 unk_08[8];
} SealCasePages;

typedef struct {
    UnkStruct_02097F18 *unk_00;
    BallCapsuleData unk_04[TOTAL_CAPSULES];
    SealCounts *unk_64;
    BallCapsule unk_68;
    u8 unk_80[SEAL_ID_MAX];
    u8 padding_D1[3];

    UnkStruct_ov76_0223C398 unk_D4;

    BallCapsuleEditData unk_264[TOTAL_CAPSULES];

    ManagedSprite *unk_2F4[2];
    ManagedSprite *unk_2FC[MAX_PARTY_SIZE];
    ManagedSprite *unk_314[4];

    PlacedSeal unk_324[SEALS_PER_CAPSULE];
    int unk_3A4[SEALS_PER_CAPSULE];
    int unk_3C4[2];
    int unk_3CC;
    u8 padding_3D0[4];
    int unk_3D4;
    int unk_3D8;
    int unk_3DC;
    int unk_3E0;
    BallCapsuleEditorSprites unk_3E4;
    SealCasePages unk_418;
    Pokemon *unk_428;
    NARC *unk_42C;
} BallCapsuleSystem;

#endif // POKEPLATINUM_STRUCT_OV76_0223DE00_H
