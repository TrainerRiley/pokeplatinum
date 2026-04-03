#include "overlay076/ov76_0223B140.h"

#include <nitro.h>
#include <nitro/sinit.h>

#include "constants/ball_capsule.h"

#include "struct_defs/seal_case.h"

#include "battle_anim/ov12_02237E54.h"
#include "overlay076/struct_ov76_0223DE00.h"

#include "game_overlay.h"
#include "palette.h"
#include "sprite_system.h"
#include "system.h"
#include "touch_screen.h"
#include "unk_0202C9F4.h"
#include "unk_02097B18.h"

FS_EXTERN_OVERLAY(overlay11);
FS_EXTERN_OVERLAY(battle_anim);

static void NitroStaticInit(void)
{
    Overlay_LoadByID(FS_OVERLAY_ID(overlay11), OVERLAY_LOAD_ASYNC);
    Overlay_LoadByID(FS_OVERLAY_ID(battle_anim), OVERLAY_LOAD_ASYNC);
}

// 20x20 rect centered at (x,y)
static void SetPlacedSealButtonPos(TouchScreenRect *rect, u8 x, u8 y)
{
    rect->rect.top = y - 10;
    rect->rect.left = x - 10;
    rect->rect.bottom = y + 10;
    rect->rect.right = x + 10;
}

static void Rect_SetToFullScreenDimensions(TouchScreenRect *rect)
{
    rect->rect.top = 0;
    rect->rect.left = 0;
    rect->rect.bottom = 192;
    rect->rect.right = 255;
}

void UpdatePlacedSealPos(TouchScreenRect *rect, ManagedSprite *sprite, BOOL movingSeal)
{
    if (sprite == NULL) {
        return;
    }

    s16 x, y;
    if (movingSeal == TRUE) {
        ManagedSprite_SetPositionXY(sprite, gSystem.touchX, gSystem.touchY);
        ManagedSprite_GetPositionXY(sprite, &x, &y);
        Rect_SetToFullScreenDimensions(rect);
    } else {
        ManagedSprite_GetPositionXY(sprite, &x, &y);
        SetPlacedSealButtonPos(rect, x, y);
    }
}

void BallCapsuleSystem_UpdatePlacedSealSpriteOffsets(BallCapsuleSystem *ballCapsuleSys)
{
    for (int i = 0; i < SEALS_PER_CAPSULE; i++) {
        if (ballCapsuleSys->currentCapsuleSeals[i].sealSprite == NULL) {
            continue;
        }

        ManagedSprite_OffsetPositionXY(ballCapsuleSys->currentCapsuleSeals[i].sealSprite, -(+7 * 8), -(-2 * 8));
    }
}

void BallCapsuleSystem_LoadSelectedCapsulePlacedSeals(BallCapsuleSystem *ballCapsuleSys)
{
    BallCapsule *capsule = ballCapsuleSys->capsuleData[ballCapsuleSys->selectedCapsules[0]].ballCapsule;

    for (int i = 0; i < SEALS_PER_CAPSULE; i++) {
        if (capsule->seals[i].type != SEAL_DUMMY) {
            ballCapsuleSys->currentCapsuleSeals[i].type = capsule->seals[i].type;
            ballCapsuleSys->currentCapsuleSeals[i].x = capsule->seals[i].x;
            ballCapsuleSys->currentCapsuleSeals[i].y = capsule->seals[i].y;

            BallCapsuleSystem_CreatePlacedSealSprite(ballCapsuleSys, i);
            UpdatePlacedSealPos(ballCapsuleSys->currentCapsuleSeals[i].rect, ballCapsuleSys->currentCapsuleSeals[i].sealSprite, FALSE);

            ballCapsuleSys->currentCapsuleSeals[i].tapped = TRUE;
        } else {
            ballCapsuleSys->currentCapsuleSeals[i].tapped = FALSE;
            ballCapsuleSys->currentCapsuleSeals[i].type = 0;
        }
    }
}

