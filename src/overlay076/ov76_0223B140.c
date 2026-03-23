#include "overlay076/ov76_0223B140.h"

#include <nitro.h>
#include <nitro/sinit.h>
#include <string.h>

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
    Overlay_LoadByID(FS_OVERLAY_ID(overlay11), 2);
    Overlay_LoadByID(FS_OVERLAY_ID(battle_anim), 2);
}

// 20x20 rect centered at (x,y)
static void SetPlacedSealButtonPos(TouchScreenRect *rect, u8 x, u8 y)
{
    rect->rect.top = y - 10;
    rect->rect.left = x - 10;
    rect->rect.bottom = y + 10;
    rect->rect.right = x + 10;
}

static void SetToFullScreenDimensions(TouchScreenRect *rect)
{
    rect->rect.top = 0;
    rect->rect.left = 0;
    rect->rect.bottom = 192;
    rect->rect.right = 255;
}

void ov76_0223B184(TouchScreenRect *rect, ManagedSprite *sprite, BOOL param2)
{
    if (sprite == NULL) {
        return;
    }

    s16 x, y;
    if (param2 == TRUE) {
        ManagedSprite_SetPositionXY(sprite, gSystem.touchX, gSystem.touchY);
        ManagedSprite_GetPositionXY(sprite, &x, &y);
        SetToFullScreenDimensions(rect);
    } else {
        ManagedSprite_GetPositionXY(sprite, &x, &y);
        SetPlacedSealButtonPos(rect, x, y);
    }
}

void ov76_0223B1E0(BallCapsuleSystem *ballCapsuleSys)
{
    for (int i = 0; i < SEALS_PER_CAPSULE; i++) {
        if (ballCapsuleSys->placedSeals[i].sealSprite == NULL) {
            continue;
        }

        ManagedSprite_OffsetPositionXY(ballCapsuleSys->placedSeals[i].sealSprite, -(+7 * 8), -(-2 * 8));
    }
}

void ov76_0223B208(BallCapsuleSystem *ballCapsuleSys)
{
    int i;
    BallCapsule *capsule = ballCapsuleSys->capsuleData[ballCapsuleSys->selectedCapsules[0]].ballCapsule;

    for (i = 0; i < SEALS_PER_CAPSULE; i++) {
        if (capsule->seals[i].type != SEAL_DUMMY) {
            ballCapsuleSys->placedSeals[i].type = capsule->seals[i].type;
            ballCapsuleSys->placedSeals[i].x = capsule->seals[i].x;
            ballCapsuleSys->placedSeals[i].y = capsule->seals[i].y;

            ov76_0223B52C(ballCapsuleSys, i);
            ov76_0223B184(ballCapsuleSys->placedSeals[i].rect, ballCapsuleSys->placedSeals[i].sealSprite, 0);

            ballCapsuleSys->placedSeals[i].tapped = TRUE;
        } else {
            ballCapsuleSys->placedSeals[i].tapped = FALSE;
            ballCapsuleSys->placedSeals[i].type = 0;
        }
    }
}

int BallCapsuleSystem_AddPlacedSeal(BallCapsuleSystem *ballCapsuleSys, u8 sealPageIndex)
{
    for (int i = 0; i < SEALS_PER_CAPSULE; i++) {
        // Find first untapped seal
        if (ballCapsuleSys->placedSeals[i].tapped == TRUE) {
            continue;
        }

        ballCapsuleSys->placedSeals[i].type = ballCapsuleSys->sealCasePages.currentPageSeals[sealPageIndex];
        ballCapsuleSys->placedSeals[i].x = 190;
        ballCapsuleSys->placedSeals[i].y = 70;

        u32 x, y;
        TouchScreen_GetHoldState(&x, &y);

        ballCapsuleSys->placedSeals[i].x = x;
        ballCapsuleSys->placedSeals[i].y = y;

        ov76_0223B52C(ballCapsuleSys, i);
        BallCapsuleSystem_IncreasePlacedSealPriority(ballCapsuleSys, i);

        ballCapsuleSys->placedSeals[i].tapped = TRUE;

        return i;
    }

    return 0xFF;
}

