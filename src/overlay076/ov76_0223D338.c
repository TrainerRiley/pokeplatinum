#include "overlay076/ov76_0223D338.h"

#include <nitro.h>
#include <string.h>

#include "constants/ball_capsule.h"
#include "generated/trainer_score_events.h"

#include "struct_decls/font_oam.h"
#include "struct_defs/seal_case.h"
#include "struct_defs/sprite_animation_frame.h"
#include "struct_defs/struct_02015958.h"

#include "battle_anim/ov12_02235E94.h"
#include "battle_anim/struct_ov12_02236030.h"
#include "battle_anim/struct_ov12_02237728.h"
#include "overlay076/const_ov76_0223EF3C.h"
#include "overlay076/ov76_0223B140.h"
#include "overlay076/ov76_0223B870.h"
#include "overlay076/struct_ov76_0223BF74.h"
#include "overlay076/struct_ov76_0223DE00.h"

#include "bg_window.h"
#include "game_records.h"
#include "gx_layers.h"
#include "heap.h"
#include "menu.h"
#include "narc.h"
#include "palette.h"
#include "pokemon.h"
#include "pokemon_anim.h"
#include "pokemon_sprite.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sprite_system.h"
#include "string_list.h"
#include "sys_task.h"
#include "sys_task_manager.h"
#include "system.h"
#include "touch_screen.h"
#include "touch_screen_actions.h"
#include "unk_02012744.h"
#include "unk_02015920.h"
#include "unk_0202C9F4.h"
#include "unk_02097B18.h"
#include "vram_transfer.h"

static BOOL BallCapsuleSystem_UpdateMainMenu(BallCapsuleSystem *ballCapsuleSys);
static BOOL ov76_0223DF94(BallCapsuleSystem *ballCapsuleSys);
static BOOL ov76_0223E8A4(BallCapsuleSystem *ballCapsuleSys);
static BOOL ov76_0223E950(BallCapsuleSystem *ballCapsuleSys);
static BOOL ov76_0223E9C4(BallCapsuleSystem *ballCapsuleSys);
static BOOL ov76_0223EB1C(BallCapsuleSystem *ballCapsuleSys);

void BallCapsuleSystem_SaveCapsuleToSelectedSlot(BallCapsuleSystem *ballCapsuleSys)
{
    BallCapsule_Copy(ballCapsuleSys->capsuleData[ballCapsuleSys->selectedCapsules[0]].ballCapsule, &ballCapsuleSys->selectedCapsule);
}

void BallCapsuleSystem_LoadSelectedCapsule(BallCapsuleSystem *ballCapsuleSys)
{
    BallCapsule_Copy(&ballCapsuleSys->selectedCapsule, ballCapsuleSys->capsuleData[ballCapsuleSys->selectedCapsules[0]].ballCapsule);
}

void BallCapsuleSystem_LoadSealCounts(BallCapsuleSystem *ballCapsuleSys)
{
    int v0;

    for (v0 = 0; v0 < (80 + 1); v0++) {
        ballCapsuleSys->currentSealCounts[v0] = SealCase_GetSealCount(ballCapsuleSys->sealCounts, v0);
    }
}

void BallCapsuleSystem_SetSealCounts(BallCapsuleSystem *ballCapsuleSys)
{
    int i;

    for (i = 0; i < (80 + 1); i++) {
        SealCase_SetSealQuantity(ballCapsuleSys->sealCounts, i, ballCapsuleSys->currentSealCounts[i]);
    }
}

void BallCapsuleSystem_SetDefaultBlend(void)
{
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_NONE, GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2, 11, 7);
    G2S_SetBlendAlpha(GX_BLEND_PLANEMASK_NONE, GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3, 7, 8);
}

void BallCapsuleSystem_LoadFirstPokemon(BallCapsuleSystem *ballCapsuleSys)
{
    int v0;
    int v1;
    Pokemon *v2;

    if (ballCapsuleSys->capsuleData[ballCapsuleSys->selectedCapsules[0]].partyIndex != BALL_CAPSULE_INVALID_PARTY_INDEX) {
        v2 = ballCapsuleSys->appData->mons[ballCapsuleSys->capsuleData[ballCapsuleSys->selectedCapsules[0]].partyIndex];
        Pokemon_Copy(v2, ballCapsuleSys->mon);

        return;
    }

    for (v0 = 0; v0 < ballCapsuleSys->appData->partyCount; v0++) {
        v2 = ballCapsuleSys->appData->mons[v0];
        v1 = Pokemon_GetValue(v2, MON_DATA_IS_EGG, NULL);

        if (v1 != 0) {
            continue;
        }

        Pokemon_Copy(v2, ballCapsuleSys->mon);
        break;
    }
}

int BallCapsule_ContainsSeals(BallCapsule *param0)
{
    int v0;
    int v1 = 0;
    int v2;
    BallSeal *v3;

    v2 = 0;

    for (v0 = 0; v0 < 8; v0++) {
        v3 = BallCapsule_GetBallSeals(param0, v0);

        if (BallSeal_GetSealType(v3) != 0) {
            v2++;
        }
    }

    if (v2 != 0) {
        v1 = 1;
    }

    return v1;
}

int ov76_0223D45C(BallCapsuleSystem *ballCapsuleSys, int param1)
{
    int v0;
    int v1;
    int v2;

    v0 = ballCapsuleSys->editData[param1].partyIndex;
    v1 = ballCapsuleSys->editData[param1].hasSeals;

    if ((v0 == BALL_CAPSULE_INVALID_PARTY_INDEX) && (v1 == 0)) {
        v2 = 0;
    } else if ((v0 == BALL_CAPSULE_INVALID_PARTY_INDEX) && (v1 == 1)) {
        v2 = 1;
    } else if ((v0 != BALL_CAPSULE_INVALID_PARTY_INDEX) && (v1 == 1)) {
        v2 = 2;
    } else {
        v2 = 3;
    }

    return v2;
}

static void ov76_0223D494(BallCapsuleSystem *ballCapsuleSys, int param1, int param2, int param3)
{
    if (param2 == 0xff) {
        ballCapsuleSys->state = ballCapsuleSys->unk_3D8;
    } else {
        ballCapsuleSys->state = param2;
    }

    ballCapsuleSys->unk_3CC = param1;
    ballCapsuleSys->unk_3D8 = param3;
    ballCapsuleSys->unk_3DC = 0;
}

static int ov76_0223D4C4(BallCapsuleSystem *ballCapsuleSys)
{
    Window_EraseStandardFrame(&ballCapsuleSys->ballCapsuleEditor.windows[1], 1);
    Window_ClearAndCopyToVRAM(&ballCapsuleSys->ballCapsuleEditor.windows[1]);
    Menu_Free(ballCapsuleSys->ballCapsuleEditor.menu, NULL);
    StringList_Free(ballCapsuleSys->ballCapsuleEditor.unk_C8);
    Window_Remove(&ballCapsuleSys->ballCapsuleEditor.windows[1]);

    return 1;
}

static int ov76_0223D4FC(BallCapsuleSystem *ballCapsuleSys)
{
    ov76_0223D494(ballCapsuleSys, 1, 0, 3);
    return 0;
}

static int ov76_0223D50C(BallCapsuleSystem *ballCapsuleSys)
{
    ov76_0223B400(ballCapsuleSys);
    ov76_0223B808(ballCapsuleSys);
    BallCapsuleSystem_UpdateCapsuleData(ballCapsuleSys);
    ov76_0223D494(ballCapsuleSys, 2, 0, 0);

    return 0;
}

static int ov76_0223D530(BallCapsuleSystem *ballCapsuleSys)
{
    ov76_0223D494(ballCapsuleSys, 3, 0, 3);
    return 0;
}

static int ov76_0223D540(BallCapsuleSystem *ballCapsuleSys)
{
    ov76_0223D494(ballCapsuleSys, 4, 0, 3);
    return 0;
}

const UnkStruct_ov76_0223BF74 Unk_ov76_0223EF3C[] = {
    { 0, (const u32)ov76_0223D4FC },
    { 1, (const u32)ov76_0223D4C4 },
    { 2, (const u32)ov76_0223D50C },
    { 3, (const u32)ov76_0223D530 },
    { 4, (const u32)ov76_0223D540 },
};

static BOOL (*const Unk_ov76_0223EE04[])(BallCapsuleSystem *cbmw) = {
    BallCapsuleSystem_UpdateMainMenu,
    ov76_0223DF94,
    ov76_0223E8A4,
    ov76_0223E950,
    ov76_0223E9C4,
    ov76_0223EB1C,
};

BOOL ov76_0223D550(BallCapsuleSystem *ballCapsuleSys)
{
    BOOL v0 = Unk_ov76_0223EE04[ballCapsuleSys->unk_3CC](ballCapsuleSys);
    SpriteSystem_DrawSprites(ballCapsuleSys->ballCapsuleEditor.spriteMan);

    return v0;
}