// returns which index the seal was placed in
int BallCapsuleSystem_AddPlacedSeal(BallCapsuleSystem *ballCapsuleSys, u8 sealPageIndex)
{
    for (int i = 0; i < SEALS_PER_CAPSULE; i++) {
        // Find first untapped seal
        if (ballCapsuleSys->currentCapsuleSeals[i].tapped == TRUE) {
            continue;
        }

        ballCapsuleSys->currentCapsuleSeals[i].type = ballCapsuleSys->sealCasePages.currentPageSeals[sealPageIndex];
        ballCapsuleSys->currentCapsuleSeals[i].x = 190;
        ballCapsuleSys->currentCapsuleSeals[i].y = 70;

        u32 x, y;
        TouchScreen_GetHoldState(&x, &y);

        ballCapsuleSys->currentCapsuleSeals[i].x = x;
        ballCapsuleSys->currentCapsuleSeals[i].y = y;

        BallCapsuleSystem_CreatePlacedSealSprite(ballCapsuleSys, i);
        BallCapsuleSystem_IncreasePlacedSealPriority(ballCapsuleSys, i);

        ballCapsuleSys->currentCapsuleSeals[i].tapped = TRUE;

        return i;
    }

    return 0xFF;
}

BOOL BallCapsuleSystem_AnyPlacedSealTapped(BallCapsuleSystem *ballCapsuleSys)
{
    for (int i = 0; i < SEALS_PER_CAPSULE; i++) {
        if (ballCapsuleSys->currentCapsuleSeals[i].tapped == TRUE) {
            continue;
        }

        return TRUE;
    }

    return FALSE;
}

void BallCapsuleSystem_IncreasePlacedSealPriority(BallCapsuleSystem *ballCapsuleSys, u8 placedSealIndex)
{
    for (int i = 0; i < SEALS_PER_CAPSULE; i++) {
        // Find tapped seal
        if (ballCapsuleSys->currentCapsuleSeals[i].tapped != TRUE) {
            continue;
        }

        if (i == placedSealIndex) {
            ManagedSprite_SetPriority(ballCapsuleSys->currentCapsuleSeals[i].sealSprite, 0);
        } else {
            int priority = ManagedSprite_GetPriority(ballCapsuleSys->currentCapsuleSeals[i].sealSprite);

            if (priority == 0) {
                ManagedSprite_SetPriority(ballCapsuleSys->currentCapsuleSeals[i].sealSprite, priority + 1);
            } else {
                ManagedSprite_SetPriority(ballCapsuleSys->currentCapsuleSeals[i].sealSprite, priority + 2);
            }
        }
    }
}

void BallCapsuleSystem_LoadPlacedSealSpriteData(BallCapsuleSystem *ballCapsuleSys, u8 sealID, u8 placedSealIndex)
{
    SpriteSystem *spriteSys = ballCapsuleSys->ballCapsuleEditor.spriteSys;
    SpriteManager *spriteMan = ballCapsuleSys->ballCapsuleEditor.spriteMan;
    PaletteData *paletteData = ballCapsuleSys->ballCapsuleEditor.paletteData;

    SpriteSystem_LoadPaletteBuffer(paletteData, PLTTBUF_SUB_OBJ, spriteSys, spriteMan, 91, 293, 0, 1, NNS_G2D_VRAM_TYPE_2DSUB, 21000 + 293);

    int charID = SealData_GetCharID(sealID);
    int resourceID = (placedSealIndex + 20000);

    SpriteSystem_LoadCharResObj(spriteSys, spriteMan, NARC_INDEX_APPLICATION__CUSTOM_BALL__DATA__CB_DATA, charID, TRUE, NNS_G2D_VRAM_TYPE_2DSUB, resourceID);
    SpriteSystem_LoadCellResObj(spriteSys, spriteMan, NARC_INDEX_APPLICATION__CUSTOM_BALL__DATA__CB_DATA, 93, TRUE, 22000 + 93);
    SpriteSystem_LoadAnimResObj(spriteSys, spriteMan, NARC_INDEX_APPLICATION__CUSTOM_BALL__DATA__CB_DATA, 1, TRUE, 23000 + 1);
}

