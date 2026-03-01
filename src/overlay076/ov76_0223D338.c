#include "overlay076/ov76_0223D338.h"

#include <nitro.h>
#include <string.h>

#include "generated/trainer_score_events.h"

#include "struct_decls/font_oam.h"
#include "struct_defs/seal_case.h"
#include "struct_defs/sprite_animation_frame.h"
#include "struct_defs/struct_02015958.h"

#include "battle_anim/ov12_02235E94.h"
#include "battle_anim/struct_ov12_02236030.h"
#include "battle_anim/struct_ov12_02237728.h"
#include "overlay076/const_ov76_0223EF3C.h"
#include "overlay076/funcptr_ov76_0223D674.h"
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

typedef struct {
    int unk_00;
    ManagedSprite *unk_04;
    FontOAM *unk_08;
} UnkStruct_ov76_0223D9AC;

static BOOL ov76_0223D674(BallCapsuleSystem *ballCapsuleSys);
static BOOL ov76_0223DF94(BallCapsuleSystem *ballCapsuleSys);
static BOOL ov76_0223E8A4(BallCapsuleSystem *ballCapsuleSys);
static BOOL ov76_0223E950(BallCapsuleSystem *ballCapsuleSys);
static BOOL ov76_0223E9C4(BallCapsuleSystem *ballCapsuleSys);
static BOOL ov76_0223EB1C(BallCapsuleSystem *ballCapsuleSys);

void ov76_0223D338(BallCapsuleSystem *ballCapsuleSys)
{
    BallCapsule_Copy(ballCapsuleSys->unk_04[ballCapsuleSys->unk_3C4[0]].unk_04, &ballCapsuleSys->unk_68);
}

void ov76_0223D350(BallCapsuleSystem *ballCapsuleSys)
{
    BallCapsule_Copy(&ballCapsuleSys->unk_68, ballCapsuleSys->unk_04[ballCapsuleSys->unk_3C4[0]].unk_04);
}

void ov76_0223D368(BallCapsuleSystem *ballCapsuleSys)
{
    int v0;

    for (v0 = 0; v0 < (80 + 1); v0++) {
        ballCapsuleSys->unk_80[v0] = SealCase_GetSealCount(ballCapsuleSys->unk_64, v0);
    }
}

void ov76_0223D384(BallCapsuleSystem *ballCapsuleSys)
{
    int i;

    for (i = 0; i < (80 + 1); i++) {
        SealCase_SetSealQuantity(ballCapsuleSys->unk_64, i, ballCapsuleSys->unk_80[i]);
    }
}

void ov76_0223D3A0(void)
{
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_NONE, GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2, 11, 7);
    G2S_SetBlendAlpha(GX_BLEND_PLANEMASK_NONE, GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3, 7, 8);
}

void ov76_0223D3CC(BallCapsuleSystem *ballCapsuleSys)
{
    int v0;
    int v1;
    Pokemon *v2;

    if (ballCapsuleSys->unk_04[ballCapsuleSys->unk_3C4[0]].unk_00 != 0xff) {
        v2 = ballCapsuleSys->unk_00->unk_04[ballCapsuleSys->unk_04[ballCapsuleSys->unk_3C4[0]].unk_00];
        Pokemon_Copy(v2, ballCapsuleSys->unk_428);

        return;
    }

    for (v0 = 0; v0 < ballCapsuleSys->unk_00->unk_00; v0++) {
        v2 = ballCapsuleSys->unk_00->unk_04[v0];
        v1 = Pokemon_GetValue(v2, MON_DATA_IS_EGG, NULL);

        if (v1 != 0) {
            continue;
        }

        Pokemon_Copy(v2, ballCapsuleSys->unk_428);
        break;
    }
}

int ov76_0223D430(BallCapsule *param0)
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

    v0 = ballCapsuleSys->unk_264[param1].unk_00;
    v1 = ballCapsuleSys->unk_264[param1].unk_04;

    if ((v0 == 0xff) && (v1 == 0)) {
        v2 = 0;
    } else if ((v0 == 0xff) && (v1 == 1)) {
        v2 = 1;
    } else if ((v0 != 0xff) && (v1 == 1)) {
        v2 = 2;
    } else {
        v2 = 3;
    }

    return v2;
}

static void ov76_0223D494(BallCapsuleSystem *ballCapsuleSys, int param1, int param2, int param3)
{
    if (param2 == 0xff) {
        ballCapsuleSys->unk_3D4 = ballCapsuleSys->unk_3D8;
    } else {
        ballCapsuleSys->unk_3D4 = param2;
    }

    ballCapsuleSys->unk_3CC = param1;
    ballCapsuleSys->unk_3D8 = param3;
    ballCapsuleSys->unk_3DC = 0;
}

static int ov76_0223D4C4(BallCapsuleSystem *ballCapsuleSys)
{
    Window_EraseStandardFrame(&ballCapsuleSys->unk_D4.unk_18[1], 1);
    Window_ClearAndCopyToVRAM(&ballCapsuleSys->unk_D4.unk_18[1]);
    Menu_Free(ballCapsuleSys->unk_D4.unk_CC, NULL);
    StringList_Free(ballCapsuleSys->unk_D4.unk_C8);
    Window_Remove(&ballCapsuleSys->unk_D4.unk_18[1]);

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
    ov76_0223C7E0(ballCapsuleSys);
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
    ov76_0223D674,
    ov76_0223DF94,
    ov76_0223E8A4,
    ov76_0223E950,
    ov76_0223E9C4,
    ov76_0223EB1C,
};

BOOL ov76_0223D550(BallCapsuleSystem *ballCapsuleSys)
{
    BOOL v0 = Unk_ov76_0223EE04[ballCapsuleSys->unk_3CC](ballCapsuleSys);
    SpriteSystem_DrawSprites(ballCapsuleSys->unk_D4.unk_0C);

    return v0;
}

static BOOL ov76_0223D574(int *param0)
{
    int *v0 = param0;

    if (gSystem.pressedKeysRepeatable & PAD_KEY_RIGHT) {
        (*v0)++;
        (*v0) %= 12;
    } else if (gSystem.pressedKeysRepeatable & PAD_KEY_LEFT) {
        if (*v0 > 0) {
            (*v0)--;
        } else {
            *v0 = 12 - 1;
        }
    } else if (gSystem.pressedKeysRepeatable & PAD_KEY_UP) {
        if ((*v0 / 4) != 0) {
            (*v0) -= 4;
            (*v0) %= 12;
        } else {
            return 0;
        }
    } else if (gSystem.pressedKeysRepeatable & PAD_KEY_DOWN) {
        if ((*v0 / 4) != (3 - 1)) {
            (*v0) += 4;
            (*v0) %= 12;
        } else {
            return 0;
        }
    } else {
        return 0;
    }

    return 1;
}