// Returns whether the cursor moved in this call
static BOOL BallCapsuleSystem_MoveCursor(int *index)
{
    // Right and left wrap and move to the next/prev row
    if (gSystem.pressedKeysRepeatable & PAD_KEY_RIGHT) {
        (*index)++;
        (*index) %= TOTAL_CAPSULES;
    } else if (gSystem.pressedKeysRepeatable & PAD_KEY_LEFT) {
        if (*index > 0) {
            (*index)--;
        } else {
            *index = TOTAL_CAPSULES - 1;
        }
        // Up and down do not wrap at all
    } else if (gSystem.pressedKeysRepeatable & PAD_KEY_UP) {
        if ((*index / CAPSULES_PER_ROW) != 0) {
            (*index) -= CAPSULES_PER_ROW;
            (*index) %= TOTAL_CAPSULES;
        } else {
            return FALSE;
        }
    } else if (gSystem.pressedKeysRepeatable & PAD_KEY_DOWN) {
        if ((*index / CAPSULES_PER_ROW) != (CAPSULES_PER_COLUMN - 1)) {
            (*index) += CAPSULES_PER_ROW;
            (*index) %= TOTAL_CAPSULES;
        } else {
            return FALSE;
        }
    } else {
        return FALSE;
    }

    return TRUE;
}

static void ov76_0223D600(BallCapsuleSystem *ballCapsuleSys, int param1, BOOL param2)
{
    s16 v0, v1;
    int v2 = 0;

    if (param1 == 0) {
        v2 = 1;
    }

    GetCapsulePosition(ballCapsuleSys->selectedCapsules[param1], &v0, &v1);
    ManagedSprite_SetPositionXY(ballCapsuleSys->unk_2F4[param1], v0, v1);

    if (param2 == 1) {
        ballCapsuleSys->selectedCapsules[v2] = ballCapsuleSys->selectedCapsules[param1];
        GetCapsulePosition(ballCapsuleSys->selectedCapsules[v2], &v0, &v1);
        ManagedSprite_SetPositionXY(ballCapsuleSys->unk_2F4[v2], v0, v1);
    }
}

enum BallCapsuleSystemState {
    BALL_CAPSULE_SYSTEM_INIT = 0,
    BALL_CAPSULE_SYSTEM_FADE_IN,
    BALL_CAPSULE_SYSTEM_WAIT_FOR_FADE_IN,
    BALL_CAPSULE_SYSTEM_MAIN,
    BALL_CAPSULE_SYSTEM_EDIT_MENU,
    BALL_CAPSULE_SYSTEM_SHUTDOWN,
    BALL_CAPSULE_SYSTEM_EXIT,
    BALL_CAPSULE_SYSTEM_EXIT_EDITOR,
    BALL_CAPSULE_SYSTEM_8,
};

typedef int (*BallCapsuleMenuFunc)(BallCapsuleSystem *);

static BOOL BallCapsuleSystem_UpdateMainMenu(BallCapsuleSystem *ballCapsuleSys)
{
    switch (ballCapsuleSys->state) {
    case BALL_CAPSULE_SYSTEM_INIT: {
        NARC *narc;

        narc = NARC_ctor(NARC_INDEX_APPLICATION__CUSTOM_BALL__DATA__CB_DATA, HEAP_ID_BALL_CAPSULE_SYSTEM);

        ov76_0223C110(ballCapsuleSys);
        BallCapsuleSystem_LoadPartyIcons(ballCapsuleSys);
        BallCapsuleSystem_UpdatePartyIconPositions(ballCapsuleSys);
        ov76_0223CE84(ballCapsuleSys, narc);
        ov76_0223CF24(ballCapsuleSys, narc);
        ov76_0223CF88(ballCapsuleSys, narc);
        BallCapsuleSystem_UpdateEditData(ballCapsuleSys);
        ov76_0223C61C(ballCapsuleSys, narc);
        CreateWindowWithScroll(ballCapsuleSys->ballCapsuleEditor.bgConfig, &ballCapsuleSys->ballCapsuleEditor.windows[0], 1, 2, 21, 27, 2, 0 + ((1 + (18 + 12)) + 9));
        ov76_0223B208(ballCapsuleSys);
        ov76_0223B69C(ballCapsuleSys, 1);
        ov76_0223B1E0(ballCapsuleSys);
        ov76_0223CFEC(ballCapsuleSys, narc);
        BallCapsuleSys_CreateStaticButtons(ballCapsuleSys);
        ov76_0223C438(ballCapsuleSys, narc);
        ov76_0223C4AC(ballCapsuleSys);
        BallCapsuleSystem_SaveCapsuleToSelectedSlot(ballCapsuleSys);
        BallCapsuleSystem_LoadSealCounts(ballCapsuleSys);
        BallCapsuleSystem_LoadCurrentPageSeals(ballCapsuleSys, ballCapsuleSys->sealCasePages.currentPage);
        BallCapsuleSystem_LoadCurrentPageData(ballCapsuleSys);
        BallCapsuleSystem_CreatePageSprites(ballCapsuleSys);
        BallCapsuleSystem_SetButtonDrawFlags(ballCapsuleSys, 0);
        ov76_0223C568(ballCapsuleSys, 0);
        ov76_0223B940(ballCapsuleSys);
        ov76_0223B96C(ballCapsuleSys, 0);
        Window_SetMessage(&ballCapsuleSys->ballCapsuleEditor.windows[0], 7);
        ov76_0223CE2C();
        BallCapsuleSystem_SetTouchScreenActive(ballCapsuleSys, 0);

        NARC_dtor(narc);
    }
        ballCapsuleSys->state++;
        break;

    case BALL_CAPSULE_SYSTEM_FADE_IN:
        BallCapsuleSystem_ScreenFadeIn();
        ballCapsuleSys->state++;
        break;

    case BALL_CAPSULE_SYSTEM_WAIT_FOR_FADE_IN:
        if (IsScreenFadeDone() != 1) {
            break;
        }

        ballCapsuleSys->state++;

    case BALL_CAPSULE_SYSTEM_MAIN: {
        BOOL cursorMoved;
        int prevCursorIndex;
        int currCursorIndex;

        if (TouchScreen_Tapped() == TRUE) {
            ov76_0223D4FC(ballCapsuleSys);
            break;
        }

        prevCursorIndex = ballCapsuleSys->selectedCapsules[0];
        cursorMoved = BallCapsuleSystem_MoveCursor(&(ballCapsuleSys->selectedCapsules[0]));
        currCursorIndex = ballCapsuleSys->selectedCapsules[0];
        ballCapsuleSys->selectedCapsules[0] = prevCursorIndex;

        if (cursorMoved == TRUE) {
            ov76_0223B400(ballCapsuleSys);
            ov76_0223B808(ballCapsuleSys);
            BallCapsuleSystem_UpdateCapsuleData(ballCapsuleSys);

            ballCapsuleSys->selectedCapsules[0] = currCursorIndex;

            ov76_0223D600(ballCapsuleSys, 0, 1);
            ov76_0223B678(ballCapsuleSys);
            ov76_0223B208(ballCapsuleSys);
            ov76_0223B69C(ballCapsuleSys, 1);
            ov76_0223B1E0(ballCapsuleSys);

            Sound_PlayEffect(SEQ_SE_CONFIRM);
        } else if (gSystem.pressedKeys & PAD_BUTTON_A) {
            ballCapsuleSys->state++;
            Window_SetMessage(&ballCapsuleSys->ballCapsuleEditor.windows[0], 8);
            ov76_0223BF74(ballCapsuleSys->ballCapsuleEditor.bgConfig, &ballCapsuleSys->ballCapsuleEditor.windows[1], 1, ballCapsuleSys, ballCapsuleSys->selectedCapsules[0]);
            Sound_PlayEffect(SEQ_SE_CONFIRM);
        } else if (gSystem.pressedKeys & PAD_BUTTON_B) {
            ballCapsuleSys->state = BALL_CAPSULE_SYSTEM_SHUTDOWN;
            Sound_PlayEffect(SEQ_SE_DP_DECIDE);
        }
    } break;
    case BALL_CAPSULE_SYSTEM_EDIT_MENU: {

        u32 result = Menu_ProcessInput(ballCapsuleSys->ballCapsuleEditor.menu);

        switch (result) {
        case 0xfffffffe:
            ov76_0223D4C4(ballCapsuleSys);
            Window_SetMessage(&ballCapsuleSys->ballCapsuleEditor.windows[0], 7);
            ballCapsuleSys->state = BALL_CAPSULE_SYSTEM_MAIN;
            break;
        case 0xffffffff:
            break;
        default:
            BallCapsuleMenuFunc menuFunc = (BallCapsuleMenuFunc)result;

            if (menuFunc != NULL) {
                int v6 = menuFunc(ballCapsuleSys);

                if (v6 != 1) {
                    ov76_0223D4C4(ballCapsuleSys);
                    break;
                }

                Window_SetMessage(&ballCapsuleSys->ballCapsuleEditor.windows[0], 7);
                ballCapsuleSys->state = BALL_CAPSULE_SYSTEM_MAIN;
            }
            break;
        }
    } break;
    case BALL_CAPSULE_SYSTEM_SHUTDOWN:
        BallCapsuleSystem_ScreeFadeOut();
        ballCapsuleSys->state++;
        break;
    case BALL_CAPSULE_SYSTEM_EXIT:
        if (IsScreenFadeDone() != 1) {
            break;
        }
        Window_Remove(&ballCapsuleSys->ballCapsuleEditor.windows[0]);
        BallCapsuleSystem_DeleteSprites(ballCapsuleSys);
        BallCapsuleSystem_DeletePartyIcons(ballCapsuleSys);
        BallCapsuleSystem_DeleteStaticButtons(ballCapsuleSys);
        ov76_0223C588(ballCapsuleSys);
        BallCapsuleSystem_UnloadEditorSprites(ballCapsuleSys);
        sub_02097F30(ballCapsuleSys->appData, 0);
        return FALSE;
    }

    BallCapsuleSystem_TickPartyIcons(ballCapsuleSys);
    BallCapsuleSystem_TickSprites(ballCapsuleSys);

    return TRUE;
}