void BallCapsuleSystem_SortPlacedSeals(BallCapsuleSystem *ballCapsuleSys)
{
    if (ballCapsuleSys->ballCapsuleEditor.dirty == FALSE) {
        return;
    }
    ballCapsuleSys->ballCapsuleEditor.dirty = FALSE;

    int priorities[SEALS_PER_CAPSULE];
    int indices[SEALS_PER_CAPSULE];

    int i;
    for (i = 0; i < SEALS_PER_CAPSULE; i++) {
        priorities[i] = 0xFF;
        indices[i] = 0xFF;

        if (ballCapsuleSys->currentCapsuleSeals[i].sealSprite != NULL) {
            priorities[i] = ManagedSprite_GetPriority(ballCapsuleSys->currentCapsuleSeals[i].sealSprite);
            indices[i] = i;
        }
    }

    // sort by priority
    for (i = 0; i < SEALS_PER_CAPSULE - 1; i++) {
        for (int j = SEALS_PER_CAPSULE - 1; j > i; j--) {
            if (priorities[j - 1] >= priorities[j]) {
                int tempPri = priorities[j];
                int tempIndex = indices[j];
                priorities[j] = priorities[j - 1];
                indices[j] = indices[j - 1];
                priorities[j - 1] = tempPri;
                indices[j - 1] = tempIndex;
            }
        }
    }

    BallCapsule ballCapsule;
    BallCapsule ballCapsuleCopy;

    for (i = 0; i < SEALS_PER_CAPSULE; i++) {
        ballCapsule.seals[i].type = ballCapsuleSys->currentCapsuleSeals[i].type;
        ballCapsule.seals[i].x = ballCapsuleSys->currentCapsuleSeals[i].x;
        ballCapsule.seals[i].y = ballCapsuleSys->currentCapsuleSeals[i].y;
    }

    // I have no idea why they make this copy, but removing it causes a mismatch.
    ballCapsuleCopy = ballCapsule;

    for (i = 0; i < SEALS_PER_CAPSULE; i++) {
        // clear unplaced seals
        if (indices[i] == 0xFF) {
            ballCapsuleSys->currentCapsuleSeals[i].type = 0;
            ballCapsuleSys->currentCapsuleSeals[i].x = 0;
            ballCapsuleSys->currentCapsuleSeals[i].y = 0;
            continue;
        }

        ballCapsuleSys->currentCapsuleSeals[i].type = ballCapsuleCopy.seals[indices[i]].type;
        ballCapsuleSys->currentCapsuleSeals[i].x = ballCapsuleCopy.seals[indices[i]].x;
        ballCapsuleSys->currentCapsuleSeals[i].y = ballCapsuleCopy.seals[indices[i]].y;

        if (ballCapsuleSys->currentCapsuleSeals[indices[i]].sealSprite == NULL) {
            continue;
        }

        ManagedSprite_SetPriority(ballCapsuleSys->currentCapsuleSeals[indices[i]].sealSprite, i);
    }
}

BOOL BallCapsuleSystem_CreatePlacedSealSprite(BallCapsuleSystem *ballCapsuleSys, u8 placedSealIndex)
{
    PlacedSeal *placedSeal;
    SpriteSystem *spriteSys = ballCapsuleSys->ballCapsuleEditor.spriteSys;
    SpriteManager *spriteMan = ballCapsuleSys->ballCapsuleEditor.spriteMan;
    placedSeal = &ballCapsuleSys->currentCapsuleSeals[placedSealIndex];

    if (placedSeal->tapped == TRUE) {
        GF_ASSERT(0);
        return FALSE;
    }

    BallCapsuleSystem_LoadPlacedSealSpriteData(ballCapsuleSys, placedSeal->type, placedSealIndex);

    SpriteTemplate template;
    template.x = placedSeal->x;
    template.y = placedSeal->y;
    template.z = 0;
    template.animIdx = 0;
    template.plttIdx = 0;
    template.vramTransfer = FALSE;
    template.priority = 0;
    template.vramType = NNS_G2D_VRAM_TYPE_2DSUB;
    template.bgPriority = 1;
    template.resources[4] = SPRITE_RESOURCE_NONE;
    template.resources[5] = SPRITE_RESOURCE_NONE;

    // unused
    int charID = SealData_GetCharID(placedSeal->type);

    template.resources[0] = 20000 + placedSealIndex;
    template.resources[1] = 21000 + 293;
    template.resources[2] = 22000 + 93;
    template.resources[3] = 23000 + 1;
    placedSeal->sealSprite = SpriteSystem_NewSprite(spriteSys, spriteMan, &template);

    ManagedSprite_TickFrame(placedSeal->sealSprite);

    return TRUE;
}