static void ov76_0223D600(BallCapsuleSystem *ballCapsuleSys, int param1, BOOL param2)
{
    s16 v0, v1;
    int v2 = 0;

    if (param1 == 0) {
        v2 = 1;
    }

    ov76_0223C0EC(ballCapsuleSys->unk_3C4[param1], &v0, &v1);
    ManagedSprite_SetPositionXY(ballCapsuleSys->unk_2F4[param1], v0, v1);

    if (param2 == 1) {
        ballCapsuleSys->unk_3C4[v2] = ballCapsuleSys->unk_3C4[param1];
        ov76_0223C0EC(ballCapsuleSys->unk_3C4[v2], &v0, &v1);
        ManagedSprite_SetPositionXY(ballCapsuleSys->unk_2F4[v2], v0, v1);
    }
}

static BOOL ov76_0223D674(BallCapsuleSystem *ballCapsuleSys)
{
    switch (ballCapsuleSys->unk_3D4) {
    case 0: {
        NARC *v0;

        v0 = NARC_ctor(NARC_INDEX_APPLICATION__CUSTOM_BALL__DATA__CB_DATA, HEAP_ID_53);

        ov76_0223C110(ballCapsuleSys);
        ov76_0223C188(ballCapsuleSys);
        ov76_0223C288(ballCapsuleSys);
        ov76_0223CE84(ballCapsuleSys, v0);
        ov76_0223CF24(ballCapsuleSys, v0);
        ov76_0223CF88(ballCapsuleSys, v0);
        ov76_0223C354(ballCapsuleSys);
        ov76_0223C61C(ballCapsuleSys, v0);
        ov76_0223CA98(ballCapsuleSys->unk_D4.unk_10, &ballCapsuleSys->unk_D4.unk_18[0], 1, 2, 21, 27, 2, 0 + ((1 + (18 + 12)) + 9));
        ov76_0223B208(ballCapsuleSys);
        ov76_0223B69C(ballCapsuleSys, 1);
        ov76_0223B1E0(ballCapsuleSys);
        ov76_0223CFEC(ballCapsuleSys, v0);
        ov76_0223D16C(ballCapsuleSys);
        ov76_0223C438(ballCapsuleSys, v0);
        ov76_0223C4AC(ballCapsuleSys);
        ov76_0223D338(ballCapsuleSys);
        ov76_0223D368(ballCapsuleSys);
        ov76_0223BA90(ballCapsuleSys, ballCapsuleSys->unk_418.unk_00);
        ov76_0223BB04(ballCapsuleSys);
        ov76_0223BBAC(ballCapsuleSys);
        ov76_0223D2F4(ballCapsuleSys, 0);
        ov76_0223C568(ballCapsuleSys, 0);
        ov76_0223B940(ballCapsuleSys);
        ov76_0223B96C(ballCapsuleSys, 0);
        ov76_0223CA30(&ballCapsuleSys->unk_D4.unk_18[0], 7);
        ov76_0223CE2C();
        ov76_0223DCB8(ballCapsuleSys, 0);

        NARC_dtor(v0);
    }
        ballCapsuleSys->unk_3D4++;
        break;

    case 1:
        ov76_0223CE44();
        ballCapsuleSys->unk_3D4++;
        break;

    case 2:
        if (IsScreenFadeDone() != 1) {
            break;
        }

        ballCapsuleSys->unk_3D4++;

    case 3: {
        BOOL v1;
        int v2;
        int v3;

        if (TouchScreen_Tapped() == 1) {
            ov76_0223D4FC(ballCapsuleSys);
            break;
        }

        v2 = ballCapsuleSys->unk_3C4[0];
        v1 = ov76_0223D574(&(ballCapsuleSys->unk_3C4[0]));
        v3 = ballCapsuleSys->unk_3C4[0];
        ballCapsuleSys->unk_3C4[0] = v2;

        if (v1 == 1) {
            ov76_0223B400(ballCapsuleSys);
            ov76_0223B808(ballCapsuleSys);
            ov76_0223C7E0(ballCapsuleSys);

            ballCapsuleSys->unk_3C4[0] = v3;

            ov76_0223D600(ballCapsuleSys, 0, 1);
            ov76_0223B678(ballCapsuleSys);
            ov76_0223B208(ballCapsuleSys);
            ov76_0223B69C(ballCapsuleSys, 1);
            ov76_0223B1E0(ballCapsuleSys);

            Sound_PlayEffect(SEQ_SE_CONFIRM);
        } else if (gSystem.pressedKeys & PAD_BUTTON_A) {
            ballCapsuleSys->unk_3D4++;
            ov76_0223CA30(&ballCapsuleSys->unk_D4.unk_18[0], 8);
            ov76_0223BF74(ballCapsuleSys->unk_D4.unk_10, &ballCapsuleSys->unk_D4.unk_18[1], 1, ballCapsuleSys, ballCapsuleSys->unk_3C4[0]);
            Sound_PlayEffect(SEQ_SE_CONFIRM);
        } else if (gSystem.pressedKeys & PAD_BUTTON_B) {
            ballCapsuleSys->unk_3D4 = 5;
            Sound_PlayEffect(SEQ_SE_DP_DECIDE);
        }
    } break;
    case 4: {
        UnkFuncPtr_ov76_0223D674 v4;
        u32 v5 = Menu_ProcessInput(ballCapsuleSys->unk_D4.unk_CC);

        switch (v5) {
        case 0xfffffffe:
            ov76_0223D4C4(ballCapsuleSys);
            ov76_0223CA30(&ballCapsuleSys->unk_D4.unk_18[0], 7);
            ballCapsuleSys->unk_3D4 = 3;
            break;
        case 0xffffffff:
            break;
        default:
            v4 = (UnkFuncPtr_ov76_0223D674)v5;

            if (v4 != NULL) {
                int v6 = v4(ballCapsuleSys);

                if (v6 != 1) {
                    ov76_0223D4C4(ballCapsuleSys);
                    break;
                }

                ov76_0223CA30(&ballCapsuleSys->unk_D4.unk_18[0], 7);
                ballCapsuleSys->unk_3D4 = 3;
            }
            break;
        }
    } break;
    case 5:
        ov76_0223CE64();
        ballCapsuleSys->unk_3D4++;
        break;
    case 6:
        if (IsScreenFadeDone() != 1) {
            break;
        }
        Window_Remove(&ballCapsuleSys->unk_D4.unk_18[0]);
        ov76_0223C8BC(ballCapsuleSys);
        ov76_0223C32C(ballCapsuleSys);
        ov76_0223D31C(ballCapsuleSys);
        ov76_0223C588(ballCapsuleSys);
        ov76_0223BC70(ballCapsuleSys);
        sub_02097F30(ballCapsuleSys->unk_00, 0);
        return 0;
    }

    ov76_0223C304(ballCapsuleSys);
    ov76_0223C88C(ballCapsuleSys);

    return 1;
}