void BallCapsuleSystem_SetButtonSpriteState(ManagedSprite *sprite, enum TouchScreenButtonState screenState)
{
    switch (screenState) {
    case TOUCH_BUTTON_PRESSED:
        ManagedSprite_SetAnimationFrame(sprite, 1);
        break;
    case TOUCH_BUTTON_HELD:
        ManagedSprite_SetAnimationFrame(sprite, 2);
        break;
    case TOUCH_BUTTON_RELEASED:
    case TOUCH_BUTTON_HELD_OUT_OF_BOUNDS:
        ManagedSprite_SetAnimationFrame(sprite, 0);
        break;
    default:
        GF_ASSERT(0);
        break;
    }
}

static void FontOAM_MovePosition(FontOAM *fontOAM, int xOffset, int yOffset)
{
    if (fontOAM != NULL) {
        int x;
        int y;
        FontOAM_GetXY(fontOAM, &x, &y);
        FontOAM_SetXY(fontOAM, x + xOffset, y + yOffset);
    }
}

typedef struct {
    int timer;
    ManagedSprite *sprite;
    FontOAM *fontOAM;
} ButtonAnimation;

static void ButtonAnimatinSysTask(SysTask *sysTask, void *buttonAnimation)
{
    ButtonAnimation *anim = buttonAnimation;

    switch (anim->timer) {
    case 3:
        FontOAM_MovePosition(anim->fontOAM, 0, -1);
        ManagedSprite_SetAnimationFrame(anim->sprite, 2);
        anim->timer++;
        break;
    case 6:
        FontOAM_MovePosition(anim->fontOAM, 0, +2);
        ManagedSprite_SetAnimationFrame(anim->sprite, 0);
        SysTask_Done(sysTask);
        Heap_Free(anim);
        break;
    default:
        anim->timer++;
        break;
    }
}

static void ButtonAnimation_Init(ManagedSprite *sprite, FontOAM *fontOAM)
{
    ButtonAnimation *anim = Heap_Alloc(HEAP_ID_BALL_CAPSULE_SYSTEM, sizeof(ButtonAnimation));

    anim->timer = 1;
    anim->sprite = sprite;
    anim->fontOAM = fontOAM;

    FontOAM_MovePosition(anim->fontOAM, 0, -1);
    SysTask_Start(ButtonAnimatinSysTask, anim, 1000);
}

void ov76_0223DA34(u32 buttonID, enum TouchScreenButtonState touchScreenState, void *data)
{
    BallCapsuleSystem *ballCapsuleSys = (BallCapsuleSystem *)data;

    if (ballCapsuleSys->ballCapsuleEditor.touchScreenActive == FALSE) {
        return;
    }

    switch (buttonID) {
    case BALL_CAPSULE_BUTTON_PREVPAGE:
        if (touchScreenState == TOUCH_BUTTON_PRESSED) {
            if (ballCapsuleSys->sealCasePages.currentPage > 0) {
                ballCapsuleSys->sealCasePages.currentPage--;
            } else {
                ballCapsuleSys->sealCasePages.currentPage = ballCapsuleSys->sealCasePages.totalPages - 1;
            }

            BallCapsuleSystem_UnloadEditorSprites(ballCapsuleSys);
            BallCapsuleSystem_LoadCurrentPageSeals(ballCapsuleSys, ballCapsuleSys->sealCasePages.currentPage);
            BallCapsuleSystem_LoadCurrentPageData(ballCapsuleSys);
            BallCapsuleSystem_CreatePageSprites(ballCapsuleSys);
            BallCapsuleSystem_PrintSealCountsToWindows(ballCapsuleSys);
            Sound_PlayEffect(SEQ_SE_DP_CUSTOM02);
        }

        BallCapsuleSystem_SetButtonSpriteState(ballCapsuleSys->buttonSprites.sprites[8], touchScreenState);
        break;
    case BALL_CAPSULE_BUTTON_NEXTPAGE:
        if (touchScreenState == TOUCH_BUTTON_PRESSED) {
            ballCapsuleSys->sealCasePages.currentPage++;
            ballCapsuleSys->sealCasePages.currentPage %= ballCapsuleSys->sealCasePages.totalPages;

            BallCapsuleSystem_UnloadEditorSprites(ballCapsuleSys);
            BallCapsuleSystem_LoadCurrentPageSeals(ballCapsuleSys, ballCapsuleSys->sealCasePages.currentPage);
            BallCapsuleSystem_LoadCurrentPageData(ballCapsuleSys);
            BallCapsuleSystem_CreatePageSprites(ballCapsuleSys);
            BallCapsuleSystem_PrintSealCountsToWindows(ballCapsuleSys);
            Sound_PlayEffect(SEQ_SE_DP_CUSTOM02);
        }
        BallCapsuleSystem_SetButtonSpriteState(ballCapsuleSys->buttonSprites.sprites[9], touchScreenState);
        break;
    case BALL_CAPSULE_BUTTON_CONFIRM:
        if (touchScreenState == TOUCH_BUTTON_PRESSED) {
            if (ballCapsuleSys->state != BALL_CAPSULE_SYSTEM_SHUTDOWN) {
                ballCapsuleSys->state = BALL_CAPSULE_SYSTEM_SHUTDOWN;
                Sound_PlayEffect(SEQ_SE_DP_DECIDE);
            }

            ButtonAnimation_Init(ballCapsuleSys->buttonSprites.sprites[10], NULL);
        }

        BallCapsuleSystem_SetButtonSpriteState(ballCapsuleSys->buttonSprites.sprites[10], touchScreenState);
        break;
    case BALL_CAPSULE_BUTTON_DECIDE:
        if (touchScreenState == TOUCH_BUTTON_PRESSED) {
            if (ballCapsuleSys->state != BALL_CAPSULE_SYSTEM_EXIT) {
                ballCapsuleSys->state = BALL_CAPSULE_SYSTEM_EXIT;
                BallCapsuleSystem_SetTouchScreenActive(ballCapsuleSys, 0);
                Sound_PlayEffect(SEQ_SE_DP_PIRORIRO);
            }

            ButtonAnimation_Init(ballCapsuleSys->buttonSprites.sprites[11], ballCapsuleSys->ballCapsuleEditor.unk_164[0]);
        }

        BallCapsuleSystem_SetButtonSpriteState(ballCapsuleSys->buttonSprites.sprites[11], touchScreenState);
        break;
    case BALL_CAPSULE_BUTTON_CANCEL:
        if (touchScreenState == TOUCH_BUTTON_PRESSED) {
            if (ballCapsuleSys->state != BALL_CAPSULE_SYSTEM_EXIT_EDITOR) {
                ballCapsuleSys->state = BALL_CAPSULE_SYSTEM_EXIT_EDITOR;
                BallCapsuleSystem_SetTouchScreenActive(ballCapsuleSys, 0);
                Sound_PlayEffect(SEQ_SE_DP_DECIDE);
            }

            ButtonAnimation_Init(ballCapsuleSys->buttonSprites.sprites[12], ballCapsuleSys->ballCapsuleEditor.unk_164[1]);
        }

        BallCapsuleSystem_SetButtonSpriteState(ballCapsuleSys->buttonSprites.sprites[12], touchScreenState);
        break;
    case BALL_CAPSULE_BUTTON_SEAL1:
    case BALL_CAPSULE_BUTTON_SEAL2:
    case BALL_CAPSULE_BUTTON_SEAL3:
    case BALL_CAPSULE_BUTTON_SEAL4:
    case BALL_CAPSULE_BUTTON_SEAL5:
    case BALL_CAPSULE_BUTTON_SEAL6:
    case BALL_CAPSULE_BUTTON_SEAL7:
    case BALL_CAPSULE_BUTTON_SEAL8: {
        int v1;

        if (touchScreenState == TOUCH_BUTTON_PRESSED) {
            if (ov76_0223B2F8(ballCapsuleSys) == 0) {
                Sound_PlayEffect(SEQ_SE_DP_CUSTOM06);
                Window_SetMessage(&ballCapsuleSys->ballCapsuleEditor.windows[0], 15);
            } else {
                if ((ballCapsuleSys->sealCasePages.currentPageSeals[buttonID] != 0) && (SealCase_GetSealCount(ballCapsuleSys->sealCounts, ballCapsuleSys->sealCasePages.currentPageSeals[buttonID] - 1) != 0)) {
                    ballCapsuleSys->ballCapsuleEditor.unk_00 = ov76_0223B278(ballCapsuleSys, buttonID);
                    v1 = SealData_GetNameID(ballCapsuleSys->sealCasePages.currentPageSeals[buttonID]);

                    Window_SetSealNameMessage(&ballCapsuleSys->ballCapsuleEditor.windows[0], v1);
                    GiveOrTakeSeal(ballCapsuleSys->appData->sealCase, ballCapsuleSys->sealCasePages.currentPageSeals[buttonID], -1);
                    BallCapsuleSys_UpdateWindowSealCount(ballCapsuleSys, buttonID);
                    Sound_PlayEffect(SEQ_SE_DP_BOX02);
                } else {
                    if (ballCapsuleSys->sealCasePages.currentPageSeals[buttonID] != 0) {
                        Sound_PlayEffect(SEQ_SE_DP_CUSTOM06);
                        Window_SetMessage(&ballCapsuleSys->ballCapsuleEditor.windows[0], 16);
                    }
                }
            }
        }
    } break;
    case BALL_CAPSULE_BUTTON_PLACED_SEAL1:
    case BALL_CAPSULE_BUTTON_PLACED_SEAL2:
    case BALL_CAPSULE_BUTTON_PLACED_SEAL3:
    case BALL_CAPSULE_BUTTON_PLACED_SEAL4:
    case BALL_CAPSULE_BUTTON_PLACED_SEAL5:
    case BALL_CAPSULE_BUTTON_PLACED_SEAL6:
    case BALL_CAPSULE_BUTTON_PLACED_SEAL7:
    case BALL_CAPSULE_BUTTON_PLACED_SEAL8: {
        int v2;
        int v3;

        if (touchScreenState == TOUCH_BUTTON_PRESSED) {
            v2 = buttonID - BALL_CAPSULE_BUTTON_MAX;
            ov76_0223B5C4(ballCapsuleSys, touchScreenState, v2);
            v3 = SealData_GetNameID(ballCapsuleSys->placedSeals[v2].type);

            Window_SetSealNameMessage(&ballCapsuleSys->ballCapsuleEditor.windows[0], v3);
            Sound_PlayEffect(SEQ_SE_DP_BOX02);
        }
    } break;
    default:
        GF_ASSERT(0);
        break;
    }
}