BOOL BallCapsuleSystem_HandlePlacedSealTouchEvent(BallCapsuleSystem *ballCapsuleSys, enum TouchScreenButtonState touchScreenState, u8 placedSealIndex)
{
    if (ballCapsuleSys->currentCapsuleSeals[placedSealIndex].tapped == FALSE) {
        return TRUE;
    }

    switch (touchScreenState) {
    case TOUCH_BUTTON_PRESSED:
        if (ballCapsuleSys->ballCapsuleEditor.unk_00 == 0xFF) {
            ballCapsuleSys->ballCapsuleEditor.unk_00 = placedSealIndex;
            BallCapsuleSystem_IncreasePlacedSealPriority(ballCapsuleSys, placedSealIndex);
        }
        break;
    case TOUCH_BUTTON_HELD: {
        if (ballCapsuleSys->ballCapsuleEditor.unk_00 == placedSealIndex) {
            UpdatePlacedSealPos(ballCapsuleSys->currentCapsuleSeals[placedSealIndex].rect, ballCapsuleSys->currentCapsuleSeals[placedSealIndex].sealSprite, TRUE);
            BallCapsuleSystem_SetPlacedSealOamMode(ballCapsuleSys, placedSealIndex);
            BallCapsuleSystem_SetSealPosToSpritePos(ballCapsuleSys, placedSealIndex);
        }
    } break;
    case TOUCH_BUTTON_HELD_OUT_OF_BOUNDS:
    case TOUCH_BUTTON_RELEASED: {
        // BOOL v2;

        if (ballCapsuleSys->ballCapsuleEditor.unk_00 == 0xFF) {
            BOOL onBall = BallCapsuleSystem_IsPlacedSealOnBall(ballCapsuleSys, placedSealIndex);
            UpdatePlacedSealPos(ballCapsuleSys->currentCapsuleSeals[placedSealIndex].rect, ballCapsuleSys->currentCapsuleSeals[placedSealIndex].sealSprite, FALSE);

            if (onBall == FALSE) {
                BallCapsuleSystem_RemovePlacedSeal(ballCapsuleSys, placedSealIndex);
            }

            ballCapsuleSys->ballCapsuleEditor.unk_00 = 0xFF;
        }
    } break;
    default:
        GF_ASSERT(0);
        break;
    }

    return TRUE;
}

void BallCapsuleSystem_RemoveTappedSeal(BallCapsuleSystem *ballCapsuleSys)
{
    for (int i = 0; i < SEALS_PER_CAPSULE; i++) {
        if (ballCapsuleSys->currentCapsuleSeals[i].tapped != TRUE) {
            continue;
        }

        BallCapsuleSystem_RemovePlacedSeal(ballCapsuleSys, i);
    }
}

void BallCapsuleSystem_SetDrawFlagOnTappedSeal(BallCapsuleSystem *ballCapsuleSys, int flag)
{
    for (int i = 0; i < SEALS_PER_CAPSULE; i++) {
        if (ballCapsuleSys->currentCapsuleSeals[i].tapped != TRUE) {
            continue;
        }

        ManagedSprite_SetDrawFlag(ballCapsuleSys->currentCapsuleSeals[i].sealSprite, flag);
    }
}

BOOL BallCapsuleSystem_IsPlacedSealOnBall(BallCapsuleSystem *ballCapsuleSys, int placedSealIndex)
{
    if (ballCapsuleSys->currentCapsuleSeals[placedSealIndex].tapped == FALSE) {
        return TRUE;
    }

    s16 x, y;
    ManagedSprite_GetPositionXY(ballCapsuleSys->currentCapsuleSeals[placedSealIndex].sealSprite, &x, &y);

    int dist = ov12_02237E54(x, y, BALL_CAPSULE_EDITOR_BALL_CENTER_X, BALL_CAPSULE_EDITOR_BALL_CENTER_Y);
    if (dist > BALL_CAPSULE_EDITOR_BALL_RADIUS) {
        return FALSE;
    }

    return TRUE;
}