BOOL BallCapsuleSystem_AnyPlacedSealTapped(BallCapsuleSystem *ballCapsuleSys)
{
    for (int i = 0; i < SEALS_PER_CAPSULE; i++) {
        if (ballCapsuleSys->placedSeals[i].tapped == TRUE) {
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
        if (ballCapsuleSys->placedSeals[i].tapped != TRUE) {
            continue;
        }

        if (i == placedSealIndex) {
            ManagedSprite_SetPriority(ballCapsuleSys->placedSeals[i].sealSprite, 0);
        } else {
            int priority = ManagedSprite_GetPriority(ballCapsuleSys->placedSeals[i].sealSprite);

            if (priority == 0) {
                ManagedSprite_SetPriority(ballCapsuleSys->placedSeals[i].sealSprite, priority + 1);
            } else {
                ManagedSprite_SetPriority(ballCapsuleSys->placedSeals[i].sealSprite, priority + 2);
            }
        }
    }
}

void ov76_0223B36C(BallCapsuleSystem *ballCapsuleSys, u8 param1, u8 param2)
{
    SpriteSystem *v2 = ballCapsuleSys->ballCapsuleEditor.spriteSys;
    SpriteManager *v3 = ballCapsuleSys->ballCapsuleEditor.spriteMan;
    PaletteData *v4 = ballCapsuleSys->ballCapsuleEditor.paletteData;

    SpriteSystem_LoadPaletteBuffer(v4, 3, v2, v3, 91, 293, 0, 1, NNS_G2D_VRAM_TYPE_2DSUB, 21000 + 293);

    int v0 = SealData_GetCharID(param1);
    int v1 = (param2 + 20000);

    SpriteSystem_LoadCharResObj(v2, v3, NARC_INDEX_APPLICATION__CUSTOM_BALL__DATA__CB_DATA, v0, TRUE, NNS_G2D_VRAM_TYPE_2DSUB, v1);
    SpriteSystem_LoadCellResObj(v2, v3, NARC_INDEX_APPLICATION__CUSTOM_BALL__DATA__CB_DATA, 93, TRUE, 22000 + 93);
    SpriteSystem_LoadAnimResObj(v2, v3, NARC_INDEX_APPLICATION__CUSTOM_BALL__DATA__CB_DATA, 1, TRUE, 23000 + 1);
}

void ov76_0223B400(BallCapsuleSystem *ballCapsuleSys)
{
    int i, j;
    int v2;
    int v3[SEALS_PER_CAPSULE];
    int v4[SEALS_PER_CAPSULE];

    if (ballCapsuleSys->ballCapsuleEditor.unk_18C == 0) {
        return;
    }

    ballCapsuleSys->ballCapsuleEditor.unk_18C = 0;

    v2 = 0;

    for (i = 0; i < SEALS_PER_CAPSULE; i++) {
        v3[i] = 0xFF;
        v4[i] = 0xFF;

        if (ballCapsuleSys->placedSeals[i].sealSprite != NULL) {
            v3[i] = ManagedSprite_GetPriority(ballCapsuleSys->placedSeals[i].sealSprite);
            v4[i] = i;
            v2++;
        }
    }

    int v5;
    int v6;

    for (i = 0; i < SEALS_PER_CAPSULE - 1; i++) {
        for (j = SEALS_PER_CAPSULE - 1; j > i; j--) {
            if (v3[j - 1] >= v3[j]) {
                v5 = v3[j];
                v6 = v4[j];
                v3[j] = v3[j - 1];
                v4[j] = v4[j - 1];
                v3[j - 1] = v5;
                v4[j - 1] = v6;
            }
        }
    }

    BallCapsule v7;
    BallCapsule v8;

    for (i = 0; i < SEALS_PER_CAPSULE; i++) {
        v7.seals[i].type = ballCapsuleSys->placedSeals[i].type;
        v7.seals[i].x = ballCapsuleSys->placedSeals[i].x;
        v7.seals[i].y = ballCapsuleSys->placedSeals[i].y;
    }

    v8 = v7;

    for (i = 0; i < SEALS_PER_CAPSULE; i++) {
        if (v4[i] == 0xFF) {
            ballCapsuleSys->placedSeals[i].type = 0;
            ballCapsuleSys->placedSeals[i].x = 0;
            ballCapsuleSys->placedSeals[i].y = 0;
            continue;
        }

        ballCapsuleSys->placedSeals[i].type = v8.seals[v4[i]].type;
        ballCapsuleSys->placedSeals[i].x = v8.seals[v4[i]].x;
        ballCapsuleSys->placedSeals[i].y = v8.seals[v4[i]].y;

        if (ballCapsuleSys->placedSeals[v4[i]].sealSprite == NULL) {
            continue;
        }

        ManagedSprite_SetPriority(ballCapsuleSys->placedSeals[v4[i]].sealSprite, i);
    }
}

BOOL ov76_0223B52C(BallCapsuleSystem *ballCapsuleSys, u8 param1)
{
    int v0;
    int v1;
    PlacedSeal *v2;
    SpriteTemplate v3;
    SpriteSystem *v4 = ballCapsuleSys->ballCapsuleEditor.spriteSys;
    SpriteManager *v5 = ballCapsuleSys->ballCapsuleEditor.spriteMan;
    PaletteData *v6 = ballCapsuleSys->ballCapsuleEditor.paletteData;
    v2 = &ballCapsuleSys->placedSeals[param1];

    if (v2->tapped == TRUE) {
        GF_ASSERT(0);
        return 0;
    }

    ov76_0223B36C(ballCapsuleSys, v2->type, param1);

    v3.x = v2->x;
    v3.y = v2->y;
    v3.z = 0;
    v3.animIdx = 0;
    v3.plttIdx = 0;
    v3.vramTransfer = FALSE;
    v3.priority = 0;
    v3.vramType = NNS_G2D_VRAM_TYPE_2DSUB;
    v3.bgPriority = 1;
    v3.resources[4] = SPRITE_RESOURCE_NONE;
    v3.resources[5] = SPRITE_RESOURCE_NONE;

    v0 = SealData_GetCharID(v2->type);
    v1 = (param1 + 20000);

    v3.resources[0] = v1;
    v3.resources[1] = 21000 + 293;
    v3.resources[2] = 22000 + 93;
    v3.resources[3] = 23000 + 1;
    v2->sealSprite = SpriteSystem_NewSprite(v4, v5, &v3);

    ManagedSprite_TickFrame(v2->sealSprite);

    return 1;
}

BOOL ov76_0223B5C4(BallCapsuleSystem *ballCapsuleSys, enum TouchScreenButtonState touchScreenState, u8 placedSealIndex)
{
    if (ballCapsuleSys->placedSeals[placedSealIndex].tapped == FALSE) {
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
        s16 v0, v1;

        if (ballCapsuleSys->ballCapsuleEditor.unk_00 == placedSealIndex) {
            ov76_0223B184(ballCapsuleSys->placedSeals[placedSealIndex].rect, ballCapsuleSys->placedSeals[placedSealIndex].sealSprite, 1);
            ov76_0223B758(ballCapsuleSys, placedSealIndex);
            BallCapsuleSystem_SetSealPosToSpritePos(ballCapsuleSys, placedSealIndex);
        }
    } break;
    case TOUCH_BUTTON_HELD_OUT_OF_BOUNDS:
    case TOUCH_BUTTON_RELEASED: {
        BOOL v2;

        if (ballCapsuleSys->ballCapsuleEditor.unk_00 == 0xFF) {
            v2 = ov76_0223B6C4(ballCapsuleSys, placedSealIndex);
            ov76_0223B184(ballCapsuleSys->placedSeals[placedSealIndex].rect, ballCapsuleSys->placedSeals[placedSealIndex].sealSprite, 0);

            if (v2 == 0) {
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
        if (ballCapsuleSys->placedSeals[i].tapped != TRUE) {
            continue;
        }

        BallCapsuleSystem_RemovePlacedSeal(ballCapsuleSys, i);
    }
}

void BallCapsuleSystem_SetDrawFlagOnTappedSeal(BallCapsuleSystem *ballCapsuleSys, int index)
{
    for (int i = 0; i < SEALS_PER_CAPSULE; i++) {
        if (ballCapsuleSys->placedSeals[i].tapped != TRUE) {
            continue;
        }

        ManagedSprite_SetDrawFlag(ballCapsuleSys->placedSeals[i].sealSprite, index);
    }
}

BOOL ov76_0223B6C4(BallCapsuleSystem *ballCapsuleSys, int placedSealIndex)
{
    int v0;
    s16 x, y;

    if (ballCapsuleSys->placedSeals[placedSealIndex].tapped == FALSE) {
        return TRUE;
    }

    ManagedSprite_GetPositionXY(ballCapsuleSys->placedSeals[placedSealIndex].sealSprite, &x, &y);
    v0 = ov12_02237E54(x, y, 190, 70);

    if (v0 > 60) {
        return FALSE;
    }

    return TRUE;
}

void BallCapsuleSystem_RemovePlacedSeal(BallCapsuleSystem *ballCapsuleSys, int placedIndex)
{
    // unused
    int charID = SealData_GetCharID(ballCapsuleSys->placedSeals[placedIndex].type);

    int resourceID = (placedIndex + 20000);

    SpriteManager_UnloadCharObjById(ballCapsuleSys->ballCapsuleEditor.spriteMan, resourceID);
    Sprite_DeleteAndFreeResources(ballCapsuleSys->placedSeals[placedIndex].sealSprite);

    ballCapsuleSys->placedSeals[placedIndex].sealSprite = NULL;
    ballCapsuleSys->placedSeals[placedIndex].tapped = FALSE;
    ballCapsuleSys->placedSeals[placedIndex].type = 0;
    ballCapsuleSys->placedSeals[placedIndex].x = 0;
    ballCapsuleSys->placedSeals[placedIndex].y = 0;

    SetPlacedSealButtonPos(ballCapsuleSys->placedSeals[placedIndex].rect, 0, 0);
}

void ov76_0223B758(BallCapsuleSystem *ballCapsuleSys, int param1)
{
    BOOL v0 = ov76_0223B6C4(ballCapsuleSys, param1);

    if (v0 == 0) {
        ManagedSprite_SetExplicitOamMode(ballCapsuleSys->placedSeals[param1].sealSprite, GX_OAM_MODE_XLU);
    } else {
        ManagedSprite_SetExplicitOamMode(ballCapsuleSys->placedSeals[param1].sealSprite, GX_OAM_MODE_NORMAL);
    }
}

BOOL BallCapsuleSystem_HasCurrentCapsuleBeenEdited(BallCapsuleSystem *ballCapsuleSys)
{
    int capsuleIndex;
    BallCapsule *capsule;

    // Get the currently selected capsule
    capsuleIndex = ballCapsuleSys->selectedCapsules[0];
    capsule = ballCapsuleSys->capsuleData[capsuleIndex].ballCapsule;

    for (int i = 0; i < SEALS_PER_CAPSULE; i++) {
        if ((ballCapsuleSys->placedSeals[i].type != capsule->seals[i].type)
            || (ballCapsuleSys->placedSeals[i].x != capsule->seals[i].x)
            || (ballCapsuleSys->placedSeals[i].y != capsule->seals[i].y)) {
            return TRUE;
        }
    }

    return FALSE;
}

void BallCapsuleSystem_SetSealPosToSpritePos(BallCapsuleSystem *ballCapsuleSys, int placedSealIndex)
{
    s16 x, y;

    ManagedSprite_GetPositionXY(ballCapsuleSys->placedSeals[placedSealIndex].sealSprite, &x, &y);

    ballCapsuleSys->placedSeals[placedSealIndex].x = (u8)x;
    ballCapsuleSys->placedSeals[placedSealIndex].y = (u8)y;
}

void BallCapsuleSystem_SaveSelectedCapsuleToSealCase(BallCapsuleSystem *ballCapsuleSys)
{
    BallCapsule capsule;

    for (int i = 0; i < SEALS_PER_CAPSULE; i++) {
        capsule.seals[i].type = ballCapsuleSys->placedSeals[i].type;
        capsule.seals[i].x = ballCapsuleSys->placedSeals[i].x;
        capsule.seals[i].y = ballCapsuleSys->placedSeals[i].y;
    }

    SealCase_CopyCapsuleToSealCase(ballCapsuleSys->appData->sealCase, &capsule, ballCapsuleSys->selectedCapsules[0]);
}

void BallCapsuleSystem_SetPlacedSealsOnBallCapsule(BallCapsule *ballCapsule, BallCapsuleSystem *ballCapsuleSys)
{
    for (int i = 0; i < SEALS_PER_CAPSULE; i++) {
        ballCapsule->seals[i].type = ballCapsuleSys->placedSeals[i].type;
        ballCapsule->seals[i].x = ballCapsuleSys->placedSeals[i].x;
        ballCapsule->seals[i].y = ballCapsuleSys->placedSeals[i].y;
    }
}