BOOL ov76_0223DCB0(BallCapsuleSystem *ballCapsuleSys)
{
    return ballCapsuleSys->ballCapsuleEditor.unk_184;
}

void BallCapsuleSystem_SetTouchScreenActive(BallCapsuleSystem *ballCapsuleSys, BOOL active)
{
    ballCapsuleSys->ballCapsuleEditor.touchScreenActive = active;
}

static const TouchScreenRect staticButtonRects[] = {
    { 16, 32, 8, 24 },
    { 16, 32, 64, 80 },
    { 40, 56, 8, 24 },
    { 40, 56, 64, 80 },
    { 64, 80, 8, 24 },
    { 64, 80, 64, 80 },
    { 88, 104, 8, 24 },
    { 88, 104, 64, 80 },
    { 104, 128, 8, 48 },
    { 104, 128, 56, 96 },
    { 168, 188, 18, 77 },
    { 168, 188, 97, 156 },
    { 168, 188, 178, 237 },
};

void ov76_0223DCC0(BallCapsuleSystem *ballCapsuleSys)
{
    int i;
    const TouchScreenRect defaultTable = { 0, 0, 0, 0 };

    for (i = 0; i < BALL_CAPSULE_BUTTON_MAX; i++) {
        ballCapsuleSys->ballCapsuleEditor.buttonRects[i] = staticButtonRects[i];
    }

    for (; i < BALL_CAPSULE_BUTTON_PLACED_SEAL_MAX; i++) {
        ballCapsuleSys->ballCapsuleEditor.buttonRects[i] = defaultTable;
        ballCapsuleSys->placedSeals[i - BALL_CAPSULE_BUTTON_PLACED_SEAL1].rect = &ballCapsuleSys->ballCapsuleEditor.buttonRects[i];
    }

    ballCapsuleSys->ballCapsuleEditor.touchScreenActions = TouchScreenActions_RegisterHandler(ballCapsuleSys->ballCapsuleEditor.buttonRects, BALL_CAPSULE_BUTTON_PLACED_SEAL_MAX, ov76_0223DA34, ballCapsuleSys, HEAP_ID_BALL_CAPSULE_SYSTEM);
}

void BallCapsuleSystem_InitializeButtons(BallCapsuleSystem *ballCapsuleSys)
{
    PokemonSpriteTemplate spriteTemplate;
    SpriteAnimFrame animFrame[10];

    Pokemon_BuildSpriteTemplate(&spriteTemplate, ballCapsuleSys->mon, FACE_FRONT);

    int species = Pokemon_GetValue(ballCapsuleSys->mon, MON_DATA_SPECIES, NULL);
    int yOffset = Pokemon_SpriteYOffset(ballCapsuleSys->mon, FACE_FRONT);

    ballCapsuleSys->ballCapsuleEditor.yOffset = yOffset;
    PokemonSprite_LoadAnimFrames(ballCapsuleSys->narc, &animFrame[0], species, 1);
    ballCapsuleSys->ballCapsuleEditor.monSprite = PokemonSpriteManager_CreateSprite(ballCapsuleSys->ballCapsuleEditor.monSpriteMan, &spriteTemplate, 256 - 64, 48 + yOffset, -0x280, 0, &animFrame[0], NULL);
}

static void BallCapsuleSystem_LoadPokemonSprite(BallCapsuleSystem *ballCapsuleSys)
{
    int species = Pokemon_GetValue(ballCapsuleSys->mon, MON_DATA_SPECIES, NULL);
    int nature = Pokemon_GetNature(ballCapsuleSys->mon);

    PokemonSprite_InitAnim(ballCapsuleSys->ballCapsuleEditor.monSprite, 1);
    PokemonSprite_LoadAnim(ballCapsuleSys->narc, ballCapsuleSys->ballCapsuleEditor.animManager, ballCapsuleSys->ballCapsuleEditor.monSprite, species, 2, 0, 0);
}

static void ov76_0223DE54(BallCapsuleSystem *ballCapsuleSys)
{
    PokemonSprite_SetAttribute(ballCapsuleSys->ballCapsuleEditor.monSprite, MON_SPRITE_SCALE_X, 0x0);
    PokemonSprite_SetAttribute(ballCapsuleSys->ballCapsuleEditor.monSprite, MON_SPRITE_SCALE_Y, 0x0);
}

static BOOL ov76_0223DE78(BallCapsuleSystem *ballCapsuleSys)
{
    if (PokemonSprite_GetAttribute(ballCapsuleSys->ballCapsuleEditor.monSprite, MON_SPRITE_SCALE_X) == 0x100) {
        return FALSE;
    } else if (PokemonSprite_GetAttribute(ballCapsuleSys->ballCapsuleEditor.monSprite, MON_SPRITE_SCALE_X) >= 0x100) {
        PokemonSprite_SetAttribute(ballCapsuleSys->ballCapsuleEditor.monSprite, MON_SPRITE_SCALE_X, 0x100);
        PokemonSprite_SetAttribute(ballCapsuleSys->ballCapsuleEditor.monSprite, MON_SPRITE_SCALE_Y, 0x100);
        return FALSE;
    } else {
        PokemonSprite_AddAttribute(ballCapsuleSys->ballCapsuleEditor.monSprite, MON_SPRITE_SCALE_X, 0x20);
        PokemonSprite_AddAttribute(ballCapsuleSys->ballCapsuleEditor.monSprite, MON_SPRITE_SCALE_Y, 0x20);
        PokemonSprite_CalcScaledYOffset(ballCapsuleSys->ballCapsuleEditor.monSprite, ballCapsuleSys->ballCapsuleEditor.yOffset);
    }

    return TRUE;
}