void ov76_0223D94C(ManagedSprite *param0, int param1)
{
    switch (param1) {
    case 0:
        ManagedSprite_SetAnimationFrame(param0, 1);
        break;
    case 2:
        ManagedSprite_SetAnimationFrame(param0, 2);
        break;
    case 1:
    case 3:
        ManagedSprite_SetAnimationFrame(param0, 0);
        break;
    default:
        GF_ASSERT(0);
        break;
    }
}

static void ov76_0223D984(FontOAM *param0, int param1, int param2)
{
    int v0;
    int v1;

    if (param0 != NULL) {
        FontOAM_GetXY(param0, &v0, &v1);
        FontOAM_SetXY(param0, v0 + param1, v1 + param2);
    }
}

static void ov76_0223D9AC(SysTask *param0, void *param1)
{
    UnkStruct_ov76_0223D9AC *v0 = param1;

    switch (v0->unk_00) {
    case 3:
        ov76_0223D984(v0->unk_08, 0, -1);
        ManagedSprite_SetAnimationFrame(v0->unk_04, 2);
        v0->unk_00++;
        break;
    case 6:
        ov76_0223D984(v0->unk_08, 0, +2);
        ManagedSprite_SetAnimationFrame(v0->unk_04, 0);
        SysTask_Done(param0);
        Heap_Free(v0);
        break;
    default:
        v0->unk_00++;
        break;
    }
}

static void ov76_0223DA00(ManagedSprite *param0, FontOAM *param1)
{
    UnkStruct_ov76_0223D9AC *v0 = Heap_Alloc(HEAP_ID_53, sizeof(UnkStruct_ov76_0223D9AC));

    v0->unk_00 = 1;
    v0->unk_04 = param0;
    v0->unk_08 = param1;

    ov76_0223D984(v0->unk_08, 0, -1);
    SysTask_Start(ov76_0223D9AC, v0, 1000);
}