void BallCapsuleSystem_RemovePlacedSeal(BallCapsuleSystem *ballCapsuleSys, int placedSealIndex)
{
    // unused
    int charID = SealData_GetCharID(ballCapsuleSys->currentCapsuleSeals[placedSealIndex].type);

    int resourceID = (placedSealIndex + 20000);

    SpriteManager_UnloadCharObjById(ballCapsuleSys->ballCapsuleEditor.spriteMan, resourceID);
    Sprite_DeleteAndFreeResources(ballCapsuleSys->currentCapsuleSeals[placedSealIndex].sealSprite);

    ballCapsuleSys->currentCapsuleSeals[placedSealIndex].sealSprite = NULL;
    ballCapsuleSys->currentCapsuleSeals[placedSealIndex].tapped = FALSE;
    ballCapsuleSys->currentCapsuleSeals[placedSealIndex].type = 0;
    ballCapsuleSys->currentCapsuleSeals[placedSealIndex].x = 0;
    ballCapsuleSys->currentCapsuleSeals[placedSealIndex].y = 0;

    SetPlacedSealButtonPos(ballCapsuleSys->currentCapsuleSeals[placedSealIndex].rect, 0, 0);
}

void BallCapsuleSystem_SetPlacedSealOamMode(BallCapsuleSystem *ballCapsuleSys, int placedSealIndex)
{
    BOOL onBall = BallCapsuleSystem_IsPlacedSealOnBall(ballCapsuleSys, placedSealIndex);

    if (onBall == FALSE) {
        ManagedSprite_SetExplicitOamMode(ballCapsuleSys->currentCapsuleSeals[placedSealIndex].sealSprite, GX_OAM_MODE_XLU);
    } else {
        ManagedSprite_SetExplicitOamMode(ballCapsuleSys->currentCapsuleSeals[placedSealIndex].sealSprite, GX_OAM_MODE_NORMAL);
    }
}

BOOL BallCapsuleSystem_HasCurrentCapsuleBeenEdited(BallCapsuleSystem *ballCapsuleSys)
{
    int capsuleIndex;
    BallCapsule *capsule;

    // Get the currently selected capsule
    capsuleIndex = ballCapsuleSys->selectedCapsules[0];
    capsule = ballCapsuleSys->capsuleData[capsuleIndex].ballCapsule;

    // returns true if any mismatch between saved capsule and current edit
    for (int i = 0; i < SEALS_PER_CAPSULE; i++) {
        if ((ballCapsuleSys->currentCapsuleSeals[i].type != capsule->seals[i].type)
            || (ballCapsuleSys->currentCapsuleSeals[i].x != capsule->seals[i].x)
            || (ballCapsuleSys->currentCapsuleSeals[i].y != capsule->seals[i].y)) {
            return TRUE;
        }
    }

    return FALSE;
}

void BallCapsuleSystem_SetSealPosToSpritePos(BallCapsuleSystem *ballCapsuleSys, int placedSealIndex)
{
    s16 x, y;

    ManagedSprite_GetPositionXY(ballCapsuleSys->currentCapsuleSeals[placedSealIndex].sealSprite, &x, &y);

    ballCapsuleSys->currentCapsuleSeals[placedSealIndex].x = (u8)x;
    ballCapsuleSys->currentCapsuleSeals[placedSealIndex].y = (u8)y;
}

void BallCapsuleSystem_SaveSelectedCapsuleToSealCase(BallCapsuleSystem *ballCapsuleSys)
{
    BallCapsule capsule;

    for (int i = 0; i < SEALS_PER_CAPSULE; i++) {
        capsule.seals[i].type = ballCapsuleSys->currentCapsuleSeals[i].type;
        capsule.seals[i].x = ballCapsuleSys->currentCapsuleSeals[i].x;
        capsule.seals[i].y = ballCapsuleSys->currentCapsuleSeals[i].y;
    }

    SealCase_CopyCapsuleToSealCase(ballCapsuleSys->appData->sealCase, &capsule, ballCapsuleSys->selectedCapsules[0]);
}

void BallCapsuleSystem_SetPlacedSealsOnBallCapsule(BallCapsule *ballCapsule, BallCapsuleSystem *ballCapsuleSys)
{
    for (int i = 0; i < SEALS_PER_CAPSULE; i++) {
        ballCapsule->seals[i].type = ballCapsuleSys->currentCapsuleSeals[i].type;
        ballCapsule->seals[i].x = ballCapsuleSys->currentCapsuleSeals[i].x;
        ballCapsule->seals[i].y = ballCapsuleSys->currentCapsuleSeals[i].y;
    }
}