static BOOL ov76_0223DEF4(BallCapsuleSystem *ballCapsuleSys)
{
    if (PokemonSprite_GetAttribute(ballCapsuleSys->ballCapsuleEditor.monSprite, MON_SPRITE_SCALE_X) == 0x0) {
        return FALSE;
    } else if (PokemonSprite_GetAttribute(ballCapsuleSys->ballCapsuleEditor.monSprite, MON_SPRITE_SCALE_X) <= 0x0) {
        PokemonSprite_SetAttribute(ballCapsuleSys->ballCapsuleEditor.monSprite, MON_SPRITE_SCALE_X, 0x0);
        PokemonSprite_SetAttribute(ballCapsuleSys->ballCapsuleEditor.monSprite, MON_SPRITE_SCALE_Y, 0x0);
        return FALSE;
    } else {
        PokemonSprite_AddAttribute(ballCapsuleSys->ballCapsuleEditor.monSprite, MON_SPRITE_SCALE_X, -0x20);
        PokemonSprite_AddAttribute(ballCapsuleSys->ballCapsuleEditor.monSprite, MON_SPRITE_SCALE_Y, -0x20);
        PokemonSprite_CalcScaledYOffset(ballCapsuleSys->ballCapsuleEditor.monSprite, ballCapsuleSys->ballCapsuleEditor.yOffset);
    }

    return TRUE;
}

void BallCapsuleSystem_LoadPokemonAnim(BallCapsuleSystem *ballCapsuleSys, int param1)
{
    PokemonSprite_SetAttribute(ballCapsuleSys->ballCapsuleEditor.monSprite, MON_SPRITE_HIDE, param1);
}

void BallCapsuleSystem_DeletePokemonSprite(BallCapsuleSystem *ballCapsuleSys)
{
    PokemonSprite_Delete(ballCapsuleSys->ballCapsuleEditor.monSprite);
}