void ov76_0223DA34(u32 param0, enum TouchScreenButtonState param1, void *param2)
{
    BallCapsuleSystem *ballCapsuleSys = (BallCapsuleSystem *)param2;

    if (ballCapsuleSys->unk_D4.unk_04 == 0) {
        return;
    }

    switch (param0) {
    case 8:
        if (param1 == TOUCH_BUTTON_PRESSED) {
            if (ballCapsuleSys->unk_418.unk_00 > 0) {
                ballCapsuleSys->unk_418.unk_00--;
            } else {
                ballCapsuleSys->unk_418.unk_00 = ballCapsuleSys->unk_418.unk_04 - 1;
            }

            ov76_0223BC70(ballCapsuleSys);
            ov76_0223BA90(ballCapsuleSys, ballCapsuleSys->unk_418.unk_00);
            ov76_0223BB04(ballCapsuleSys);
            ov76_0223BBAC(ballCapsuleSys);
            ov76_0223CC8C(ballCapsuleSys);
            Sound_PlayEffect(SEQ_SE_DP_CUSTOM02);
        }

        ov76_0223D94C(ballCapsuleSys->unk_3E4.unk_00[8], param1);
        break;
    case 9:
        if (param1 == TOUCH_BUTTON_PRESSED) {
            ballCapsuleSys->unk_418.unk_00++;
            ballCapsuleSys->unk_418.unk_00 %= ballCapsuleSys->unk_418.unk_04;

            ov76_0223BC70(ballCapsuleSys);
            ov76_0223BA90(ballCapsuleSys, ballCapsuleSys->unk_418.unk_00);
            ov76_0223BB04(ballCapsuleSys);
            ov76_0223BBAC(ballCapsuleSys);
            ov76_0223CC8C(ballCapsuleSys);
            Sound_PlayEffect(SEQ_SE_DP_CUSTOM02);
        }
        ov76_0223D94C(ballCapsuleSys->unk_3E4.unk_00[9], param1);
        break;
    case 10:
        if (param1 == TOUCH_BUTTON_PRESSED) {
            if (ballCapsuleSys->unk_3D4 != 5) {
                ballCapsuleSys->unk_3D4 = 5;
                Sound_PlayEffect(SEQ_SE_DP_DECIDE);
            }

            ov76_0223DA00(ballCapsuleSys->unk_3E4.unk_00[10], NULL);
        }

        ov76_0223D94C(ballCapsuleSys->unk_3E4.unk_00[10], param1);
        break;
    case 11:
        if (param1 == TOUCH_BUTTON_PRESSED) {
            if (ballCapsuleSys->unk_3D4 != 6) {
                ballCapsuleSys->unk_3D4 = 6;
                ov76_0223DCB8(ballCapsuleSys, 0);
                Sound_PlayEffect(SEQ_SE_DP_PIRORIRO);
            }

            ov76_0223DA00(ballCapsuleSys->unk_3E4.unk_00[11], ballCapsuleSys->unk_D4.unk_164[0]);
        }

        ov76_0223D94C(ballCapsuleSys->unk_3E4.unk_00[11], param1);
        break;
    case 12:
        if (param1 == TOUCH_BUTTON_PRESSED) {
            if (ballCapsuleSys->unk_3D4 != 7) {
                ballCapsuleSys->unk_3D4 = 7;
                ov76_0223DCB8(ballCapsuleSys, 0);
                Sound_PlayEffect(SEQ_SE_DP_DECIDE);
            }

            ov76_0223DA00(ballCapsuleSys->unk_3E4.unk_00[12], ballCapsuleSys->unk_D4.unk_164[1]);
        }

        ov76_0223D94C(ballCapsuleSys->unk_3E4.unk_00[12], param1);
        break;
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7: {
        int v1;

        if (param1 == TOUCH_BUTTON_PRESSED) {
            if (ov76_0223B2F8(ballCapsuleSys) == 0) {
                Sound_PlayEffect(SEQ_SE_DP_CUSTOM06);
                ov76_0223CA30(&ballCapsuleSys->unk_D4.unk_18[0], 15);
            } else {
                if ((ballCapsuleSys->unk_418.unk_08[param0] != 0) && (SealCase_GetSealCount(ballCapsuleSys->unk_64, ballCapsuleSys->unk_418.unk_08[param0] - 1) != 0)) {
                    ballCapsuleSys->unk_D4.unk_00 = ov76_0223B278(ballCapsuleSys, param0);
                    v1 = sub_02098164(ballCapsuleSys->unk_418.unk_08[param0]);

                    ov76_0223CDC4(&ballCapsuleSys->unk_D4.unk_18[0], v1);
                    GiveOrTakeSeal(ballCapsuleSys->unk_00->unk_20, ballCapsuleSys->unk_418.unk_08[param0], -1);
                    ov76_0223CD20(ballCapsuleSys, param0);
                    Sound_PlayEffect(SEQ_SE_DP_BOX02);
                } else {
                    if (ballCapsuleSys->unk_418.unk_08[param0] != 0) {
                        Sound_PlayEffect(SEQ_SE_DP_CUSTOM06);
                        ov76_0223CA30(&ballCapsuleSys->unk_D4.unk_18[0], 16);
                    }
                }
            }
        }
    } break;
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20: {
        int v2;
        int v3;

        if (param1 == TOUCH_BUTTON_PRESSED) {
            v2 = param0 - 13;
            ov76_0223B5C4(ballCapsuleSys, param1, v2);
            v3 = sub_02098164(ballCapsuleSys->unk_324[v2].unk_04);

            ov76_0223CDC4(&ballCapsuleSys->unk_D4.unk_18[0], v3);
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
    return ballCapsuleSys->unk_D4.unk_184;
}

void ov76_0223DCB8(BallCapsuleSystem *ballCapsuleSys, BOOL param1)
{
    ballCapsuleSys->unk_D4.unk_04 = param1;
}

static const TouchScreenRect Unk_ov76_0223EE44[] = {
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
    int v0;
    const TouchScreenRect v1 = { 0, 0, 0, 0 };

    for (v0 = 0; v0 < 13; v0++) {
        ballCapsuleSys->unk_D4.unk_FC[v0] = Unk_ov76_0223EE44[v0];
    }

    for (; v0 < 21; v0++) {
        ballCapsuleSys->unk_D4.unk_FC[v0] = v1;
        ballCapsuleSys->unk_324[v0 - 13].unk_0C = &ballCapsuleSys->unk_D4.unk_FC[v0];
    }

    ballCapsuleSys->unk_D4.unk_F8 = TouchScreenActions_RegisterHandler(ballCapsuleSys->unk_D4.unk_FC, 21, ov76_0223DA34, ballCapsuleSys, HEAP_ID_53);
}

void ov76_0223DD88(BallCapsuleSystem *ballCapsuleSys)
{
    PokemonSpriteTemplate v0;
    SpriteAnimFrame v1[10];
    int v2;
    int v3;

    Pokemon_BuildSpriteTemplate(&v0, ballCapsuleSys->unk_428, 2);

    v2 = Pokemon_GetValue(ballCapsuleSys->unk_428, MON_DATA_SPECIES, NULL);
    v3 = Pokemon_SpriteYOffset(ballCapsuleSys->unk_428, 2);

    ballCapsuleSys->unk_D4.unk_D8 = v3;
    PokemonSprite_LoadAnimFrames(ballCapsuleSys->unk_42C, &v1[0], v2, 1);
    ballCapsuleSys->unk_D4.unk_D4 = PokemonSpriteManager_CreateSprite(ballCapsuleSys->unk_D4.unk_D0, &v0, 256 - 64, 48 + v3, -0x280, 0, &v1[0], NULL);
}

static void ov76_0223DE00(BallCapsuleSystem *ballCapsuleSys)
{
    int v0;
    int v1;

    v0 = Pokemon_GetValue(ballCapsuleSys->unk_428, MON_DATA_SPECIES, NULL);
    v1 = Pokemon_GetNature(ballCapsuleSys->unk_428);

    PokemonSprite_InitAnim(ballCapsuleSys->unk_D4.unk_D4, 1);
    PokemonSprite_LoadAnim(ballCapsuleSys->unk_42C, ballCapsuleSys->unk_D4.unk_188, ballCapsuleSys->unk_D4.unk_D4, v0, 2, 0, 0);
}

static void ov76_0223DE54(BallCapsuleSystem *ballCapsuleSys)
{
    PokemonSprite_SetAttribute(ballCapsuleSys->unk_D4.unk_D4, MON_SPRITE_SCALE_X, 0x0);
    PokemonSprite_SetAttribute(ballCapsuleSys->unk_D4.unk_D4, MON_SPRITE_SCALE_Y, 0x0);
}

static BOOL ov76_0223DE78(BallCapsuleSystem *ballCapsuleSys)
{
    if (PokemonSprite_GetAttribute(ballCapsuleSys->unk_D4.unk_D4, MON_SPRITE_SCALE_X) == 0x100) {
        return 0;
    } else if (PokemonSprite_GetAttribute(ballCapsuleSys->unk_D4.unk_D4, MON_SPRITE_SCALE_X) >= 0x100) {
        PokemonSprite_SetAttribute(ballCapsuleSys->unk_D4.unk_D4, MON_SPRITE_SCALE_X, 0x100);
        PokemonSprite_SetAttribute(ballCapsuleSys->unk_D4.unk_D4, MON_SPRITE_SCALE_Y, 0x100);
        return 0;
    } else {
        PokemonSprite_AddAttribute(ballCapsuleSys->unk_D4.unk_D4, MON_SPRITE_SCALE_X, 0x20);
        PokemonSprite_AddAttribute(ballCapsuleSys->unk_D4.unk_D4, MON_SPRITE_SCALE_Y, 0x20);
        PokemonSprite_CalcScaledYOffset(ballCapsuleSys->unk_D4.unk_D4, ballCapsuleSys->unk_D4.unk_D8);
    }

    return 1;
}

static BOOL ov76_0223DEF4(BallCapsuleSystem *ballCapsuleSys)
{
    if (PokemonSprite_GetAttribute(ballCapsuleSys->unk_D4.unk_D4, MON_SPRITE_SCALE_X) == 0x0) {
        return 0;
    } else if (PokemonSprite_GetAttribute(ballCapsuleSys->unk_D4.unk_D4, MON_SPRITE_SCALE_X) <= 0x0) {
        PokemonSprite_SetAttribute(ballCapsuleSys->unk_D4.unk_D4, MON_SPRITE_SCALE_X, 0x0);
        PokemonSprite_SetAttribute(ballCapsuleSys->unk_D4.unk_D4, MON_SPRITE_SCALE_Y, 0x0);
        return 0;
    } else {
        PokemonSprite_AddAttribute(ballCapsuleSys->unk_D4.unk_D4, MON_SPRITE_SCALE_X, -0x20);
        PokemonSprite_AddAttribute(ballCapsuleSys->unk_D4.unk_D4, MON_SPRITE_SCALE_Y, -0x20);
        PokemonSprite_CalcScaledYOffset(ballCapsuleSys->unk_D4.unk_D4, ballCapsuleSys->unk_D4.unk_D8);
    }

    return 1;
}

void ov76_0223DF70(BallCapsuleSystem *ballCapsuleSys, int param1)
{
    PokemonSprite_SetAttribute(ballCapsuleSys->unk_D4.unk_D4, MON_SPRITE_HIDE, param1);
}

void ov76_0223DF84(BallCapsuleSystem *ballCapsuleSys)
{
    PokemonSprite_Delete(ballCapsuleSys->unk_D4.unk_D4);
}

static BOOL ov76_0223DF94(BallCapsuleSystem *ballCapsuleSys)
{
    switch (ballCapsuleSys->unk_3D4) {
    case 0:
        ov76_0223DCB8(ballCapsuleSys, 0);
        ov76_0223CA30(&ballCapsuleSys->unk_D4.unk_18[0], 0xFFFF);
        ov76_0223CB58(ballCapsuleSys);
        ov76_0223CC8C(ballCapsuleSys);
        ov76_0223BD30(ballCapsuleSys, +1, 4);
        ov76_0223D338(ballCapsuleSys);
        ov76_0223D368(ballCapsuleSys);
        ballCapsuleSys->unk_D4.unk_18C = 0;
        ballCapsuleSys->unk_3D4++;
        break;
    case 1:
        if (ov76_0223DCB0(ballCapsuleSys) == 1) {
            break;
        }

        PaletteData_StartFade(ballCapsuleSys->unk_D4.unk_14, 0x1, (1 << 0) | (1 << 1), 0, 0, 16, 0);
        PaletteData_StartFade(ballCapsuleSys->unk_D4.unk_14, 0x4, 0xFFFF, 0, 0, 16, 0);
        GXLayers_EngineAToggleLayers(GX_PLANEMASK_OBJ, 0);
        GXLayers_EngineBToggleLayers(GX_PLANEMASK_BG2, 1);
        Bg_SetPriority(BG_LAYER_SUB_3, 1);
        ov76_0223D2F4(ballCapsuleSys, 1);
        ov76_0223C568(ballCapsuleSys, 1);
        ov76_0223B96C(ballCapsuleSys, 1);
        ballCapsuleSys->unk_3D4++;
        break;
    case 2:
        if (PaletteData_GetSelectedBuffersMask(ballCapsuleSys->unk_D4.unk_14) != 0) {
            break;
        }
        Bg_SetPriority(BG_LAYER_MAIN_3, 1);
        PaletteData_StartFade(ballCapsuleSys->unk_D4.unk_14, 0x1, 1 << 1, 0, 16, 0, 0);
        ballCapsuleSys->unk_3D4++;
        break;
    case 3:
        if (PaletteData_GetSelectedBuffersMask(ballCapsuleSys->unk_D4.unk_14) != 0) {
            break;
        }
        ov76_0223DCB8(ballCapsuleSys, 1);
        ballCapsuleSys->unk_3D4++;
        break;
    case 4:
        break;
    case 5: {
        switch (ballCapsuleSys->unk_3DC) {
        case 0:
            GXLayers_EngineAToggleLayers(GX_PLANEMASK_OBJ, 1);
            PaletteData_StartFade(ballCapsuleSys->unk_D4.unk_14, 0x2, (1 << 0) | (1 << 1) | (1 << 3) | (1 << 11), 0, 0, 10, 0);
            PaletteData_StartFade(ballCapsuleSys->unk_D4.unk_14, 0x8, 0xFFFF, 0, 0, 10, 0);
            ov76_0223DCB8(ballCapsuleSys, 0);
            ballCapsuleSys->unk_3E0 = 0;
            ov76_0223D3CC(ballCapsuleSys);
            ov76_0223DD88(ballCapsuleSys);
            ov76_0223DF70(ballCapsuleSys, 1);
            ov76_0223CA30(&ballCapsuleSys->unk_D4.unk_18[0], 13);
            ballCapsuleSys->unk_3DC++;
            break;
        case 1:
            if (PaletteData_GetSelectedBuffersMask(ballCapsuleSys->unk_D4.unk_14) != 0) {
                break;
            }

            {
                BallThrow v0;

                v0.type = 1;
                v0.heapID = HEAP_ID_53;
                v0.mode = 0;
                v0.target = 0xFF;
                v0.bgPrio = 0;
                v0.surface = 1;
                v0.cellActorSys = ballCapsuleSys->unk_D4.unk_08;
                v0.paletteSys = ballCapsuleSys->unk_D4.unk_14;
                v0.ballID = Pokemon_GetValue(ballCapsuleSys->unk_428, MON_DATA_POKEBALL, NULL);

                ballCapsuleSys->unk_D4.unk_158 = ov12_02237728(&v0);
                ballCapsuleSys->unk_3DC++;
            }
            break;
        case 2: {
            int v1;
            BallCapsule v2;
            UnkStruct_ov12_02236030 v3 = { 0 };

            v3.unk_00 = 1;
            v3.unk_08 = ballCapsuleSys->unk_428;
            v1 = ballCapsuleSys->unk_3C4[0] + 1;

            ov76_0223B848(&v2, ballCapsuleSys);
            Pokemon_SetValue(ballCapsuleSys->unk_428, MON_DATA_BALL_CAPSULE_ID, (u8 *)&v1);
            Pokemon_SetValue(ballCapsuleSys->unk_428, MON_DATA_BALL_CAPSULE, &v2);

            ballCapsuleSys->unk_D4.unk_154 = ov12_02236004(HEAP_ID_53, &v3);
            ov12_02236320(ballCapsuleSys->unk_D4.unk_154);
        }
            ballCapsuleSys->unk_3DC++;
            ballCapsuleSys->unk_3E0 = 0;
            break;
        case 3:
            if (ov12_02236374(ballCapsuleSys->unk_D4.unk_154) != 1) {
                break;
            }

            if (ov12_02237810(ballCapsuleSys->unk_D4.unk_158) == 1) {
                ov76_0223DE78(ballCapsuleSys);
                ov12_022363B4(ballCapsuleSys->unk_D4.unk_154);
                ov76_0223DF70(ballCapsuleSys, 0);
                ov76_0223DE54(ballCapsuleSys);
                Sound_PlayEffect(SEQ_SE_DP_CUSTOM05);
                ballCapsuleSys->unk_3DC++;
            }
            break;
        case 4: {
            BOOL v4 = ov76_0223DE78(ballCapsuleSys);

            if (ov12_022377F8(ballCapsuleSys->unk_D4.unk_158) != 0) {
                break;
            }

            if (v4 == 0) {
                ov12_0223783C(ballCapsuleSys->unk_D4.unk_158);
                GXLayers_EngineAToggleLayers(GX_PLANEMASK_OBJ, 0);
                ov76_0223DE00(ballCapsuleSys);
                ballCapsuleSys->unk_3E0 = 0;
                ballCapsuleSys->unk_3DC++;
            }
        } break;
        case 5:
            if (ov12_022363C4(ballCapsuleSys->unk_D4.unk_154) != 0) {
                break;
            }

            if (PokemonAnimManager_HasAnimCompleted(ballCapsuleSys->unk_D4.unk_188, 0) != TRUE) {
                break;
            }

            if (PokemonSprite_IsAnimActive(ballCapsuleSys->unk_D4.unk_D4) != 0) {
                break;
            }

            ov12_02236428(ballCapsuleSys->unk_D4.unk_154);
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
                v5.heapID = HEAP_ID_53;
                v5.mode = 5;
                v5.target = 0xFF;
                v5.bgPrio = 0;
                v5.surface = 1;
                v5.cellActorSys = ballCapsuleSys->unk_D4.unk_08;
                v5.paletteSys = ballCapsuleSys->unk_D4.unk_14;
                v5.ballID = Pokemon_GetValue(ballCapsuleSys->unk_428, MON_DATA_POKEBALL, NULL);

                ballCapsuleSys->unk_D4.unk_158 = ov12_02237728(&v5);

                ov76_0223D3A0();
                GXLayers_EngineAToggleLayers(GX_PLANEMASK_OBJ, 1);

                ballCapsuleSys->unk_3DC++;
            }
            break;
        case 7: {
            BOOL v6 = ov76_0223DEF4(ballCapsuleSys);

            if ((ov12_022377F8(ballCapsuleSys->unk_D4.unk_158) == 0) && (v6 == 0)) {
                PaletteData_StartFade(ballCapsuleSys->unk_D4.unk_14, 0x2, (1 << 0) | (1 << 1) | (1 << 3) | (1 << 11), 0, 10, 0, 0);
                PaletteData_StartFade(ballCapsuleSys->unk_D4.unk_14, 0x8, 0xFFFF, 0, 10, 0, 0);
                ov76_0223DF70(ballCapsuleSys, 1);
                ov76_0223DF84(ballCapsuleSys);
                ov12_0223783C(ballCapsuleSys->unk_D4.unk_158);
                ballCapsuleSys->unk_3DC++;
            }
        } break;
        default:
            if (PaletteData_GetSelectedBuffersMask(ballCapsuleSys->unk_D4.unk_14) != 0) {
                break;
            }

            GXLayers_EngineAToggleLayers(GX_PLANEMASK_OBJ, 0);
            ov76_0223DCB8(ballCapsuleSys, 1);
            ballCapsuleSys->unk_3DC = 0;
            ballCapsuleSys->unk_3D4 = 4;
            ov76_0223CA30(&ballCapsuleSys->unk_D4.unk_18[0], 0xFFFF);
            break;
        }
    } break;
    case 6:
        ov76_0223CA30(&ballCapsuleSys->unk_D4.unk_18[0], 12);

        if (ov76_0223B78C(ballCapsuleSys) == 1) {
            ballCapsuleSys->unk_D4.unk_18C = 1;
        }

        ov76_0223B808(ballCapsuleSys);
        ov76_0223C7E0(ballCapsuleSys);
        {
            GameRecords *v7;
            v7 = SaveData_GetGameRecords(ballCapsuleSys->unk_00->saveData);

            GameRecords_IncrementTrainerScore(v7, TRAINER_SCORE_EVENT_UNK_06);
        }

        {
            Pokemon *v8;

            if (ballCapsuleSys->unk_264[ballCapsuleSys->unk_3C4[0]].unk_00 != 0xff) {
                v8 = ballCapsuleSys->unk_00->unk_04[ballCapsuleSys->unk_264[ballCapsuleSys->unk_3C4[0]].unk_00];

                Pokemon_SetValue(v8, MON_DATA_BALL_CAPSULE, SealCase_GetCapsuleById(ballCapsuleSys->unk_00->unk_20, ballCapsuleSys->unk_3C4[0]));
            }
        }
        ballCapsuleSys->unk_3D4 = 8;
        break;
    case 7: {
        switch (ballCapsuleSys->unk_3DC) {
        case 0:
            if (ov76_0223B78C(ballCapsuleSys) == 0) {
                ballCapsuleSys->unk_3D4 = 8;
                break;
            }

            PaletteData_StartFade(ballCapsuleSys->unk_D4.unk_14, 0x2, (1 << 0) | (1 << 1) | (1 << 3) | (1 << 11), 0, 0, 10, 0);
            PaletteData_StartFade(ballCapsuleSys->unk_D4.unk_14, 0x8, 0xFFFF, 0, 0, 10, 0);
            ov76_0223DCB8(ballCapsuleSys, 0);
            GXLayers_EngineBToggleLayers(GX_PLANEMASK_BG0, 0);
            ballCapsuleSys->unk_3DC++;
            break;
        case 1:
            if (PaletteData_GetSelectedBuffersMask(ballCapsuleSys->unk_D4.unk_14) != 0) {
                break;
            }

            PaletteData_SetAutoTransparent(ballCapsuleSys->unk_D4.unk_14, 0);
            ov76_0223CA98(ballCapsuleSys->unk_D4.unk_10, &ballCapsuleSys->unk_D4.unk_18[2], 4, 2, 1, 27, 4, 0 + ((1 + (18 + 12)) + 9));

            {
                UnkStruct_02015958 v9;

                v9.unk_00 = ballCapsuleSys->unk_D4.unk_10;
                v9.unk_04 = 4;
                v9.unk_08 = (((3 + 1) * 2) + (((3 + 1) * 2) + (((3 + 1) * 2) + (((3 + 1) * 2) + (((3 + 1) * 2) + (((3 + 1) * 2) + (((3 + 1) * 2) + (((3 + 1) * 2) + ((27 * 4) + (0 + ((1 + (18 + 12)) + 9)))))))))));
                v9.unk_0C = 5;
                v9.unk_10 = 25;
                v9.unk_11 = 6;

                ballCapsuleSys->unk_D4.unk_150 = sub_02015920(HEAP_ID_53);
                sub_02015958(ballCapsuleSys->unk_D4.unk_150, &v9);
            }

            ov76_0223CA30(&ballCapsuleSys->unk_D4.unk_18[2], 14);
            ballCapsuleSys->unk_3DC++;
            break;
        case 2:
            GXLayers_EngineBToggleLayers(GX_PLANEMASK_BG0, 1);
            ballCapsuleSys->unk_3DC++;
        case 3: {
            u32 v10;
            v10 = sub_020159FC(ballCapsuleSys->unk_D4.unk_150);

            switch (v10) {
            case 1:
            case 2:
                ballCapsuleSys->unk_3E0 = v10;
                PaletteData_SetAutoTransparent(ballCapsuleSys->unk_D4.unk_14, 1);
                sub_02015A54(ballCapsuleSys->unk_D4.unk_150);
                sub_02015938(ballCapsuleSys->unk_D4.unk_150);
                Window_EraseMessageBox(&ballCapsuleSys->unk_D4.unk_18[2], 1);
                Window_ClearAndCopyToVRAM(&ballCapsuleSys->unk_D4.unk_18[2]);
                Window_Remove(&ballCapsuleSys->unk_D4.unk_18[2]);
                ballCapsuleSys->unk_3DC++;
                break;
            case 0:
            default:
                break;
            }
        } break;
        case 4:
            PaletteData_StartFade(ballCapsuleSys->unk_D4.unk_14, 0x2, (1 << 0) | (1 << 1) | (1 << 3) | (1 << 11), 0, 10, 0, 0);
            PaletteData_StartFade(ballCapsuleSys->unk_D4.unk_14, 0x8, 0xFFFF, 0, 10, 0, 0);
            ballCapsuleSys->unk_3DC++;
            break;
        case 5:
            if (PaletteData_GetSelectedBuffersMask(ballCapsuleSys->unk_D4.unk_14) != 0) {
                break;
            }

            switch (ballCapsuleSys->unk_3E0) {
            case 1:
                ov76_0223D350(ballCapsuleSys);
                ov76_0223D384(ballCapsuleSys);
                ov76_0223CC8C(ballCapsuleSys);
                ov76_0223B678(ballCapsuleSys);
                ov76_0223B208(ballCapsuleSys);
                ov76_0223B69C(ballCapsuleSys, 1);
                ballCapsuleSys->unk_3D4 = 6;
                break;
            case 2:
                ballCapsuleSys->unk_3D4 = 4;
                break;
            case 0:
            default:
                break;
            }

            ov76_0223DCB8(ballCapsuleSys, 1);
            ballCapsuleSys->unk_3DC = 0;
        }
    } break;
    case 8:
        GXLayers_EngineAToggleLayers(GX_PLANEMASK_OBJ, 1);
        PaletteData_StartFade(ballCapsuleSys->unk_D4.unk_14, 0x1, 1 << 1, 0, 0, 16, 0);
        ballCapsuleSys->unk_3D4++;
        break;
    case 9:

        if (PaletteData_GetSelectedBuffersMask(ballCapsuleSys->unk_D4.unk_14) != 0) {
            break;
        }

        PaletteData_StartFade(ballCapsuleSys->unk_D4.unk_14, 0x1, 1 << 0, 0, 16, 0, 0);
        PaletteData_StartFade(ballCapsuleSys->unk_D4.unk_14, 0x4, 0xFFFF, 0, 16, 0, 0);
        Bg_SetPriority(BG_LAYER_MAIN_3, 3);
        Bg_SetPriority(BG_LAYER_SUB_3, 3);
        GXLayers_EngineBToggleLayers(GX_PLANEMASK_BG2, 0);
        ov76_0223D2F4(ballCapsuleSys, 0);
        ov76_0223C568(ballCapsuleSys, 0);
        ov76_0223B96C(ballCapsuleSys, 0);
        ov76_0223BD30(ballCapsuleSys, -1, 4);
        ov76_0223DCB8(ballCapsuleSys, 0);
        ballCapsuleSys->unk_3D4++;
        break;
    case 10:
        if (PaletteData_GetSelectedBuffersMask(ballCapsuleSys->unk_D4.unk_14) != 0) {
            break;
        }

        if (ov76_0223DCB0(ballCapsuleSys) == 1) {
            break;
        }

        ManagedSprite_SetDrawFlag(ballCapsuleSys->unk_2F4[1], 0);
        ov76_0223D494(ballCapsuleSys, 0, 0xff, 0);
        ov76_0223CA30(&ballCapsuleSys->unk_D4.unk_18[0], 7);
        ov76_0223CDA4(ballCapsuleSys);

        return 1;
    }

    if (ballCapsuleSys->unk_D4.unk_00 == 0xFF) {
        TouchScreenActions_HandleAction(ballCapsuleSys->unk_D4.unk_F8);
    } else {
        u32 v11, v12;
        int v13;
        BOOL v14;

        v13 = ballCapsuleSys->unk_D4.unk_00;
        v14 = TouchScreen_GetHoldState(&v11, &v12);

        if (v14) {
            int v15;

            v15 = sub_02098164(ballCapsuleSys->unk_324[v13].unk_04);
            ov76_0223CDC4(&ballCapsuleSys->unk_D4.unk_18[0], v15);
            ManagedSprite_SetPositionXY(ballCapsuleSys->unk_324[v13].unk_08, v11, v12);
            ov76_0223B758(ballCapsuleSys, v13);
            ov76_0223B7D4(ballCapsuleSys, v13);
        } else {
            BOOL v16;

            v16 = ov76_0223B6C4(ballCapsuleSys, v13);
            ov76_0223B184(ballCapsuleSys->unk_324[v13].unk_0C, ballCapsuleSys->unk_324[v13].unk_08, 0);

            if (v16 == 0) {
                GiveOrTakeSeal2(ballCapsuleSys->unk_00->unk_20, ballCapsuleSys->unk_324[v13].unk_04, +1);
                ov76_0223CC8C(ballCapsuleSys);
                ov76_0223B704(ballCapsuleSys, v13);
            }

            Sound_PlayEffect(SEQ_SE_DP_BOX01);
            ballCapsuleSys->unk_D4.unk_00 = 0xFF;
            ov76_0223CDC4(&ballCapsuleSys->unk_D4.unk_18[0], 0xFFFF);
        }
    }

    ov76_0223D318(ballCapsuleSys);
    ov76_0223C544(ballCapsuleSys);
    ov76_0223C304(ballCapsuleSys);
    ov76_0223C88C(ballCapsuleSys);

    return 1;
}

static BOOL ov76_0223E8A4(BallCapsuleSystem *ballCapsuleSys)
{
    switch (ballCapsuleSys->unk_3D4) {
    case 0:
        ballCapsuleSys->unk_3D4++;
    case 1:
        ov76_0223CE64();
        ballCapsuleSys->unk_3D4++;
        break;
    case 2:
        if (IsScreenFadeDone() != 1) {
            break;
        }

        Window_Remove(&ballCapsuleSys->unk_D4.unk_18[0]);
        ov76_0223C8BC(ballCapsuleSys);
        ov76_0223C32C(ballCapsuleSys);
        ov76_0223D31C(ballCapsuleSys);
        ov76_0223C588(ballCapsuleSys);
        ov76_0223BC70(ballCapsuleSys);
        sub_02097F30(ballCapsuleSys->unk_00, 1);

        return 0;
    }

    ov76_0223C304(ballCapsuleSys);
    ov76_0223C88C(ballCapsuleSys);

    return 1;
}

void ov76_0223E91C(BallCapsuleSystem *ballCapsuleSys, int param1)
{
    int v0;
    int v1;
    int v2 = 0;
    BallCapsule v3;

    v1 = ballCapsuleSys->unk_04[param1].unk_00;

    if (v1 != 0xff) {
        Pokemon_SetValue(ballCapsuleSys->unk_00->unk_04[v1], MON_DATA_BALL_CAPSULE_ID, (u8 *)&v2);
    }

    ballCapsuleSys->unk_04[param1].unk_00 = 0xff;
    ov76_0223C7E0(ballCapsuleSys);
}

static BOOL ov76_0223E950(BallCapsuleSystem *ballCapsuleSys)
{
    switch (ballCapsuleSys->unk_3D4) {
    case 0:
        ov76_0223E91C(ballCapsuleSys, ballCapsuleSys->unk_3C4[0]);
        ov76_0223CA30(&ballCapsuleSys->unk_D4.unk_18[0], 9);
        ballCapsuleSys->unk_3D4++;
        break;
    case 1:
        if (gSystem.pressedKeys & (0x1 | 0x2 | 0x400 | 0x800 | 0x40 | 0x80 | 0x20 | 0x10)) {
            ballCapsuleSys->unk_3D4++;
        }
        break;
    case 2:
        ov76_0223D494(ballCapsuleSys, 0, 0xff, 0);
        ov76_0223CA30(&ballCapsuleSys->unk_D4.unk_18[0], 7);
        break;
    }

    ov76_0223C304(ballCapsuleSys);
    ov76_0223C88C(ballCapsuleSys);

    return 1;
}

static BOOL ov76_0223E9C4(BallCapsuleSystem *ballCapsuleSys)
{
    switch (ballCapsuleSys->unk_3D4) {
    case 0:
        ManagedSprite_SetPriority(ballCapsuleSys->unk_2F4[0], 25);
        ManagedSprite_SetPriority(ballCapsuleSys->unk_2F4[1], 20);
        ManagedSprite_SetAnim(ballCapsuleSys->unk_2F4[0], 1);
        ov76_0223CA30(&ballCapsuleSys->unk_D4.unk_18[0], 10);
        ManagedSprite_SetDrawFlag(ballCapsuleSys->unk_2F4[1], 1);
        ballCapsuleSys->unk_3D4++;
    case 1: {
        BOOL v0;

        v0 = ov76_0223D574(&(ballCapsuleSys->unk_3C4[1]));

        if (v0 == 1) {
            ov76_0223D600(ballCapsuleSys, 1, 0);

            Sound_PlayEffect(SEQ_SE_CONFIRM);
        } else if (gSystem.pressedKeys & PAD_BUTTON_A) {
            ov76_0223C80C(ballCapsuleSys, ballCapsuleSys->unk_3C4[0], ballCapsuleSys->unk_3C4[1]);
            ov76_0223D600(ballCapsuleSys, 1, 1);
            ov76_0223CA30(&ballCapsuleSys->unk_D4.unk_18[0], 11);
            ballCapsuleSys->unk_3D4 = 2;
            Sound_PlayEffect(SEQ_SE_CONFIRM);
        } else if (gSystem.pressedKeys & PAD_BUTTON_B) {
            ManagedSprite_SetDrawFlag(ballCapsuleSys->unk_2F4[1], 0);
            ov76_0223D600(ballCapsuleSys, 0, 1);
            ballCapsuleSys->unk_3D4 = 3;
            Sound_PlayEffect(SEQ_SE_DP_DECIDE);
        }
    } break;
    case 2:
        if (gSystem.pressedKeys & (0x1 | 0x2 | 0x400 | 0x800 | 0x40 | 0x80 | 0x20 | 0x10)) {
            ballCapsuleSys->unk_3D4 = 3;
        }
        break;
    case 3:
        ManagedSprite_SetPriority(ballCapsuleSys->unk_2F4[0], 20);
        ManagedSprite_SetPriority(ballCapsuleSys->unk_2F4[1], 25);
        ov76_0223D494(ballCapsuleSys, 0, 0xff, 0);
        ManagedSprite_SetAnim(ballCapsuleSys->unk_2F4[0], 0);
        ov76_0223CA30(&ballCapsuleSys->unk_D4.unk_18[0], 7);
        break;
    }

    ov76_0223C304(ballCapsuleSys);
    ov76_0223C88C(ballCapsuleSys);

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

    PokemonSpriteManager_UpdateCharAndPltt(ballCapsuleSys->unk_D4.unk_D0);
    VramTransfer_Process();
    SpriteSystem_TransferOam();
    PaletteData_CommitFadedBuffers(ballCapsuleSys->unk_D4.unk_14);
    Bg_RunScheduledUpdates(ballCapsuleSys->unk_D4.unk_10);

    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}
