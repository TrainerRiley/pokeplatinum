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

static void ov76_0223B15C(TouchScreenRect *rect, u8 param1, u8 param2)
{
    rect->rect.top = param2 - 10;
    rect->rect.left = param1 - 10;
    rect->rect.bottom = param2 + 10;
    rect->rect.right = param1 + 10;
}

static void ov76_0223B174(TouchScreenRect *rect)
{
    rect->rect.top = 0;
    rect->rect.left = 0;
    rect->rect.bottom = 192;
    rect->rect.right = 255;
}

void ov76_0223B184(TouchScreenRect *rect, ManagedSprite *param1, BOOL param2)
{
    s16 v0, v1;

    if (param1 == NULL) {
        return;
    }

    if (param2 == 1) {
        ManagedSprite_SetPositionXY(param1, gSystem.touchX, gSystem.touchY);
        ManagedSprite_GetPositionXY(param1, &v0, &v1);
        ov76_0223B174(rect);
    } else {
        ManagedSprite_GetPositionXY(param1, &v0, &v1);
        ov76_0223B15C(rect, v0, v1);
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
    BallCapsule *v1 = ballCapsuleSys->capsuleData[ballCapsuleSys->selectedCapsules[0]].ballCapsule;

    for (i = 0; i < SEALS_PER_CAPSULE; i++) {
        if (v1->seals[i].type != SEAL_DUMMY) {
            ballCapsuleSys->placedSeals[i].type = v1->seals[i].type;
            ballCapsuleSys->placedSeals[i].x = v1->seals[i].x;
            ballCapsuleSys->placedSeals[i].y = v1->seals[i].y;

            ov76_0223B52C(ballCapsuleSys, i);
            ov76_0223B184(ballCapsuleSys->placedSeals[i].rect, ballCapsuleSys->placedSeals[i].sealSprite, 0);

            ballCapsuleSys->placedSeals[i].tapped = TRUE;
        } else {
            ballCapsuleSys->placedSeals[i].tapped = FALSE;
            ballCapsuleSys->placedSeals[i].type = 0;
        }
    }
}

int ov76_0223B278(BallCapsuleSystem *ballCapsuleSys, u8 param1)
{
    int v0;
    u32 v1, v2;

    for (v0 = 0; v0 < SEALS_PER_CAPSULE; v0++) {
        if (ballCapsuleSys->placedSeals[v0].tapped == TRUE) {
            continue;
        }

        ballCapsuleSys->placedSeals[v0].type = ballCapsuleSys->sealCasePages.currentPageSeals[param1];
        ballCapsuleSys->placedSeals[v0].x = 190;
        ballCapsuleSys->placedSeals[v0].y = 70;

        TouchScreen_GetHoldState(&v1, &v2);

        ballCapsuleSys->placedSeals[v0].x = v1;
        ballCapsuleSys->placedSeals[v0].y = v2;

        ov76_0223B52C(ballCapsuleSys, v0);
        ov76_0223B314(ballCapsuleSys, v0);

        ballCapsuleSys->placedSeals[v0].tapped = TRUE;

        return v0;
    }

    return 0xFF;
}

BOOL ov76_0223B2F8(BallCapsuleSystem *ballCapsuleSys)
{
    int v0;
    u32 v1, v2;

    for (v0 = 0; v0 < SEALS_PER_CAPSULE; v0++) {
        if (ballCapsuleSys->placedSeals[v0].tapped == TRUE) {
            continue;
        }

        return 1;
    }

    return 0;
}

void ov76_0223B314(BallCapsuleSystem *ballCapsuleSys, u8 param1)
{
    int v0;

    for (v0 = 0; v0 < SEALS_PER_CAPSULE; v0++) {
        if (ballCapsuleSys->placedSeals[v0].tapped != TRUE) {
            continue;
        }

        if (v0 == param1) {
            ManagedSprite_SetPriority(ballCapsuleSys->placedSeals[v0].sealSprite, 0);
        } else {
            int v1 = ManagedSprite_GetPriority(ballCapsuleSys->placedSeals[v0].sealSprite);

            if (v1 == 0) {
                ManagedSprite_SetPriority(ballCapsuleSys->placedSeals[v0].sealSprite, v1 + 1);
            } else {
                ManagedSprite_SetPriority(ballCapsuleSys->placedSeals[v0].sealSprite, v1 + 2);
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

    int v0 = sub_02098140(param1);
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

    v0 = sub_02098140(v2->type);
    v1 = (param1 + 20000);

    v3.resources[0] = v1;
    v3.resources[1] = 21000 + 293;
    v3.resources[2] = 22000 + 93;
    v3.resources[3] = 23000 + 1;
    v2->sealSprite = SpriteSystem_NewSprite(v4, v5, &v3);

    ManagedSprite_TickFrame(v2->sealSprite);

    return 1;
}

BOOL ov76_0223B5C4(BallCapsuleSystem *ballCapsuleSys, u32 param1, u8 param2)
{
    if (ballCapsuleSys->placedSeals[param2].tapped == FALSE) {
        return 1;
    }

    switch (param1) {
    case 0:
        if (ballCapsuleSys->ballCapsuleEditor.unk_00 == 0xFF) {
            ballCapsuleSys->ballCapsuleEditor.unk_00 = param2;
            ov76_0223B314(ballCapsuleSys, param2);
        }
        break;
    case 2: {
        s16 v0, v1;

        if (ballCapsuleSys->ballCapsuleEditor.unk_00 == param2) {
            ov76_0223B184(ballCapsuleSys->placedSeals[param2].rect, ballCapsuleSys->placedSeals[param2].sealSprite, 1);
            ov76_0223B758(ballCapsuleSys, param2);
            ov76_0223B7D4(ballCapsuleSys, param2);
        }
    } break;
    case 3:
    case 1: {
        BOOL v2;

        if (ballCapsuleSys->ballCapsuleEditor.unk_00 == 0xFF) {
            v2 = ov76_0223B6C4(ballCapsuleSys, param2);
            ov76_0223B184(ballCapsuleSys->placedSeals[param2].rect, ballCapsuleSys->placedSeals[param2].sealSprite, 0);

            if (v2 == 0) {
                ov76_0223B704(ballCapsuleSys, param2);
            }

            ballCapsuleSys->ballCapsuleEditor.unk_00 = 0xFF;
        }
    } break;
    default:
        GF_ASSERT(0);
        break;
    }

    return 1;
}

void ov76_0223B678(BallCapsuleSystem *ballCapsuleSys)
{
    for (int i = 0; i < SEALS_PER_CAPSULE; i++) {
        if (ballCapsuleSys->placedSeals[i].tapped != TRUE) {
            continue;
        }

        ov76_0223B704(ballCapsuleSys, i);
    }
}

void ov76_0223B69C(BallCapsuleSystem *ballCapsuleSys, int param1)
{
    for (int i = 0; i < SEALS_PER_CAPSULE; i++) {
        if (ballCapsuleSys->placedSeals[i].tapped != TRUE) {
            continue;
        }

        ManagedSprite_SetDrawFlag(ballCapsuleSys->placedSeals[i].sealSprite, param1);
    }
}

BOOL ov76_0223B6C4(BallCapsuleSystem *ballCapsuleSys, int param1)
{
    int v0;
    s16 x, y;

    if (ballCapsuleSys->placedSeals[param1].tapped == FALSE) {
        return 1;
    }

    ManagedSprite_GetPositionXY(ballCapsuleSys->placedSeals[param1].sealSprite, &x, &y);
    v0 = ov12_02237E54(x, y, 190, 70);

    if (v0 > 60) {
        return 0;
    }

    return TRUE;
}

void ov76_0223B704(BallCapsuleSystem *ballCapsuleSys, int param1)
{
    int v0;
    int v1;

    v0 = sub_02098140(ballCapsuleSys->placedSeals[param1].type);
    v1 = (param1 + 20000);

    SpriteManager_UnloadCharObjById(ballCapsuleSys->ballCapsuleEditor.spriteMan, v1);
    Sprite_DeleteAndFreeResources(ballCapsuleSys->placedSeals[param1].sealSprite);

    ballCapsuleSys->placedSeals[param1].sealSprite = NULL;
    ballCapsuleSys->placedSeals[param1].tapped = FALSE;
    ballCapsuleSys->placedSeals[param1].type = 0;
    ballCapsuleSys->placedSeals[param1].x = 0;
    ballCapsuleSys->placedSeals[param1].y = 0;

    ov76_0223B15C(ballCapsuleSys->placedSeals[param1].rect, 0, 0);
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

BOOL ov76_0223B78C(BallCapsuleSystem *ballCapsuleSys)
{
    int i;
    int v1;
    BallCapsule *v2;

    v1 = ballCapsuleSys->selectedCapsules[0];
    v2 = ballCapsuleSys->capsuleData[v1].ballCapsule;

    for (i = 0; i < SEALS_PER_CAPSULE; i++) {
        if ((ballCapsuleSys->placedSeals[i].type != v2->seals[i].type)
            || (ballCapsuleSys->placedSeals[i].x != v2->seals[i].x)
            || (ballCapsuleSys->placedSeals[i].y != v2->seals[i].y)) {
            return 1;
        }
    }

    return 0;
}

void ov76_0223B7D4(BallCapsuleSystem *ballCapsuleSys, int param1)
{
    s16 v0, v1;

    ManagedSprite_GetPositionXY(ballCapsuleSys->placedSeals[param1].sealSprite, &v0, &v1);

    ballCapsuleSys->placedSeals[param1].x = (u8)v0;
    ballCapsuleSys->placedSeals[param1].y = (u8)v1;
}

void ov76_0223B808(BallCapsuleSystem *ballCapsuleSys)
{
    int i;
    BallCapsule capsule;

    for (i = 0; i < SEALS_PER_CAPSULE; i++) {
        capsule.seals[i].type = ballCapsuleSys->placedSeals[i].type;
        capsule.seals[i].x = ballCapsuleSys->placedSeals[i].x;
        capsule.seals[i].y = ballCapsuleSys->placedSeals[i].y;
    }

    SealCase_CopyCapsuleFromId(ballCapsuleSys->appData->sealCase, &capsule, ballCapsuleSys->selectedCapsules[0]);
}

void ov76_0223B848(BallCapsule *ballCapsule, BallCapsuleSystem *ballCapsuleSys)
{
    int i;

    for (i = 0; i < SEALS_PER_CAPSULE; i++) {
        ballCapsule->seals[i].type = ballCapsuleSys->placedSeals[i].type;
        ballCapsule->seals[i].x = ballCapsuleSys->placedSeals[i].x;
        ballCapsule->seals[i].y = ballCapsuleSys->placedSeals[i].y;
    }
}