static BOOL ov76_0223DF94(BallCapsuleSystem *ballCapsuleSys)
{
    switch (ballCapsuleSys->state) {
    case 0:
        BallCapsuleSystem_SetTouchScreenActive(ballCapsuleSys, 0);
        Window_SetMessage(&ballCapsuleSys->ballCapsuleEditor.windows[0], 0xFFFF);
        BallCapsuleSystem_CreateSealCountWindows(ballCapsuleSys);
        BallCapsuleSystem_PrintSealCountsToWindows(ballCapsuleSys);
        ov76_0223BD30(ballCapsuleSys, +1, 4);
        BallCapsuleSystem_SaveCapsuleToSelectedSlot(ballCapsuleSys);
        BallCapsuleSystem_LoadSealCounts(ballCapsuleSys);
        ballCapsuleSys->ballCapsuleEditor.unk_18C = 0;
        ballCapsuleSys->state++;
        break;
    case 1:
        if (ov76_0223DCB0(ballCapsuleSys) == TRUE) {
            break;
        }

        PaletteData_StartFade(ballCapsuleSys->ballCapsuleEditor.paletteData, 0x1, (1 << 0) | (1 << 1), 0, 0, 16, 0);
        PaletteData_StartFade(ballCapsuleSys->ballCapsuleEditor.paletteData, 0x4, 0xFFFF, 0, 0, 16, 0);
        GXLayers_EngineAToggleLayers(GX_PLANEMASK_OBJ, 0);
        GXLayers_EngineBToggleLayers(GX_PLANEMASK_BG2, 1);
        Bg_SetPriority(BG_LAYER_SUB_3, 1);
        BallCapsuleSystem_SetButtonDrawFlags(ballCapsuleSys, 1);
        ov76_0223C568(ballCapsuleSys, 1);
        ov76_0223B96C(ballCapsuleSys, 1);
        ballCapsuleSys->state++;
        break;
    case 2:
        if (PaletteData_GetSelectedBuffersMask(ballCapsuleSys->ballCapsuleEditor.paletteData) != 0) {
            break;
        }
        Bg_SetPriority(BG_LAYER_MAIN_3, 1);
        PaletteData_StartFade(ballCapsuleSys->ballCapsuleEditor.paletteData, 0x1, 1 << 1, 0, 16, 0, 0);
        ballCapsuleSys->state++;
        break;
    case 3:
        if (PaletteData_GetSelectedBuffersMask(ballCapsuleSys->ballCapsuleEditor.paletteData) != 0) {
            break;
        }
        BallCapsuleSystem_SetTouchScreenActive(ballCapsuleSys, 1);
        ballCapsuleSys->state++;
        break;
    case 4:
        break;
    case 5: {
        switch (ballCapsuleSys->unk_3DC) {
        case 0:
            GXLayers_EngineAToggleLayers(GX_PLANEMASK_OBJ, 1);
            PaletteData_StartFade(ballCapsuleSys->ballCapsuleEditor.paletteData, 0x2, (1 << 0) | (1 << 1) | (1 << 3) | (1 << 11), 0, 0, 10, 0);
            PaletteData_StartFade(ballCapsuleSys->ballCapsuleEditor.paletteData, 0x8, 0xFFFF, 0, 0, 10, 0);
            BallCapsuleSystem_SetTouchScreenActive(ballCapsuleSys, 0);
            ballCapsuleSys->unk_3E0 = 0;
            BallCapsuleSystem_LoadFirstPokemon(ballCapsuleSys);
            BallCapsuleSystem_InitializeButtons(ballCapsuleSys);
            BallCapsuleSystem_LoadPokemonAnim(ballCapsuleSys, 1);
            Window_SetMessage(&ballCapsuleSys->ballCapsuleEditor.windows[0], 13);
            ballCapsuleSys->unk_3DC++;
            break;
        case 1:
            if (PaletteData_GetSelectedBuffersMask(ballCapsuleSys->ballCapsuleEditor.paletteData) != 0) {
                break;
            }

            {
                BallThrow v0;

                v0.type = 1;
                v0.heapID = HEAP_ID_BALL_CAPSULE_SYSTEM;
                v0.mode = 0;
                v0.target = 0xFF;
                v0.bgPrio = 0;
                v0.surface = 1;
                v0.cellActorSys = ballCapsuleSys->ballCapsuleEditor.spriteSys;
                v0.paletteSys = ballCapsuleSys->ballCapsuleEditor.paletteData;
                v0.ballID = Pokemon_GetValue(ballCapsuleSys->mon, MON_DATA_POKEBALL, NULL);

                ballCapsuleSys->ballCapsuleEditor.ballRotation = ov12_02237728(&v0);
                ballCapsuleSys->unk_3DC++;
            }
            break;
        case 2: {
            int v1;
            BallCapsule v2;
            UnkStruct_ov12_02236030 v3 = { 0 };

            v3.unk_00 = 1;
            v3.unk_08 = ballCapsuleSys->mon;
            v1 = ballCapsuleSys->selectedCapsules[0] + 1;

            ov76_0223B848(&v2, ballCapsuleSys);
            Pokemon_SetValue(ballCapsuleSys->mon, MON_DATA_BALL_CAPSULE_ID, (u8 *)&v1);
            Pokemon_SetValue(ballCapsuleSys->mon, MON_DATA_BALL_CAPSULE, &v2);

            ballCapsuleSys->ballCapsuleEditor.unk_154 = ov12_02236004(HEAP_ID_BALL_CAPSULE_SYSTEM, &v3);
            ov12_02236320(ballCapsuleSys->ballCapsuleEditor.unk_154);
        }
            ballCapsuleSys->unk_3DC++;
            ballCapsuleSys->unk_3E0 = 0;
            break;
        case 3:
            if (ov12_02236374(ballCapsuleSys->ballCapsuleEditor.unk_154) != 1) {
                break;
            }

            if (ov12_02237810(ballCapsuleSys->ballCapsuleEditor.ballRotation) == 1) {
                ov76_0223DE78(ballCapsuleSys);
                ov12_022363B4(ballCapsuleSys->ballCapsuleEditor.unk_154);
                BallCapsuleSystem_LoadPokemonAnim(ballCapsuleSys, 0);
                ov76_0223DE54(ballCapsuleSys);
                Sound_PlayEffect(SEQ_SE_DP_CUSTOM05);
                ballCapsuleSys->unk_3DC++;
            }
            break;
        case 4: {
            BOOL v4 = ov76_0223DE78(ballCapsuleSys);

            if (ov12_022377F8(ballCapsuleSys->ballCapsuleEditor.ballRotation) != 0) {
                break;
            }

            if (v4 == 0) {
                ov12_0223783C(ballCapsuleSys->ballCapsuleEditor.ballRotation);
                GXLayers_EngineAToggleLayers(GX_PLANEMASK_OBJ, 0);
                BallCapsuleSystem_LoadPokemonSprite(ballCapsuleSys);
                ballCapsuleSys->unk_3E0 = 0;
                ballCapsuleSys->unk_3DC++;
            }
        } break;
        case 5:
            if (ov12_022363C4(ballCapsuleSys->ballCapsuleEditor.unk_154) != 0) {
                break;
            }

            if (PokemonAnimManager_HasAnimCompleted(ballCapsuleSys->ballCapsuleEditor.animManager, 0) != TRUE) {
                break;
            }

            if (PokemonSprite_IsAnimActive(ballCapsuleSys->ballCapsuleEditor.monSprite) != 0) {
                break;
            }

            ov12_02236428(ballCapsuleSys->ballCapsuleEditor.unk_154);
            ballCapsuleSys->unk_3DC++;
            break;
        case 6:
            if ((++ballCapsuleSys->unk_3E0) < 30) {
                break;
            }

            ballCapsuleSys->unk_3E0 = 0;
            {
                BallThrow v5;

                v5.type = 1;
                v5.heapID = HEAP_ID_BALL_CAPSULE_SYSTEM;
                v5.mode = 5;
                v5.target = 0xFF;
                v5.bgPrio = 0;
                v5.surface = 1;
                v5.cellActorSys = ballCapsuleSys->ballCapsuleEditor.spriteSys;
                v5.paletteSys = ballCapsuleSys->ballCapsuleEditor.paletteData;
                v5.ballID = Pokemon_GetValue(ballCapsuleSys->mon, MON_DATA_POKEBALL, NULL);

                ballCapsuleSys->ballCapsuleEditor.ballRotation = ov12_02237728(&v5);

                BallCapsuleSystem_SetDefaultBlend();
                GXLayers_EngineAToggleLayers(GX_PLANEMASK_OBJ, 1);

                ballCapsuleSys->unk_3DC++;
            }
            break;
        case 7: {
            BOOL v6 = ov76_0223DEF4(ballCapsuleSys);

            if ((ov12_022377F8(ballCapsuleSys->ballCapsuleEditor.ballRotation) == 0) && (v6 == 0)) {
                PaletteData_StartFade(ballCapsuleSys->ballCapsuleEditor.paletteData, 0x2, (1 << 0) | (1 << 1) | (1 << 3) | (1 << 11), 0, 10, 0, 0);
                PaletteData_StartFade(ballCapsuleSys->ballCapsuleEditor.paletteData, 0x8, 0xFFFF, 0, 10, 0, 0);
                BallCapsuleSystem_LoadPokemonAnim(ballCapsuleSys, 1);
                BallCapsuleSystem_DeletePokemonSprite(ballCapsuleSys);
                ov12_0223783C(ballCapsuleSys->ballCapsuleEditor.ballRotation);
                ballCapsuleSys->unk_3DC++;
            }
        } break;
        default:
            if (PaletteData_GetSelectedBuffersMask(ballCapsuleSys->ballCapsuleEditor.paletteData) != 0) {
                break;
            }

            GXLayers_EngineAToggleLayers(GX_PLANEMASK_OBJ, 0);
            BallCapsuleSystem_SetTouchScreenActive(ballCapsuleSys, 1);
            ballCapsuleSys->unk_3DC = 0;
            ballCapsuleSys->state = BALL_CAPSULE_SYSTEM_EDIT_MENU;
            Window_SetMessage(&ballCapsuleSys->ballCapsuleEditor.windows[0], 0xFFFF);
            break;
        }
    } break;
    case 6:
        Window_SetMessage(&ballCapsuleSys->ballCapsuleEditor.windows[0], 12);

        if (ov76_0223B78C(ballCapsuleSys) == 1) {
            ballCapsuleSys->ballCapsuleEditor.unk_18C = 1;
        }

        ov76_0223B808(ballCapsuleSys);
        BallCapsuleSystem_UpdateCapsuleData(ballCapsuleSys);
        {
            GameRecords *v7;
            v7 = SaveData_GetGameRecords(ballCapsuleSys->appData->saveData);

            GameRecords_IncrementTrainerScore(v7, TRAINER_SCORE_EVENT_UNK_06);
        }

        {
            Pokemon *v8;

            if (ballCapsuleSys->editData[ballCapsuleSys->selectedCapsules[0]].partyIndex != BALL_CAPSULE_INVALID_PARTY_INDEX) {
                v8 = ballCapsuleSys->appData->mons[ballCapsuleSys->editData[ballCapsuleSys->selectedCapsules[0]].partyIndex];

                Pokemon_SetValue(v8, MON_DATA_BALL_CAPSULE, SealCase_GetCapsuleById(ballCapsuleSys->appData->sealCase, ballCapsuleSys->selectedCapsules[0]));
            }
        }
        ballCapsuleSys->state = BALL_CAPSULE_SYSTEM_8;
        break;
    case 7: {
        switch (ballCapsuleSys->unk_3DC) {
        case 0:
            if (ov76_0223B78C(ballCapsuleSys) == 0) {
                ballCapsuleSys->state = BALL_CAPSULE_SYSTEM_8;
                break;
            }

            PaletteData_StartFade(ballCapsuleSys->ballCapsuleEditor.paletteData, 0x2, (1 << 0) | (1 << 1) | (1 << 3) | (1 << 11), 0, 0, 10, 0);
            PaletteData_StartFade(ballCapsuleSys->ballCapsuleEditor.paletteData, 0x8, 0xFFFF, 0, 0, 10, 0);
            BallCapsuleSystem_SetTouchScreenActive(ballCapsuleSys, 0);
            GXLayers_EngineBToggleLayers(GX_PLANEMASK_BG0, 0);
            ballCapsuleSys->unk_3DC++;
            break;
        case 1:
            if (PaletteData_GetSelectedBuffersMask(ballCapsuleSys->ballCapsuleEditor.paletteData) != 0) {
                break;
            }

            PaletteData_SetAutoTransparent(ballCapsuleSys->ballCapsuleEditor.paletteData, 0);
            CreateWindowWithScroll(ballCapsuleSys->ballCapsuleEditor.bgConfig, &ballCapsuleSys->ballCapsuleEditor.windows[2], 4, 2, 1, 27, 4, 0 + ((1 + (18 + 12)) + 9));

            {
                UnkStruct_02015958 v9;

                v9.unk_00 = ballCapsuleSys->ballCapsuleEditor.bgConfig;
                v9.unk_04 = 4;
                v9.unk_08 = (((3 + 1) * 2) + (((3 + 1) * 2) + (((3 + 1) * 2) + (((3 + 1) * 2) + (((3 + 1) * 2) + (((3 + 1) * 2) + (((3 + 1) * 2) + (((3 + 1) * 2) + ((27 * 4) + (0 + ((1 + (18 + 12)) + 9)))))))))));
                v9.unk_0C = 5;
                v9.unk_10 = 25;
                v9.unk_11 = 6;

                ballCapsuleSys->ballCapsuleEditor.unk_150 = sub_02015920(HEAP_ID_BALL_CAPSULE_SYSTEM);
                sub_02015958(ballCapsuleSys->ballCapsuleEditor.unk_150, &v9);
            }

            Window_SetMessage(&ballCapsuleSys->ballCapsuleEditor.windows[2], 14);
            ballCapsuleSys->unk_3DC++;
            break;
        case 2:
            GXLayers_EngineBToggleLayers(GX_PLANEMASK_BG0, 1);
            ballCapsuleSys->unk_3DC++;
        case 3: {
            u32 v10;
            v10 = sub_020159FC(ballCapsuleSys->ballCapsuleEditor.unk_150);

            switch (v10) {
            case 1:
            case 2:
                ballCapsuleSys->unk_3E0 = v10;
                PaletteData_SetAutoTransparent(ballCapsuleSys->ballCapsuleEditor.paletteData, 1);
                sub_02015A54(ballCapsuleSys->ballCapsuleEditor.unk_150);
                sub_02015938(ballCapsuleSys->ballCapsuleEditor.unk_150);
                Window_EraseMessageBox(&ballCapsuleSys->ballCapsuleEditor.windows[2], 1);
                Window_ClearAndCopyToVRAM(&ballCapsuleSys->ballCapsuleEditor.windows[2]);
                Window_Remove(&ballCapsuleSys->ballCapsuleEditor.windows[2]);
                ballCapsuleSys->unk_3DC++;
                break;
            case 0:
            default:
                break;
            }
        } break;
        case 4:
            PaletteData_StartFade(ballCapsuleSys->ballCapsuleEditor.paletteData, 0x2, (1 << 0) | (1 << 1) | (1 << 3) | (1 << 11), 0, 10, 0, 0);
            PaletteData_StartFade(ballCapsuleSys->ballCapsuleEditor.paletteData, 0x8, 0xFFFF, 0, 10, 0, 0);
            ballCapsuleSys->unk_3DC++;
            break;
        case 5:
            if (PaletteData_GetSelectedBuffersMask(ballCapsuleSys->ballCapsuleEditor.paletteData) != 0) {
                break;
            }

            switch (ballCapsuleSys->unk_3E0) {
            case 1:
                BallCapsuleSystem_LoadSelectedCapsule(ballCapsuleSys);
                BallCapsuleSystem_SetSealCounts(ballCapsuleSys);
                BallCapsuleSystem_PrintSealCountsToWindows(ballCapsuleSys);
                ov76_0223B678(ballCapsuleSys);
                ov76_0223B208(ballCapsuleSys);
                ov76_0223B69C(ballCapsuleSys, 1);
                ballCapsuleSys->state = BALL_CAPSULE_SYSTEM_EXIT;
                break;
            case 2:
                ballCapsuleSys->state = BALL_CAPSULE_SYSTEM_EDIT_MENU;
                break;
            case 0:
            default:
                break;
            }

            BallCapsuleSystem_SetTouchScreenActive(ballCapsuleSys, 1);
            ballCapsuleSys->unk_3DC = 0;
        }
    } break;
    case 8:
        GXLayers_EngineAToggleLayers(GX_PLANEMASK_OBJ, 1);
        PaletteData_StartFade(ballCapsuleSys->ballCapsuleEditor.paletteData, 0x1, 1 << 1, 0, 0, 16, 0);
        ballCapsuleSys->state++;
        break;
    case 9:

        if (PaletteData_GetSelectedBuffersMask(ballCapsuleSys->ballCapsuleEditor.paletteData) != 0) {
            break;
        }

        PaletteData_StartFade(ballCapsuleSys->ballCapsuleEditor.paletteData, 0x1, 1 << 0, 0, 16, 0, 0);
        PaletteData_StartFade(ballCapsuleSys->ballCapsuleEditor.paletteData, 0x4, 0xFFFF, 0, 16, 0, 0);
        Bg_SetPriority(BG_LAYER_MAIN_3, 3);
        Bg_SetPriority(BG_LAYER_SUB_3, 3);
        GXLayers_EngineBToggleLayers(GX_PLANEMASK_BG2, 0);
        BallCapsuleSystem_SetButtonDrawFlags(ballCapsuleSys, 0);
        ov76_0223C568(ballCapsuleSys, 0);
        ov76_0223B96C(ballCapsuleSys, 0);
        ov76_0223BD30(ballCapsuleSys, -1, 4);
        BallCapsuleSystem_SetTouchScreenActive(ballCapsuleSys, 0);
        ballCapsuleSys->state++;
        break;
    case 10:
        if (PaletteData_GetSelectedBuffersMask(ballCapsuleSys->ballCapsuleEditor.paletteData) != 0) {
            break;
        }

        if (ov76_0223DCB0(ballCapsuleSys) == 1) {
            break;
        }

        ManagedSprite_SetDrawFlag(ballCapsuleSys->unk_2F4[1], 0);
        ov76_0223D494(ballCapsuleSys, 0, 0xff, 0);
        Window_SetMessage(&ballCapsuleSys->ballCapsuleEditor.windows[0], 7);
        BallCapsuleSys_DeleteSealCountWindows(ballCapsuleSys);

        return 1;
    }

    if (ballCapsuleSys->ballCapsuleEditor.unk_00 == 0xFF) {
        TouchScreenActions_HandleAction(ballCapsuleSys->ballCapsuleEditor.touchScreenActions);
    } else {
        u32 v11, v12;
        int v13;
        BOOL v14;

        v13 = ballCapsuleSys->ballCapsuleEditor.unk_00;
        v14 = TouchScreen_GetHoldState(&v11, &v12);

        if (v14) {
            int v15;

            v15 = SealData_GetNameID(ballCapsuleSys->placedSeals[v13].type);
            Window_SetSealNameMessage(&ballCapsuleSys->ballCapsuleEditor.windows[0], v15);
            ManagedSprite_SetPositionXY(ballCapsuleSys->placedSeals[v13].sealSprite, v11, v12);
            ov76_0223B758(ballCapsuleSys, v13);
            ov76_0223B7D4(ballCapsuleSys, v13);
        } else {
            BOOL v16;

            v16 = ov76_0223B6C4(ballCapsuleSys, v13);
            ov76_0223B184(ballCapsuleSys->placedSeals[v13].rect, ballCapsuleSys->placedSeals[v13].sealSprite, 0);

            if (v16 == 0) {
                GiveOrTakeSeal2(ballCapsuleSys->appData->sealCase, ballCapsuleSys->placedSeals[v13].type, +1);
                BallCapsuleSystem_PrintSealCountsToWindows(ballCapsuleSys);
                ov76_0223B704(ballCapsuleSys, v13);
            }

            Sound_PlayEffect(SEQ_SE_DP_BOX01);
            ballCapsuleSys->ballCapsuleEditor.unk_00 = 0xFF;
            Window_SetSealNameMessage(&ballCapsuleSys->ballCapsuleEditor.windows[0], 0xFFFF);
        }
    }

    BallCapsuleSystem_NOP(ballCapsuleSys);
    ov76_0223C544(ballCapsuleSys);
    BallCapsuleSystem_TickPartyIcons(ballCapsuleSys);
    BallCapsuleSystem_TickSprites(ballCapsuleSys);

    return 1;
}

static BOOL ov76_0223E8A4(BallCapsuleSystem *ballCapsuleSys)
{
    switch (ballCapsuleSys->state) {
    case 0:
        ballCapsuleSys->state++;
    case 1:
        BallCapsuleSystem_ScreeFadeOut();
        ballCapsuleSys->state++;
        break;
    case 2:
        if (IsScreenFadeDone() != 1) {
            break;
        }

        Window_Remove(&ballCapsuleSys->ballCapsuleEditor.windows[0]);
        BallCapsuleSystem_DeleteSprites(ballCapsuleSys);
        BallCapsuleSystem_DeletePartyIcons(ballCapsuleSys);
        BallCapsuleSystem_DeleteStaticButtons(ballCapsuleSys);
        ov76_0223C588(ballCapsuleSys);
        BallCapsuleSystem_UnloadEditorSprites(ballCapsuleSys);
        sub_02097F30(ballCapsuleSys->appData, 1);

        return 0;
    }

    BallCapsuleSystem_TickPartyIcons(ballCapsuleSys);
    BallCapsuleSystem_TickSprites(ballCapsuleSys);

    return 1;
}

void BallCapsuleSystem_UnsetCapsule(BallCapsuleSystem *ballCapsuleSys, int param1)
{
    int v0;
    int v1;
    int v2 = 0;
    BallCapsule v3;

    v1 = ballCapsuleSys->capsuleData[param1].partyIndex;

    if (v1 != BALL_CAPSULE_INVALID_PARTY_INDEX) {
        Pokemon_SetValue(ballCapsuleSys->appData->mons[v1], MON_DATA_BALL_CAPSULE_ID, (u8 *)&v2);
    }

    ballCapsuleSys->capsuleData[param1].partyIndex = BALL_CAPSULE_INVALID_PARTY_INDEX;
    BallCapsuleSystem_UpdateCapsuleData(ballCapsuleSys);
}

static BOOL ov76_0223E950(BallCapsuleSystem *ballCapsuleSys)
{
    switch (ballCapsuleSys->state) {
    case 0:
        BallCapsuleSystem_UnsetCapsule(ballCapsuleSys, ballCapsuleSys->selectedCapsules[0]);
        Window_SetMessage(&ballCapsuleSys->ballCapsuleEditor.windows[0], 9);
        ballCapsuleSys->state++;
        break;
    case 1:
        if (gSystem.pressedKeys & (0x1 | 0x2 | 0x400 | 0x800 | 0x40 | 0x80 | 0x20 | 0x10)) {
            ballCapsuleSys->state++;
        }
        break;
    case 2:
        ov76_0223D494(ballCapsuleSys, 0, 0xff, 0);
        Window_SetMessage(&ballCapsuleSys->ballCapsuleEditor.windows[0], 7);
        break;
    }

    BallCapsuleSystem_TickPartyIcons(ballCapsuleSys);
    BallCapsuleSystem_TickSprites(ballCapsuleSys);

    return 1;
}

static BOOL ov76_0223E9C4(BallCapsuleSystem *ballCapsuleSys)
{
    switch (ballCapsuleSys->state) {
    case 0:
        ManagedSprite_SetPriority(ballCapsuleSys->unk_2F4[0], 25);
        ManagedSprite_SetPriority(ballCapsuleSys->unk_2F4[1], 20);
        ManagedSprite_SetAnim(ballCapsuleSys->unk_2F4[0], 1);
        Window_SetMessage(&ballCapsuleSys->ballCapsuleEditor.windows[0], 10);
        ManagedSprite_SetDrawFlag(ballCapsuleSys->unk_2F4[1], 1);
        ballCapsuleSys->state++;
    case 1: {
        BOOL v0;

        v0 = BallCapsuleSystem_MoveCursor(&(ballCapsuleSys->selectedCapsules[1]));

        if (v0 == 1) {
            ov76_0223D600(ballCapsuleSys, 1, 0);

            Sound_PlayEffect(SEQ_SE_CONFIRM);
        } else if (gSystem.pressedKeys & PAD_BUTTON_A) {
            BallCapsuleSys_SwapCapsules(ballCapsuleSys, ballCapsuleSys->selectedCapsules[0], ballCapsuleSys->selectedCapsules[1]);
            ov76_0223D600(ballCapsuleSys, 1, 1);
            Window_SetMessage(&ballCapsuleSys->ballCapsuleEditor.windows[0], 11);
            ballCapsuleSys->state = BALL_CAPSULE_SYSTEM_WAIT_FOR_FADE_IN;
            Sound_PlayEffect(SEQ_SE_CONFIRM);
        } else if (gSystem.pressedKeys & PAD_BUTTON_B) {
            ManagedSprite_SetDrawFlag(ballCapsuleSys->unk_2F4[1], 0);
            ov76_0223D600(ballCapsuleSys, 0, 1);
            ballCapsuleSys->state = BALL_CAPSULE_SYSTEM_MAIN;
            Sound_PlayEffect(SEQ_SE_DP_DECIDE);
        }
    } break;
    case 2:
        if (gSystem.pressedKeys & (0x1 | 0x2 | 0x400 | 0x800 | 0x40 | 0x80 | 0x20 | 0x10)) {
            ballCapsuleSys->state = BALL_CAPSULE_SYSTEM_MAIN;
        }
        break;
    case 3:
        ManagedSprite_SetPriority(ballCapsuleSys->unk_2F4[0], 20);
        ManagedSprite_SetPriority(ballCapsuleSys->unk_2F4[1], 25);
        ov76_0223D494(ballCapsuleSys, 0, 0xff, 0);
        ManagedSprite_SetAnim(ballCapsuleSys->unk_2F4[0], 0);
        Window_SetMessage(&ballCapsuleSys->ballCapsuleEditor.windows[0], 7);
        break;
    }

    BallCapsuleSystem_TickPartyIcons(ballCapsuleSys);
    BallCapsuleSystem_TickSprites(ballCapsuleSys);

    return 1;
}

static BOOL ov76_0223EB1C(BallCapsuleSystem *ballCapsuleSys)
{
    return 0;
}

void ov76_0223EB20(int param0)
{
    SetVBlankCallback(NULL, NULL);
    DisableHBlank();
    GXLayers_DisableEngineALayers();
    GXLayers_DisableEngineBLayers();

    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
}

void ov76_0223EB54(int param0)
{
    SetVBlankCallback(NULL, NULL);
    DisableHBlank();
}

void ov76_0223EB64(BgConfig *param0)
{
    GXLayers_DisableEngineALayers();

    {
        GXBanks v0 = {
            GX_VRAM_BG_128_A,
            GX_VRAM_BGEXTPLTT_NONE,
            GX_VRAM_SUB_BG_32_H,
            GX_VRAM_SUB_BGEXTPLTT_NONE,
            GX_VRAM_OBJ_64_E,
            GX_VRAM_OBJEXTPLTT_NONE,
            GX_VRAM_SUB_OBJ_16_I,
            GX_VRAM_SUB_OBJEXTPLTT_NONE,
            GX_VRAM_TEX_01_BC,
            GX_VRAM_TEXPLTT_01_FG
        };

        GXLayers_SetBanks(&v0);

        MI_CpuClear32((void *)HW_BG_VRAM, HW_BG_VRAM_SIZE);
        MI_CpuClear32((void *)HW_DB_BG_VRAM, HW_DB_BG_VRAM_SIZE);
        MI_CpuClear32((void *)HW_OBJ_VRAM, HW_OBJ_VRAM_SIZE);
        MI_CpuClear32((void *)HW_DB_OBJ_VRAM, HW_DB_OBJ_VRAM_SIZE);
    }

    {
        GraphicsModes v1 = {
            GX_DISPMODE_GRAPHICS,
            GX_BGMODE_0,
            GX_BGMODE_0,
            GX_BG0_AS_3D,
        };

        SetAllGraphicsModes(&v1);
    }

    {
        BgTemplate v2[] = {
            {
                .x = 0,
                .y = 0,
                .bufferSize = 0x800,
                .baseTile = 0,
                .screenSize = BG_SCREEN_SIZE_256x256,
                .colorMode = GX_BG_COLORMODE_16,
                .screenBase = GX_BG_SCRBASE_0x0000,
                .charBase = GX_BG_CHARBASE_0x04000,
                .bgExtPltt = GX_BG_EXTPLTT_01,
                .priority = 0,
                .areaOver = 0,
                .mosaic = FALSE,
            },
            {
                .x = 0,
                .y = 0,
                .bufferSize = 0x2000,
                .baseTile = 0,
                .screenSize = BG_SCREEN_SIZE_256x256,
                .colorMode = GX_BG_COLORMODE_16,
                .screenBase = GX_BG_SCRBASE_0x1000,
                .charBase = GX_BG_CHARBASE_0x0c000,
                .bgExtPltt = GX_BG_EXTPLTT_01,
                .priority = 2,
                .areaOver = 0,
                .mosaic = FALSE,
            },
            {
                .x = 0,
                .y = 0,
                .bufferSize = 0x1000,
                .baseTile = 0,
                .screenSize = BG_SCREEN_SIZE_256x256,
                .colorMode = GX_BG_COLORMODE_16,
                .screenBase = GX_BG_SCRBASE_0x3000,
                .charBase = GX_BG_CHARBASE_0x10000,
                .bgExtPltt = GX_BG_EXTPLTT_01,
                .priority = 3,
                .areaOver = 0,
                .mosaic = FALSE,
            },
        };

        Bg_InitFromTemplate(param0, BG_LAYER_MAIN_1, &v2[0], 0);
        Bg_InitFromTemplate(param0, BG_LAYER_MAIN_2, &v2[1], 0);
        Bg_InitFromTemplate(param0, BG_LAYER_MAIN_3, &v2[2], 0);
        Bg_ClearTilemap(param0, BG_LAYER_MAIN_1);
        Bg_ClearTilemap(param0, BG_LAYER_MAIN_2);
        Bg_ClearTilemap(param0, BG_LAYER_MAIN_3);

        G2_SetBG0Priority(1);
        GXLayers_EngineAToggleLayers(GX_PLANEMASK_BG0, 1);
    }

    {
        BgTemplate v3[] = {
            {
                .x = 0,
                .y = 0,
                .bufferSize = 0x800,
                .baseTile = 0,
                .screenSize = BG_SCREEN_SIZE_256x256,
                .colorMode = GX_BG_COLORMODE_16,
                .screenBase = GX_BG_SCRBASE_0x6800,
                .charBase = GX_BG_CHARBASE_0x00000,
                .bgExtPltt = GX_BG_EXTPLTT_01,
                .priority = 0,
                .areaOver = 0,
                .mosaic = FALSE,
            },
            {
                .x = 0,
                .y = 0,
                .bufferSize = 0x800,
                .baseTile = 0,
                .screenSize = BG_SCREEN_SIZE_256x256,
                .colorMode = GX_BG_COLORMODE_16,
                .screenBase = GX_BG_SCRBASE_0x7000,
                .charBase = GX_BG_CHARBASE_0x04000,
                .bgExtPltt = GX_BG_EXTPLTT_01,
                .priority = 2,
                .areaOver = 0,
                .mosaic = FALSE,
            },
            {
                .x = 0,
                .y = 0,
                .bufferSize = 0x800,
                .baseTile = 0,
                .screenSize = BG_SCREEN_SIZE_256x256,
                .colorMode = GX_BG_COLORMODE_16,
                .screenBase = GX_BG_SCRBASE_0x6000,
                .charBase = GX_BG_CHARBASE_0x00000,
                .bgExtPltt = GX_BG_EXTPLTT_01,
                .priority = 1,
                .areaOver = 0,
                .mosaic = FALSE,
            },
            {
                .x = 0,
                .y = 0,
                .bufferSize = 0x800,
                .baseTile = 0,
                .screenSize = BG_SCREEN_SIZE_256x256,
                .colorMode = GX_BG_COLORMODE_16,
                .screenBase = GX_BG_SCRBASE_0x7800,
                .charBase = GX_BG_CHARBASE_0x04000,
                .bgExtPltt = GX_BG_EXTPLTT_01,
                .priority = 3,
                .areaOver = 0,
                .mosaic = FALSE,
            },
        };

        Bg_InitFromTemplate(param0, BG_LAYER_SUB_0, &v3[0], 0);
        Bg_InitFromTemplate(param0, BG_LAYER_SUB_1, &v3[1], 0);
        Bg_InitFromTemplate(param0, BG_LAYER_SUB_2, &v3[2], 0);
        Bg_InitFromTemplate(param0, BG_LAYER_SUB_3, &v3[3], 0);
        Bg_ClearTilemap(param0, BG_LAYER_SUB_0);
        Bg_ClearTilemap(param0, BG_LAYER_SUB_1);
        Bg_ClearTilemap(param0, BG_LAYER_SUB_2);
        Bg_ClearTilemap(param0, BG_LAYER_SUB_3);
        GXLayers_EngineBToggleLayers(GX_PLANEMASK_BG2, 0);
    }
}

void ov76_0223ECB0(void *param0)
{
    BallCapsuleSystem *ballCapsuleSys = param0;

    PokemonSpriteManager_UpdateCharAndPltt(ballCapsuleSys->ballCapsuleEditor.monSpriteMan);
    VramTransfer_Process();
    SpriteSystem_TransferOam();
    PaletteData_CommitFadedBuffers(ballCapsuleSys->ballCapsuleEditor.paletteData);
    Bg_RunScheduledUpdates(ballCapsuleSys->ballCapsuleEditor.bgConfig);

    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}
