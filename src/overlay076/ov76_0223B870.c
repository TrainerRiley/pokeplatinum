#include "overlay076/ov76_0223B870.h"

#include <nitro.h>
#include <string.h>

#include "constants/ball_capsule.h"
#include "constants/heap.h"
#include "constants/narc.h"

#include "struct_defs/seal_case.h"
#include "struct_defs/struct_020127E8.h"

#include "battle_anim/battle_anim_helpers.h"
#include "overlay076/const_ov76_0223EF3C.h"
#include "overlay076/ov76_0223D338.h"
#include "overlay076/struct_ov76_0223DE00.h"

#include "bg_window.h"
#include "char_transfer.h"
#include "font.h"
#include "g3d_pipeline.h"
#include "graphics.h"
#include "gx_layers.h"
#include "heap.h"
#include "menu.h"
#include "message.h"
#include "narc.h"
#include "palette.h"
#include "particle_system.h"
#include "pokemon.h"
#include "pokemon_icon.h"
#include "render_window.h"
#include "screen_fade.h"
#include "seals.h"
#include "sprite.h"
#include "sprite_system.h"
#include "string_gf.h"
#include "string_list.h"
#include "sys_task.h"
#include "sys_task_manager.h"
#include "text.h"
#include "touch_screen.h"
#include "unk_02012744.h"
#include "unk_0202419C.h"
#include "unk_0202C9F4.h"
#include "unk_02097B18.h"

void ov76_0223BF74(BgConfig *param0, Window *param1, int param2, BallCapsuleSystem *ballCapsuleSys, int param4);
void ov76_0223C110(BallCapsuleSystem *ballCapsuleSys);
void BallCapsuleSystem_UpdatePartyIconPositions(BallCapsuleSystem *ballCapsuleSys);
void BallCapsuleSystem_UpdateEditData(BallCapsuleSystem *ballCapsuleSys);
void ov76_0223C398(BallCapsuleEditor *ballCapsuleEditor);
void ov76_0223C424(BallCapsuleEditor *ballCapsuleEditor);
void ov76_0223C5A4(SpriteSystem *param0, SpriteManager *param1, PaletteData *param2, int param3, int param4, int param5, int param6, int param7, int param8, int param9, NARC *param10);
void ov76_0223C61C(BallCapsuleSystem *ballCapsuleSys, NARC *param1);
void BallCapsuleSystem_UpdateCapsuleData(BallCapsuleSystem *ballCapsuleSys);
void BallCapsuleSystem_TickSprites(BallCapsuleSystem *ballCapsuleSys);
void BallCapsuleSystem_DeleteSprites(BallCapsuleSystem *ballCapsuleSys);
void ov76_0223C8EC(BgConfig *param0, PaletteData *param1, int param2);
void ov76_0223C974(BgConfig *param0, PaletteData *param1, int param2);
void ov76_0223CB58(BallCapsuleSystem *ballCapsuleSys);
void ov76_0223CDA4(BallCapsuleSystem *ballCapsuleSys);
void ov76_0223CDC4(Window *param0, int param1);
void ov76_0223CE2C(void);
void ov76_0223CE44(void);
void ov76_0223CE64(void);
void ov76_0223CE84(BallCapsuleSystem *ballCapsuleSys, NARC *param1);
void ov76_0223CF24(BallCapsuleSystem *ballCapsuleSys, NARC *param1);
void ov76_0223CF88(BallCapsuleSystem *ballCapsuleSys, NARC *param1);
void ov76_0223CFEC(BallCapsuleSystem *ballCapsuleSys, NARC *param1);
void ov76_0223D16C(BallCapsuleSystem *ballCapsuleSys);
void ov76_0223D318(BallCapsuleSystem *ballCapsuleSys);
void ov76_0223D31C(BallCapsuleSystem *ballCapsuleSys);

// This sets the rect dimensions to be 2x the given width and height,
// centered at the sprite's position.
static void Rect_SetDimensions(TouchScreenRect *rect, ManagedSprite *sprite, int width, int height)
{
    s16 x, y;

    ManagedSprite_GetPositionXY(sprite, &x, &y);

    rect->rect.top = y - height;
    rect->rect.bottom = y + height;
    rect->rect.left = x - width;
    rect->rect.right = x + width;
}

void ov76_0223B8A8(BallCapsuleSystem *ballCapsuleSys)
{
    ballCapsuleSys->ballCapsuleEditor.unk_160 = sub_02012744(2, HEAP_ID_BALL_CAPSULE_SYSTEM);
    Font_InitManager(FONT_SUBSCREEN, HEAP_ID_BALL_CAPSULE_SYSTEM);
}

void ov76_0223B8C4(BallCapsuleSystem *ballCapsuleSys)
{
    Font_Free(FONT_SUBSCREEN);
    sub_02012870(ballCapsuleSys->ballCapsuleEditor.unk_164[0]);
    CharTransfer_ClearRange(&ballCapsuleSys->ballCapsuleEditor.unk_16C[0]);
    sub_02012870(ballCapsuleSys->ballCapsuleEditor.unk_164[1]);
    CharTransfer_ClearRange(&ballCapsuleSys->ballCapsuleEditor.unk_16C[1]);
    sub_020127BC(ballCapsuleSys->ballCapsuleEditor.unk_160);
}

void ov76_0223B904(BallCapsuleSystem *ballCapsuleSys)
{
    SpriteSystem_LoadPaletteBuffer(ballCapsuleSys->ballCapsuleEditor.paletteData, 3, ballCapsuleSys->ballCapsuleEditor.spriteSys, ballCapsuleSys->ballCapsuleEditor.spriteMan, 14, 7, 0, 1, NNS_G2D_VRAM_TYPE_2DSUB, 30000);
}

void ov76_0223B940(BallCapsuleSystem *ballCapsuleSys)
{
    ov76_0223B904(ballCapsuleSys);
    ov76_0223B98C(ballCapsuleSys, 0, 104, 165, 0);
    ov76_0223B98C(ballCapsuleSys, 1, 192, 165, 0);
}

void ov76_0223B96C(BallCapsuleSystem *ballCapsuleSys, BOOL param1)
{
    sub_020129D0(ballCapsuleSys->ballCapsuleEditor.unk_164[0], param1);
    sub_020129D0(ballCapsuleSys->ballCapsuleEditor.unk_164[1], param1);
}

void ov76_0223B98C(BallCapsuleSystem *ballCapsuleSys, int param1, int param2, int param3, int param4)
{
    UnkStruct_020127E8 v0;
    String *v1;
    int v2;
    int v3;
    MessageLoader *v4;
    Window v5;

    v4 = MessageLoader_Init(MSG_LOADER_PRELOAD_ENTIRE_BANK, NARC_INDEX_MSGDATA__PL_MSG, TEXT_BANK_UNK_0008, HEAP_ID_BALL_CAPSULE_SYSTEM);
    v1 = MessageLoader_GetNewString(v4, 5 + param1);

    {
        Window_Init(&v5);
        Window_AddToTopLeftCorner(ballCapsuleSys->ballCapsuleEditor.bgConfig, &v5, 10, 2, 0, 0);
        Text_AddPrinterWithParamsAndColor(&v5, FONT_SUBSCREEN, v1, 0, 0, TEXT_SPEED_NO_TRANSFER, TEXT_COLOR(15, 13, 2), NULL);
    }

    v2 = 30000;
    v3 = sub_02012898(&v5, NNS_G2D_VRAM_TYPE_2DSUB, HEAP_ID_BALL_CAPSULE_SYSTEM);
    CharTransfer_AllocRange(v3, 1, NNS_G2D_VRAM_TYPE_2DSUB, &ballCapsuleSys->ballCapsuleEditor.unk_16C[param1]);

    v0.unk_00 = ballCapsuleSys->ballCapsuleEditor.unk_160;
    v0.unk_04 = &v5;
    v0.unk_08 = SpriteManager_GetSpriteList(ballCapsuleSys->ballCapsuleEditor.spriteMan);
    v0.unk_0C = SpriteManager_FindPlttResourceProxy(ballCapsuleSys->ballCapsuleEditor.spriteMan, v2);
    v0.unk_10 = NULL;
    v0.unk_14 = ballCapsuleSys->ballCapsuleEditor.unk_16C[param1].offset;
    v0.unk_18 = param2 - Font_CalcStringWidth(FONT_SUBSCREEN, v1, 0) / 2;
    v0.unk_1C = param3 + 192;
    v0.unk_20 = 1;
    v0.unk_24 = 40;
    v0.unk_28 = NNS_G2D_VRAM_TYPE_2DSUB;
    v0.heapID = HEAP_ID_BALL_CAPSULE_SYSTEM;
    ballCapsuleSys->ballCapsuleEditor.unk_164[param1] = sub_020127E8(&v0);

    sub_02012AC0(ballCapsuleSys->ballCapsuleEditor.unk_164[param1], param4);
    String_Free(v1);
    MessageLoader_Free(v4);
    Window_Remove(&v5);
}

void BallCapsuleSystem_LoadCurrentPageSeals(BallCapsuleSystem *ballCapsuleSys, int pageIndex)
{
    int sealsOnPage = 0;
    int totalSeals = 0;
    int firstPageEntry = pageIndex;
    firstPageEntry *= SEALS_PER_PAGE;

    int i;
    for (i = 0; i < SEALS_PER_PAGE; i++) {
        ballCapsuleSys->sealCasePages.currentPageSeals[i] = 0;
    }

    for (i = 0; i < (SEAL_ID_MAX); i++) {
        BOOL containsSeal;
        for (int j = 0; j < TOTAL_CAPSULES; j++) {
            containsSeal = SealIsOnCapsule(ballCapsuleSys->capsuleData[j].ballCapsule, i);

            if (containsSeal) {
                break;
            }
        }

        int sealCount = ballCapsuleSys->currentSealCounts[i];

        // Include this seal in the list of seals if it has more than one seal
        // OR if the seal is currently on a capsule
        if ((sealCount != 0) || (containsSeal == TRUE)) {
            totalSeals++;

            // Skip all seals until we get to the requested page
            if (totalSeals <= firstPageEntry) {
                continue;
            }

            ballCapsuleSys->sealCasePages.currentPageSeals[sealsOnPage] = (i + 1);
            sealsOnPage++;

            if (sealsOnPage >= SEALS_PER_PAGE) {
                break;
            }
        }
    }
}

void BallCapsuleSystem_LoadCurrentPageData(BallCapsuleSystem *ballCapsuleSys)
{
    int i;
    SpriteSystem *spriteSys = ballCapsuleSys->ballCapsuleEditor.spriteSys;
    SpriteManager *spriteMan = ballCapsuleSys->ballCapsuleEditor.spriteMan;
    PaletteData *paletteData = ballCapsuleSys->ballCapsuleEditor.paletteData;

    for (i = 0; i < SEALS_PER_PAGE; i++) {
        int charID = SealData_GetCharID(ballCapsuleSys->sealCasePages.currentPageSeals[i]);
        SpriteSystem_LoadCharResObj(spriteSys, spriteMan, NARC_INDEX_APPLICATION__CUSTOM_BALL__DATA__CB_DATA, charID, TRUE, NNS_G2D_VRAM_TYPE_2DSUB, i + 25000);
    }

    SpriteSystem_LoadPaletteBuffer(paletteData, PLTTBUF_SUB_OBJ, spriteSys, spriteMan, 91, 293, FALSE, 1, NNS_G2D_VRAM_TYPE_2DSUB, 26000 + 292);
    SpriteSystem_LoadCellResObj(spriteSys, spriteMan, NARC_INDEX_APPLICATION__CUSTOM_BALL__DATA__CB_DATA, 180, TRUE, 27000 + 180);
    SpriteSystem_LoadAnimResObj(spriteSys, spriteMan, NARC_INDEX_APPLICATION__CUSTOM_BALL__DATA__CB_DATA, 88, TRUE, 28000 + 88);
}

void BallCapsuleSystem_CreatePageSprites(BallCapsuleSystem *ballCapsuleSys)
{
    BallCapsuleEditorSprites *editorSprites;
    SpriteTemplate spriteTemplate;
    SpriteSystem *spriteSys = ballCapsuleSys->ballCapsuleEditor.spriteSys;
    SpriteManager *spriteMan = ballCapsuleSys->ballCapsuleEditor.spriteMan;
    PaletteData *paletteData = ballCapsuleSys->ballCapsuleEditor.paletteData;
    editorSprites = &ballCapsuleSys->editorSprites;

    spriteTemplate.x = 0;
    spriteTemplate.y = 0;
    spriteTemplate.z = 0;
    spriteTemplate.animIdx = 0;
    spriteTemplate.priority = 60;
    spriteTemplate.vramType = NNS_G2D_VRAM_TYPE_2DSUB;
    spriteTemplate.bgPriority = 1;
    spriteTemplate.vramTransfer = FALSE;
    spriteTemplate.resources[4] = SPRITE_RESOURCE_NONE;
    spriteTemplate.resources[5] = SPRITE_RESOURCE_NONE;
    spriteTemplate.plttIdx = 0;
    spriteTemplate.resources[1] = 26000 + 292;
    spriteTemplate.resources[2] = 27000 + 180;
    spriteTemplate.resources[3] = 28000 + 88;

    int i;
    for (i = 0; i < SEALS_PER_PAGE; i++) {
        spriteTemplate.resources[0] = (i + 25000);
        editorSprites->sprites[i] = SpriteSystem_NewSprite(spriteSys, spriteMan, &spriteTemplate);
    }

    {
        const s16 sealButtonPos[][2] = {
            { 19, 23 },
            { 75, 23 },
            { 19, 47 },
            { 75, 47 },
            { 19, 71 },
            { 75, 71 },
            { 19, 95 },
            { 75, 95 },
        };

        for (i = 0; i < SEALS_PER_PAGE; i++) {
            ManagedSprite_SetPositionXY(ballCapsuleSys->editorSprites.sprites[i], sealButtonPos[i][0], sealButtonPos[i][1] - 1);
            ManagedSprite_TickFrame(ballCapsuleSys->editorSprites.sprites[i]);
            ManagedSprite_SetAnimationFrame(ballCapsuleSys->editorSprites.sprites[i], 0);
        }
    }
}

void BallCapsuleSystem_UnloadEditorSprites(BallCapsuleSystem *ballCapsuleSys)
{
    for (int i = 0; i < SEALS_PER_PAGE; i++) {
        SpriteManager_UnloadCharObjById(ballCapsuleSys->ballCapsuleEditor.spriteMan, i + 25000);
        Sprite_DeleteAndFreeResources(ballCapsuleSys->editorSprites.sprites[i]);
    }
}

typedef struct {
    XYTransformContext sealCtx[SEALS_PER_CAPSULE + 1];
    ManagedSprite *sealSprite[SEALS_PER_CAPSULE];
    BgConfig *bgConfig;
    int unused_168;
    BOOL *unk_16C;
} UnkStruct_ov76_0223BCA0;

static void ov76_0223BCA0(SysTask *param0, void *param1)
{
    UnkStruct_ov76_0223BCA0 *v0 = (UnkStruct_ov76_0223BCA0 *)param1;
    BOOL stillLerping = 0;
    {
        int i;

        for (i = 0; i < SEALS_PER_CAPSULE; i++) {
            if (v0->sealSprite[i] == NULL) {
                continue;
            }

            if (PosLerpContext_UpdateAndApplyToSprite(&v0->sealCtx[i], v0->sealSprite[i]) == TRUE) {
                stillLerping = TRUE;
            }
        }

        // true = still lerping
        if (PosLerpContext_Update(&v0->sealCtx[SEALS_PER_CAPSULE]) == TRUE) {
            stillLerping = TRUE;
            Bg_SetOffset(v0->bgConfig, BG_LAYER_SUB_1, 0, v0->sealCtx[SEALS_PER_CAPSULE].x);
            Bg_SetOffset(v0->bgConfig, BG_LAYER_SUB_1, 3, v0->sealCtx[SEALS_PER_CAPSULE].y);
        }
    }

    // lerp is done
    if (stillLerping == FALSE) {
        *(v0->unk_16C) = FALSE;
        Heap_Free(v0);
        SysTask_Done(param0);
    }
}

void ov76_0223BD30(BallCapsuleSystem *ballCapsuleSys, s8 param1, int param2)
{
    UnkStruct_ov76_0223BCA0 *v0 = Heap_Alloc(HEAP_ID_BALL_CAPSULE_SYSTEM, sizeof(UnkStruct_ov76_0223BCA0));
    v0->bgConfig = ballCapsuleSys->ballCapsuleEditor.bgConfig;

    {
        int i;

        for (int i = 0; i < SEALS_PER_CAPSULE; i++) {
            if (ballCapsuleSys->placedSeals[i].tapped == FALSE) {
                v0->sealSprite[i] = NULL;
                continue;
            }

            v0->sealSprite[i] = ballCapsuleSys->placedSeals[i].sealSprite;

            s16 x, y;
            ManagedSprite_GetPositionXY(v0->sealSprite[i], &x, &y);
            PosLerpContext_Init(&v0->sealCtx[i], x, x + ((+7 * 8) * param1), y, y + ((-2 * 8) * param1), param2);
            PosLerpContext_UpdateAndApplyToSprite(&v0->sealCtx[i], v0->sealSprite[i]);
            PosLerpContext_UpdateAndApplyToSprite(&v0->sealCtx[i], v0->sealSprite[i]);
        }
    }

    {
        int xOffset = Bg_GetXOffset(v0->bgConfig, 5);
        int yOffset = Bg_GetYOffset(v0->bgConfig, 5);
        PosLerpContext_Init(&v0->sealCtx[SEALS_PER_CAPSULE], xOffset, xOffset + (((+7 * 8) * param1) * -1), yOffset, yOffset + (((-2 * 8) * param1) * -1), param2);
    }

    v0->unk_16C = &ballCapsuleSys->ballCapsuleEditor.unk_184;
    *(v0->unk_16C) = TRUE;

    SysTask_Start(ov76_0223BCA0, v0, 0x1000);
}

G3DPipelineBuffers *ov76_0223BE6C(void)
{
    return G3DPipeline_Init(HEAP_ID_BALL_CAPSULE_SYSTEM, TEXTURE_VRAM_SIZE_256K, PALETTE_VRAM_SIZE_32K, ov76_0223BE8C);
}

void ov76_0223BE8C(void)
{
    GXLayers_EngineAToggleLayers(GX_PLANEMASK_BG0, 1);

    G2_SetBG0Priority(1);
    G3X_SetShading(GX_SHADING_TOON);
    G3X_AntiAlias(1);
    G3X_AlphaTest(0, 0);
    G3X_AlphaBlend(1);
    G3X_EdgeMarking(0);
    G3X_SetFog(0, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
    G3X_SetClearColor(GX_RGB(0, 0, 0), 0, 0x7fff, 63, 0);
    G3_ViewPort(0, 0, 255, 191);
}

void ov76_0223BF10(void)
{
    NNSGfdTexKey v0;
    NNSGfdPlttKey v1;
    u32 v2, v3;

    v0 = NNS_GfdAllocTexVram(0x2000 * 4, 0, 0);
    v1 = NNS_GfdAllocPlttVram(0x20 * 4, 0, 0);

    GF_ASSERT(v0 != NNS_GFD_ALLOC_ERROR_TEXKEY);
    GF_ASSERT(v1 != NNS_GFD_ALLOC_ERROR_PLTTKEY);

    v2 = NNS_GfdGetTexKeyAddr(v0);
    v3 = NNS_GfdGetPlttKeyAddr(v1);

    ParticleSystem_ZeroAll();
}

void ov76_0223BF50(void)
{
    int v0;
    const MtxFx43 *v1;

    G3_ResetG3X();

    v0 = ParticleSystem_DrawAll();

    if (v0 > 0) {
        G3_ResetG3X();
        NNS_G2dSetupSoftwareSpriteCamera();
    }

    ParticleSystem_UpdateAll();
    G3_RequestSwapBuffers(GX_SORTMODE_MANUAL, GX_BUFFERMODE_Z);
}

void ov76_0223BF74(BgConfig *param0, Window *param1, int param2, BallCapsuleSystem *ballCapsuleSys, int param4)
{
    MenuTemplate v0;
    int v1;
    int v2, v3, v4, v5, v6, v7;
    int v8[4];

    v1 = ov76_0223D45C(ballCapsuleSys, param4);
    v6 = ((9 * 8) + ((27 * 2) + (0 + ((1 + (18 + 12)) + 9))));
    v2 = 21;
    v4 = 9;

    switch (v1) {
    case 0:
        v3 = (15 - 2);
        v5 = (4 + 2);
        v8[0] = 0;
        v8[1] = 4;
        v8[2] = 1;
        break;
    case 1:
        v3 = 11;
        v5 = 8;
        v8[0] = 2;
        v8[1] = 0;
        v8[2] = 4;
        v8[3] = 1;
        break;
    case 2:
        v3 = 11;
        v5 = 8;
        v8[0] = 3;
        v8[1] = 0;
        v8[2] = 4;
        v8[3] = 1;
        break;
    case 3:
        v3 = 11;
        v5 = 8;
        v8[0] = 2;
        v8[1] = 0;
        v8[2] = 4;
        v8[3] = 1;
        break;
    }

    v7 = (v5 / 2);

    Window_Init(param1);
    Window_Add(param0, param1, param2, v2, v3, v4, v5, 14, v6);

    ballCapsuleSys->ballCapsuleEditor.unk_C8 = StringList_New(v7, HEAP_ID_BALL_CAPSULE_SYSTEM);

    {
        int v9;
        String *v10;
        MessageLoader *v11 = MessageLoader_Init(MSG_LOADER_PRELOAD_ENTIRE_BANK, NARC_INDEX_MSGDATA__PL_MSG, TEXT_BANK_UNK_0008, HEAP_ID_BALL_CAPSULE_SYSTEM);

        for (v9 = 0; v9 < v7; v9++) {
            v10 = MessageLoader_GetNewString(v11, Unk_ov76_0223EF3C[v8[v9]].unk_00);

            StringList_AddFromString(ballCapsuleSys->ballCapsuleEditor.unk_C8, v10, Unk_ov76_0223EF3C[v8[v9]].unk_04);
            String_Free(v10);
        }

        MessageLoader_Free(v11);
    }

    v0.choices = ballCapsuleSys->ballCapsuleEditor.unk_C8;
    v0.fontID = FONT_SYSTEM;
    v0.window = param1;
    v0.xSize = 1;
    v0.ySize = v7;
    v0.lineSpacing = 0;
    v0.suppressCursor = FALSE;
    v0.loopAround = TRUE;

    Window_DrawStandardFrame(param1, 1, 1 + (18 + 12), 13);
    ballCapsuleSys->ballCapsuleEditor.menu = Menu_NewAndCopyToVRAM(&v0, 8, 0, 0, HEAP_ID_BALL_CAPSULE_SYSTEM, PAD_BUTTON_B);
}

void GetCapsulePosition(int capsuleIndex, s16 *x, s16 *y)
{
    int offset = (capsuleIndex % 4);
    *x = 32 + 8 + (offset * 56);
    offset = (capsuleIndex >> 2);
    *y = 27 + (offset * 53);
}

void ov76_0223C110(BallCapsuleSystem *ballCapsuleSys)
{
    SpriteSystem *spriteSys = ballCapsuleSys->ballCapsuleEditor.spriteSys;
    SpriteManager *spriteMan = ballCapsuleSys->ballCapsuleEditor.spriteMan;
    PaletteData *paletteData = ballCapsuleSys->ballCapsuleEditor.paletteData;

    SpriteSystem_LoadPaletteBuffer(paletteData, PLTTBUF_MAIN_OBJ, spriteSys, spriteMan, NARC_INDEX_POKETOOL__ICONGRA__PL_POKE_ICON, PokeIconPalettesFileIndex(), 0, 3, NNS_G2D_VRAM_TYPE_2DMAIN, 16000);

    {
        int fileIndex = PokeIcon64KCellsFileIndex();
        SpriteSystem_LoadCellResObj(spriteSys, spriteMan, 19, fileIndex, 0, 17000);
    }

    {
        int fileIndex = PokeIcon64KAnimationFileIndex();
        SpriteSystem_LoadAnimResObj(spriteSys, spriteMan, 19, fileIndex, 0, 18000);
    }
}

void BallCapsuleSystem_LoadPartyIcons(BallCapsuleSystem *ballCapsuleSys)
{
    // int v1;
    // int v2;
    // int v3;
    // int v4;
    Pokemon *mon;
    SpriteTemplate spriteTemplate;

    for (int i = 0; i < ballCapsuleSys->appData->partyCount; i++) {
        mon = ballCapsuleSys->appData->mons[i];

        SpriteSystem_LoadCharResObjAtEndWithHardwareMappingType(ballCapsuleSys->ballCapsuleEditor.spriteSys, ballCapsuleSys->ballCapsuleEditor.spriteMan, NARC_INDEX_POKETOOL__ICONGRA__PL_POKE_ICON, Pokemon_IconSpriteIndex(mon), FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, i + 15000);

        spriteTemplate.x = 0;
        spriteTemplate.y = 0;
        spriteTemplate.z = 0;
        spriteTemplate.animIdx = 0;
        spriteTemplate.priority = 10;
        spriteTemplate.plttIdx = 0;
        spriteTemplate.vramType = NNS_G2D_VRAM_TYPE_2DMAIN;
        spriteTemplate.bgPriority = 2;
        spriteTemplate.vramTransfer = FALSE;
        spriteTemplate.resources[0] = (i + 15000);
        spriteTemplate.resources[1] = 16000;
        spriteTemplate.resources[2] = 17000;
        spriteTemplate.resources[3] = 18000;
        spriteTemplate.resources[4] = SPRITE_RESOURCE_NONE;
        spriteTemplate.resources[5] = SPRITE_RESOURCE_NONE;

        ballCapsuleSys->partyIcons[i] = SpriteSystem_NewSprite(ballCapsuleSys->ballCapsuleEditor.spriteSys, ballCapsuleSys->ballCapsuleEditor.spriteMan, &spriteTemplate);

        int species = Pokemon_GetValue(mon, MON_DATA_SPECIES, NULL);
        int isEgg = Pokemon_GetValue(mon, MON_DATA_IS_EGG, NULL);
        int form = Pokemon_GetValue(mon, MON_DATA_FORM, NULL);
        int palleteIndex = PokeIconPaletteIndex(species, form, isEgg);

        Sprite_SetExplicitPaletteOffsetAutoAdjust(ballCapsuleSys->partyIcons[i]->sprite, palleteIndex);
        ManagedSprite_SetAnim(ballCapsuleSys->partyIcons[i], 1);
    }
}

void BallCapsuleSystem_UpdatePartyIconPositions(BallCapsuleSystem *ballCapsuleSys)
{
    int i;

    for (i = 0; i < ballCapsuleSys->appData->partyCount; i++) {
        ManagedSprite_SetDrawFlag(ballCapsuleSys->partyIcons[i], FALSE);
    }

    for (i = 0; i < TOTAL_CAPSULES; i++) {
        int index = ballCapsuleSys->capsuleData[i].partyIndex;

        if (index != BALL_CAPSULE_INVALID_PARTY_INDEX) {
            s16 x, y;
            GetCapsulePosition(i, &x, &y);
            ManagedSprite_SetPositionXY(ballCapsuleSys->partyIcons[index], x + -16, y + (+12));
            ManagedSprite_SetDrawFlag(ballCapsuleSys->partyIcons[index], TRUE);
        }
    }
}

void BallCapsuleSystem_TickPartyIcons(BallCapsuleSystem *ballCapsuleSys)
{
    for (int i = 0; i < ballCapsuleSys->appData->partyCount; i++) {
        ManagedSprite_TickFrame(ballCapsuleSys->partyIcons[i]);
    }
}

void BallCapsuleSystem_DeletePartyIcons(BallCapsuleSystem *ballCapsuleSys)
{
    for (int i = 0; i < ballCapsuleSys->appData->partyCount; i++) {
        Sprite_DeleteAndFreeResources(ballCapsuleSys->partyIcons[i]);
    }
}

void BallCapsuleSystem_UpdateEditData(BallCapsuleSystem *ballCapsuleSys)
{

    for (int i = 0; i < TOTAL_CAPSULES; i++) {
        ballCapsuleSys->editData[i].hasSeals = BallCapsule_ContainsSeals(ballCapsuleSys->capsuleData[i].ballCapsule);
        ballCapsuleSys->editData[i].partyIndex = ballCapsuleSys->capsuleData[i].partyIndex;

        if ((ballCapsuleSys->editData[i].hasSeals == 0) && (ballCapsuleSys->editData[i].partyIndex != BALL_CAPSULE_INVALID_PARTY_INDEX)) {
            BallCapsuleSystem_UnsetCapsule(ballCapsuleSys, i);
        }
    }
}

void ov76_0223C398(BallCapsuleEditor *ballCapsuleEditor)
{
    ballCapsuleEditor->spriteSys = SpriteSystem_Alloc(HEAP_ID_BALL_CAPSULE_SYSTEM);
    {
        const RenderOamTemplate v0 = {
            0,
            128,
            0,
            32,
            0,
            128,
            0,
            32,
        };
        const CharTransferTemplateWithModes v1 = {
            48 + 48, 1024 * 0x40, 512 * 0x20, GX_OBJVRAMMODE_CHAR_1D_64K, GX_OBJVRAMMODE_CHAR_1D_32K
        };

        SpriteSystem_Init(ballCapsuleEditor->spriteSys, &v0, &v1, 16 + 16);
    }

    {
        BOOL v2;
        const SpriteResourceCapacities v3 = {
            48 + 48,
            16 + 16,
            64,
            64,
            16,
            16,
        };

        ballCapsuleEditor->spriteMan = SpriteManager_New(ballCapsuleEditor->spriteSys);
        v2 = SpriteSystem_InitSprites(ballCapsuleEditor->spriteSys, ballCapsuleEditor->spriteMan, 64 + 64);
        GF_ASSERT(v2);

        v2 = SpriteSystem_InitManagerWithCapacities(ballCapsuleEditor->spriteSys, ballCapsuleEditor->spriteMan, &v3);
        GF_ASSERT(v2);
    }
}

void ov76_0223C424(BallCapsuleEditor *ballCapsuleEditor)
{
    SpriteSystem_FreeResourcesAndManager(ballCapsuleEditor->spriteSys, ballCapsuleEditor->spriteMan);
    SpriteSystem_Free(ballCapsuleEditor->spriteSys);
}

void ov76_0223C438(BallCapsuleSystem *ballCapsuleSys, NARC *param1)
{
    SpriteSystem_LoadCharResObjFromOpenNarc(ballCapsuleSys->ballCapsuleEditor.spriteSys, ballCapsuleSys->ballCapsuleEditor.spriteMan, param1, 265, TRUE, NNS_G2D_VRAM_TYPE_2DSUB, 45000);
    SpriteSystem_LoadCellResObjFromOpenNarc(ballCapsuleSys->ballCapsuleEditor.spriteSys, ballCapsuleSys->ballCapsuleEditor.spriteMan, param1, 173, TRUE, 47000);
    SpriteSystem_LoadAnimResObjFromOpenNarc(ballCapsuleSys->ballCapsuleEditor.spriteSys, ballCapsuleSys->ballCapsuleEditor.spriteMan, param1, 81, TRUE, 48000);
}

void ov76_0223C4AC(BallCapsuleSystem *ballCapsuleSys)
{
    int v0;
    SpriteTemplate v1;
    int v2[][3] = {
        { 136, 16, 1 },
        { 136, 128, 3 },
        { 240, 16, 0 },
        { 240, 128, 2 },
    };

    for (v0 = 0; v0 < 4; v0++) {
        v1.x = v2[v0][0];
        v1.y = v2[v0][1];
        v1.z = 0;
        v1.animIdx = 0;
        v1.priority = 40;
        v1.plttIdx = 2;
        v1.vramType = NNS_G2D_VRAM_TYPE_2DSUB;
        v1.bgPriority = 1;
        v1.vramTransfer = FALSE;
        v1.resources[0] = 45000;
        v1.resources[1] = 26000 + 290;
        v1.resources[2] = 47000;
        v1.resources[3] = 48000;
        v1.resources[4] = SPRITE_RESOURCE_NONE;
        v1.resources[5] = SPRITE_RESOURCE_NONE;
        ballCapsuleSys->unk_314[v0] = SpriteSystem_NewSprite(ballCapsuleSys->ballCapsuleEditor.spriteSys, ballCapsuleSys->ballCapsuleEditor.spriteMan, &v1);

        ManagedSprite_SetFlipMode(ballCapsuleSys->unk_314[v0], v2[v0][2]);
    }
}

void ov76_0223C544(BallCapsuleSystem *ballCapsuleSys)
{
    if (ballCapsuleSys->ballCapsuleEditor.unk_04 == 0) {
        return;
    }

    for (int i = 0; i < 4; i++) {
        ManagedSprite_TickFrame(ballCapsuleSys->unk_314[i]);
    }
}

void ov76_0223C568(BallCapsuleSystem *ballCapsuleSys, int param1)
{
    for (int i = 0; i < 4; i++) {
        ManagedSprite_SetDrawFlag(ballCapsuleSys->unk_314[i], param1);
    }
}

void ov76_0223C588(BallCapsuleSystem *ballCapsuleSys)
{
    for (int i = 0; i < 4; i++) {
        Sprite_DeleteAndFreeResources(ballCapsuleSys->unk_314[i]);
    }
}

void ov76_0223C5A4(SpriteSystem *param0, SpriteManager *param1, PaletteData *param2, int param3, int param4, int param5, int param6, int param7, int param8, int param9, NARC *param10)
{
    SpriteSystem_LoadCharResObjFromOpenNarc(param0, param1, param10, param3, TRUE, param7, param3 + 11000);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(param2, param8, param0, param1, param10, param4, FALSE, param9, param7, param4 + 11000);
    SpriteSystem_LoadCellResObjFromOpenNarc(param0, param1, param10, param5, TRUE, param5 + 11000);
    SpriteSystem_LoadAnimResObjFromOpenNarc(param0, param1, param10, param6, TRUE, param6 + 11000);
}

void ov76_0223C61C(BallCapsuleSystem *ballCapsuleSys, NARC *param1)
{
    ov76_0223C5A4(ballCapsuleSys->ballCapsuleEditor.spriteSys, ballCapsuleSys->ballCapsuleEditor.spriteMan, ballCapsuleSys->ballCapsuleEditor.paletteData, 266, 286, 174, 82, NNS_G2D_VRAM_TYPE_2DMAIN, 2, 1, param1);
    ov76_0223C5A4(ballCapsuleSys->ballCapsuleEditor.spriteSys, ballCapsuleSys->ballCapsuleEditor.spriteMan, ballCapsuleSys->ballCapsuleEditor.paletteData, 275, 291, 179, 87, NNS_G2D_VRAM_TYPE_2DMAIN, 2, 1, param1);
    {
        SpriteTemplate v1;

        for (int i = 0; i < TOTAL_CAPSULES; i++) {
            v1.x = 0;
            v1.y = 0;
            v1.z = 0;
            v1.animIdx = ballCapsuleSys->editData[i].hasSeals;
            v1.priority = 40 - i;
            v1.plttIdx = 0;
            v1.vramType = NNS_G2D_VRAM_TYPE_2DMAIN;
            v1.bgPriority = 2;
            v1.vramTransfer = FALSE;
            v1.resources[0] = (266 + 11000);
            v1.resources[1] = (286 + 11000);
            v1.resources[2] = (174 + 11000);
            v1.resources[3] = (82 + 11000);
            v1.resources[4] = SPRITE_RESOURCE_NONE;
            v1.resources[5] = SPRITE_RESOURCE_NONE;

            ballCapsuleSys->editData[i].sprite = SpriteSystem_NewSprite(ballCapsuleSys->ballCapsuleEditor.spriteSys, ballCapsuleSys->ballCapsuleEditor.spriteMan, &v1);
            {
                s16 v2, v3;

                GetCapsulePosition(i, &v2, &v3);
                ManagedSprite_SetPositionXY(ballCapsuleSys->editData[i].sprite, v2, v3);
            }
        }

        v1.x = 0;
        v1.y = 0;
        v1.z = 0;
        v1.animIdx = 0;
        v1.priority = 20;
        v1.plttIdx = 0;
        v1.vramType = NNS_G2D_VRAM_TYPE_2DMAIN;
        v1.bgPriority = 2;
        v1.vramTransfer = FALSE;
        v1.resources[0] = (275 + 11000);
        v1.resources[1] = (291 + 11000);
        v1.resources[2] = (179 + 11000);
        v1.resources[3] = (87 + 11000);
        v1.resources[4] = SPRITE_RESOURCE_NONE;
        v1.resources[5] = SPRITE_RESOURCE_NONE;

        ballCapsuleSys->unk_2F4[0] = SpriteSystem_NewSprite(ballCapsuleSys->ballCapsuleEditor.spriteSys, ballCapsuleSys->ballCapsuleEditor.spriteMan, &v1);
        ballCapsuleSys->unk_2F4[1] = SpriteSystem_NewSprite(ballCapsuleSys->ballCapsuleEditor.spriteSys, ballCapsuleSys->ballCapsuleEditor.spriteMan, &v1);

        {
            s16 v4, v5;

            GetCapsulePosition(ballCapsuleSys->selectedCapsules[0], &v4, &v5);
            ManagedSprite_SetPositionXY(ballCapsuleSys->unk_2F4[0], v4, v5);
            ManagedSprite_SetPositionXY(ballCapsuleSys->unk_2F4[1], v4, v5);
            ManagedSprite_SetPriority(ballCapsuleSys->unk_2F4[1], 25);
            ManagedSprite_SetAnim(ballCapsuleSys->unk_2F4[0], 0);
            ManagedSprite_SetAnim(ballCapsuleSys->unk_2F4[1], 0);
        }
    }
}

void BallCapsuleSystem_UpdateCapsuleData(BallCapsuleSystem *ballCapsuleSys)
{
    BallCapsuleSystem_UpdateEditData(ballCapsuleSys);
    BallCapsuleSystem_UpdatePartyIconPositions(ballCapsuleSys);

    for (int i = 0; i < TOTAL_CAPSULES; i++) {
        if (ballCapsuleSys->editData[i].sprite == NULL) {
            continue;
        }

        ManagedSprite_SetAnim(ballCapsuleSys->editData[i].sprite, ballCapsuleSys->editData[i].hasSeals);
    }
}

void BallCapsuleSys_SwapCapsules(BallCapsuleSystem *ballCapsuleSys, int indexA, int indexB)
{
    int partyA = ballCapsuleSys->capsuleData[indexA].partyIndex;
    int partyB = ballCapsuleSys->capsuleData[indexB].partyIndex;

    // Swap capsule indices on the pokemon
    if (partyA != BALL_CAPSULE_INVALID_PARTY_INDEX) {
        int pokePartyA = indexB + 1;
        Pokemon_SetValue(ballCapsuleSys->appData->mons[partyA], MON_DATA_BALL_CAPSULE_ID, (u8 *)&pokePartyA);
    }

    if (partyB != BALL_CAPSULE_INVALID_PARTY_INDEX) {
        int pokePartyB = indexA + 1;
        Pokemon_SetValue(ballCapsuleSys->appData->mons[partyB], MON_DATA_BALL_CAPSULE_ID, (u8 *)&pokePartyB);
    }

    // Swap indices
    int tempIndex = ballCapsuleSys->capsuleData[indexA].partyIndex;
    ballCapsuleSys->capsuleData[indexA].partyIndex = ballCapsuleSys->capsuleData[indexB].partyIndex;
    ballCapsuleSys->capsuleData[indexB].partyIndex = tempIndex;

    // Swap capsules
    BallCapsule tempCapsule;
    BallCapsule_Copy(ballCapsuleSys->capsuleData[indexA].ballCapsule, &tempCapsule);
    BallCapsule_Copy(ballCapsuleSys->capsuleData[indexB].ballCapsule, ballCapsuleSys->capsuleData[indexA].ballCapsule);
    BallCapsule_Copy(&tempCapsule, ballCapsuleSys->capsuleData[indexB].ballCapsule);
    BallCapsuleSystem_UpdateCapsuleData(ballCapsuleSys);
}

void BallCapsuleSystem_TickSprites(BallCapsuleSystem *ballCapsuleSys)
{
    for (int i = 0; i < TOTAL_CAPSULES; i++) {
        ManagedSprite_TickFrame(ballCapsuleSys->editData[i].sprite);
    }

    ManagedSprite_TickFrame(ballCapsuleSys->unk_2F4[0]);
    ManagedSprite_TickFrame(ballCapsuleSys->unk_2F4[1]);
}

void BallCapsuleSystem_DeleteSprites(BallCapsuleSystem *ballCapsuleSys)
{
    for (int i = 0; i < TOTAL_CAPSULES; i++) {
        Sprite_DeleteAndFreeResources(ballCapsuleSys->editData[i].sprite);
    }

    Sprite_DeleteAndFreeResources(ballCapsuleSys->unk_2F4[0]);
    Sprite_DeleteAndFreeResources(ballCapsuleSys->unk_2F4[1]);
}

void ov76_0223C8EC(BgConfig *ballCapsuleSys, PaletteData *param1, int param2)
{
    LoadMessageBoxGraphics(ballCapsuleSys, BG_LAYER_MAIN_1, 1, 15, param2, HEAP_ID_BALL_CAPSULE_SYSTEM);
    PaletteData_LoadBufferFromFileStart(param1, 38, GetMessageBoxPaletteNARCMember(param2), HEAP_ID_BALL_CAPSULE_SYSTEM, 0, 0x20, 12 * 16);
    LoadStandardWindowGraphics(ballCapsuleSys, BG_LAYER_MAIN_1, 1 + (18 + 12), 13, 0, HEAP_ID_BALL_CAPSULE_SYSTEM);
    PaletteData_LoadBufferFromFileStart(param1, 38, GetStandardWindowPaletteNARCMember(), HEAP_ID_BALL_CAPSULE_SYSTEM, 0, 0x20, 13 * 16);
    PaletteData_LoadBufferFromFileStart(param1, 14, 7, HEAP_ID_BALL_CAPSULE_SYSTEM, 0, 0x20, 14 * 16);
}

void ov76_0223C974(BgConfig *param0, PaletteData *param1, int param2)
{
    LoadMessageBoxGraphics(param0, BG_LAYER_SUB_0, 1, 15, param2, HEAP_ID_BALL_CAPSULE_SYSTEM);
    PaletteData_LoadBufferFromFileStart(param1, 38, GetMessageBoxPaletteNARCMember(param2), HEAP_ID_BALL_CAPSULE_SYSTEM, 1, 0x20, 12 * 16);
    LoadStandardWindowGraphics(param0, BG_LAYER_SUB_0, 1 + (18 + 12), 13, 0, HEAP_ID_BALL_CAPSULE_SYSTEM);
    PaletteData_LoadBufferFromFileStart(param1, 38, GetStandardWindowPaletteNARCMember(), HEAP_ID_BALL_CAPSULE_SYSTEM, 1, 0x20, 13 * 16);
    PaletteData_LoadBufferFromFileStart(param1, 14, 7, HEAP_ID_BALL_CAPSULE_SYSTEM, 1, 0x20, 14 * 16);
    PaletteData_LoadBufferFromFileStart(param1, 14, 7, HEAP_ID_BALL_CAPSULE_SYSTEM, 1, 0x20, 3 * 16);
    PaletteData_LoadBufferFromFileStart(param1, 91, 294, HEAP_ID_BALL_CAPSULE_SYSTEM, 1, 0x20, 11 * 16);
}

void Window_SetMessage(Window *window, int messageID)
{
    MessageLoader *messageLoader;
    String *message;

    if (messageID == 0xFFFF) {
        Window_FillTilemap(window, 15);
        Window_CopyToVRAM(window);
        return;
    }

    messageLoader = MessageLoader_Init(MSG_LOADER_PRELOAD_ENTIRE_BANK, NARC_INDEX_MSGDATA__PL_MSG, TEXT_BANK_UNK_0008, HEAP_ID_BALL_CAPSULE_SYSTEM);
    message = MessageLoader_GetNewString(messageLoader, messageID);

    Window_FillTilemap(window, 15);
    Text_AddPrinterWithParams(window, FONT_MESSAGE, message, 0, 0, TEXT_SPEED_INSTANT, NULL);
    Window_CopyToVRAM(window);
    String_Free(message);
    MessageLoader_Free(messageLoader);
}

void CreateBasicWindow(BgConfig *param0, Window *param1, int param2, int param3, int param4, int param5, int param6, int param7)
{
    Window_Init(param1);
    Window_Add(param0, param1, param2, param3, param4, param5, param6, 14, param7);
    Window_DrawMessageBoxWithScrollCursor(param1, 1, 1, 12);
    Window_FillTilemap(param1, 15);
    Window_CopyToVRAM(param1);
}

static void ov76_0223CAFC(BgConfig *param0, Window *param1, int param2, int param3, int param4, int param5, int param6, int param7, int param8)
{
    Window_Init(param1);
    Window_Add(param0, param1, param2, param3, param4, param5, param6, param8, param7);
    Window_FillTilemap(param1, 15);
    Window_CopyToVRAM(param1);
}

void ov76_0223CB58(BallCapsuleSystem *ballCapsuleSys)
{
    ov76_0223CAFC(ballCapsuleSys->ballCapsuleEditor.bgConfig, &ballCapsuleSys->ballCapsuleEditor.unk_18[3], 6, 4 - 1, 2, 3 + 1, 2, (27 * 4) + (0 + ((1 + (18 + 12)) + 9)), 11);
    ov76_0223CAFC(ballCapsuleSys->ballCapsuleEditor.bgConfig, &ballCapsuleSys->ballCapsuleEditor.unk_18[4], 6, 11 - 1, 2, 3 + 1, 2, ((3 + 1) * 2) + ((27 * 4) + (0 + ((1 + (18 + 12)) + 9))), 11);
    ov76_0223CAFC(ballCapsuleSys->ballCapsuleEditor.bgConfig, &ballCapsuleSys->ballCapsuleEditor.unk_18[5], 6, 4 - 1, 5, 3 + 1, 2, ((3 + 1) * 2) + (((3 + 1) * 2) + ((27 * 4) + (0 + ((1 + (18 + 12)) + 9)))), 11);
    ov76_0223CAFC(ballCapsuleSys->ballCapsuleEditor.bgConfig, &ballCapsuleSys->ballCapsuleEditor.unk_18[6], 6, 11 - 1, 5, 3 + 1, 2, ((3 + 1) * 2) + (((3 + 1) * 2) + (((3 + 1) * 2) + ((27 * 4) + (0 + ((1 + (18 + 12)) + 9))))), 11);
    ov76_0223CAFC(ballCapsuleSys->ballCapsuleEditor.bgConfig, &ballCapsuleSys->ballCapsuleEditor.unk_18[7], 6, 4 - 1, 8, 3 + 1, 2, ((3 + 1) * 2) + (((3 + 1) * 2) + (((3 + 1) * 2) + (((3 + 1) * 2) + ((27 * 4) + (0 + ((1 + (18 + 12)) + 9)))))), 11);
    ov76_0223CAFC(ballCapsuleSys->ballCapsuleEditor.bgConfig, &ballCapsuleSys->ballCapsuleEditor.unk_18[8], 6, 11 - 1, 8, 3 + 1, 2, ((3 + 1) * 2) + (((3 + 1) * 2) + (((3 + 1) * 2) + (((3 + 1) * 2) + (((3 + 1) * 2) + ((27 * 4) + (0 + ((1 + (18 + 12)) + 9))))))), 11);
    ov76_0223CAFC(ballCapsuleSys->ballCapsuleEditor.bgConfig, &ballCapsuleSys->ballCapsuleEditor.unk_18[9], 6, 4 - 1, 11, 3 + 1, 2, ((3 + 1) * 2) + (((3 + 1) * 2) + (((3 + 1) * 2) + (((3 + 1) * 2) + (((3 + 1) * 2) + (((3 + 1) * 2) + ((27 * 4) + (0 + ((1 + (18 + 12)) + 9)))))))), 11);
    ov76_0223CAFC(ballCapsuleSys->ballCapsuleEditor.bgConfig, &ballCapsuleSys->ballCapsuleEditor.unk_18[10], 6, 11 - 1, 11, 3 + 1, 2, ((3 + 1) * 2) + (((3 + 1) * 2) + (((3 + 1) * 2) + (((3 + 1) * 2) + (((3 + 1) * 2) + (((3 + 1) * 2) + (((3 + 1) * 2) + ((27 * 4) + (0 + ((1 + (18 + 12)) + 9))))))))), 11);
}

void ov76_0223CC8C(BallCapsuleSystem *ballCapsuleSys)
{
    int v0;
    int v1;
    int v2;
    int v3;
    String *v4;
    Window *v5;

    for (v0 = 3; v0 < 11; v0++) {
        v5 = &ballCapsuleSys->ballCapsuleEditor.unk_18[v0];
        v1 = v0 - 3;
        v2 = ballCapsuleSys->sealCasePages.currentPageSeals[v1];

        if (v2 == 0) {
            Window_FillTilemap(v5, 0xEE);
            Window_CopyToVRAM(v5);
            continue;
        }

        Window_FillTilemap(v5, 0x0);

        v4 = String_Init(100, HEAP_ID_BALL_CAPSULE_SYSTEM);
        v3 = SealCase_GetSealCount(ballCapsuleSys->sealCounts, v2 - 1);

        String_FormatInt(v4, v3, 3, 1, 1);
        Text_AddPrinterWithParamsAndColor(v5, FONT_SYSTEM, v4, -1 + 8, 0, TEXT_SPEED_NO_TRANSFER, TEXT_COLOR(1, 2, 0), NULL);
        Window_CopyToVRAM(v5);
        String_Free(v4);
    }
}

void ov76_0223CD20(BallCapsuleSystem *ballCapsuleSys, int param1)
{
    int v0;
    int v1;
    int v2;
    int v3;
    String *v4;
    Window *v5 = &ballCapsuleSys->ballCapsuleEditor.unk_18[param1 + 3];
    v1 = param1;
    v2 = ballCapsuleSys->sealCasePages.currentPageSeals[v1];

    if (v2 == 0) {
        Window_FillTilemap(v5, 0xEE);
        Window_CopyToVRAM(v5);
        return;
    }

    Window_FillTilemap(v5, 0x0);

    v4 = String_Init(100, HEAP_ID_BALL_CAPSULE_SYSTEM);
    v3 = SealCase_GetSealCount(ballCapsuleSys->sealCounts, v2 - 1);

    String_FormatInt(v4, v3, 3, 1, 1);
    Text_AddPrinterWithParamsAndColor(v5, FONT_MESSAGE, v4, -1 + 8, 0, TEXT_SPEED_NO_TRANSFER, TEXT_COLOR(1, 2, 0), NULL);
    Window_CopyToVRAM(v5);
    String_Free(v4);
}

void ov76_0223CDA4(BallCapsuleSystem *ballCapsuleSys)
{
    int v0;

    for (v0 = 3; v0 < 11; v0++) {
        Window_ClearAndCopyToVRAM(&ballCapsuleSys->ballCapsuleEditor.unk_18[v0]);
        Window_Remove(&ballCapsuleSys->ballCapsuleEditor.unk_18[v0]);
    }
}

void ov76_0223CDC4(Window *param0, int param1)
{
    MessageLoader *v0;
    String *v1;

    if (param1 == 0xFFFF) {
        Window_FillTilemap(param0, 15);
        Window_CopyToVRAM(param0);
        return;
    }

    v0 = MessageLoader_Init(MSG_LOADER_PRELOAD_ENTIRE_BANK, NARC_INDEX_MSGDATA__PL_MSG, TEXT_BANK_BALL_SEAL_NAMES, HEAP_ID_BALL_CAPSULE_SYSTEM);
    v1 = MessageLoader_GetNewString(v0, param1);

    Window_FillTilemap(param0, 15);
    Text_AddPrinterWithParams(param0, FONT_MESSAGE, v1, 0, 0, TEXT_SPEED_INSTANT, NULL);
    Window_CopyToVRAM(param0);
    String_Free(v1);
    MessageLoader_Free(v0);
}

void ov76_0223CE2C(void)
{
    GXLayers_TurnBothDispOn();
    GXLayers_EngineAToggleLayers(GX_PLANEMASK_OBJ, 1);
    GXLayers_EngineBToggleLayers(GX_PLANEMASK_OBJ, 1);
}

void ov76_0223CE44(void)
{
    StartScreenFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, COLOR_BLACK, 6, 1, HEAP_ID_BALL_CAPSULE_SYSTEM);
}

void ov76_0223CE64(void)
{
    StartScreenFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, COLOR_BLACK, 6, 1, HEAP_ID_BALL_CAPSULE_SYSTEM);
}

void ov76_0223CE84(BallCapsuleSystem *ballCapsuleSys, NARC *param1)
{
    int v0 = 91;
    int v1 = 268;
    int v2 = 284;
    int v3 = 288;
    int v4 = 2;

    Graphics_LoadTilesToBgLayerFromOpenNARC(param1, v1, ballCapsuleSys->ballCapsuleEditor.bgConfig, v4, 0, 0, 1, HEAP_ID_BALL_CAPSULE_SYSTEM);
    Graphics_LoadTilemapToBgLayerFromOpenNARC(param1, v2, ballCapsuleSys->ballCapsuleEditor.bgConfig, v4, 0, 0, 1, HEAP_ID_BALL_CAPSULE_SYSTEM);
    PaletteData_LoadBufferFromFileStart(ballCapsuleSys->ballCapsuleEditor.paletteData, v0, v3, HEAP_ID_BALL_CAPSULE_SYSTEM, 0, 0x20 * 2, 0);

    v1 = 269;
    v2 = 285;
    v4 = 3;

    Graphics_LoadTilesToBgLayerFromOpenNARC(param1, v1, ballCapsuleSys->ballCapsuleEditor.bgConfig, v4, 0, 0, 1, HEAP_ID_BALL_CAPSULE_SYSTEM);
    Graphics_LoadTilemapToBgLayerFromOpenNARC(param1, v2, ballCapsuleSys->ballCapsuleEditor.bgConfig, v4, 0, 0, 1, HEAP_ID_BALL_CAPSULE_SYSTEM);
}

void ov76_0223CF24(BallCapsuleSystem *ballCapsuleSys, NARC *param1)
{
    int v0 = 91;
    int v1 = 267;
    int v2 = 283;
    int v3 = 287;
    int v4 = 5;

    Graphics_LoadTilesToBgLayerFromOpenNARC(param1, v1, ballCapsuleSys->ballCapsuleEditor.bgConfig, v4, 0, 0, 1, HEAP_ID_BALL_CAPSULE_SYSTEM);
    Graphics_LoadTilemapToBgLayerFromOpenNARC(param1, v2, ballCapsuleSys->ballCapsuleEditor.bgConfig, v4, 0, 0, 1, HEAP_ID_BALL_CAPSULE_SYSTEM);
    PaletteData_LoadBufferFromFileStart(ballCapsuleSys->ballCapsuleEditor.paletteData, v0, v3, HEAP_ID_BALL_CAPSULE_SYSTEM, 1, 0x20 * 2, 0);
}

void ov76_0223CF88(BallCapsuleSystem *ballCapsuleSys, NARC *param1)
{
    int v0 = 91;
    int v1 = 267;
    int v2 = 282;
    int v3 = 287;
    int v4 = 7;

    Graphics_LoadTilesToBgLayerFromOpenNARC(param1, v1, ballCapsuleSys->ballCapsuleEditor.bgConfig, v4, 0, 0, 1, HEAP_ID_BALL_CAPSULE_SYSTEM);
    Graphics_LoadTilemapToBgLayerFromOpenNARC(param1, v2, ballCapsuleSys->ballCapsuleEditor.bgConfig, v4, 0, 0, 1, HEAP_ID_BALL_CAPSULE_SYSTEM);
    PaletteData_LoadBufferFromFileStart(ballCapsuleSys->ballCapsuleEditor.paletteData, v0, v3, HEAP_ID_BALL_CAPSULE_SYSTEM, 1, 0x20 * 2, 0);
}

void ov76_0223CFEC(BallCapsuleSystem *ballCapsuleSys, NARC *param1)
{
    SpriteSystem *v0 = ballCapsuleSys->ballCapsuleEditor.spriteSys;
    SpriteManager *v1 = ballCapsuleSys->ballCapsuleEditor.spriteMan;
    PaletteData *v2 = ballCapsuleSys->ballCapsuleEditor.paletteData;

    SpriteSystem_LoadPaletteBufferFromOpenNarc(v2, PLTTBUF_SUB_OBJ, v0, v1, param1, 290, FALSE, 3, NNS_G2D_VRAM_TYPE_2DSUB, 26000 + 290);
    SpriteSystem_LoadCharResObjFromOpenNarc(v0, v1, param1, 273, TRUE, NNS_G2D_VRAM_TYPE_2DSUB, 25000 + 273);
    SpriteSystem_LoadCellResObjFromOpenNarc(v0, v1, param1, 177, TRUE, 27000 + 177);
    SpriteSystem_LoadAnimResObjFromOpenNarc(v0, v1, param1, 85, TRUE, 28000 + 85);
    SpriteSystem_LoadCharResObjFromOpenNarc(v0, v1, param1, 274, TRUE, NNS_G2D_VRAM_TYPE_2DSUB, 25000 + 274);
    SpriteSystem_LoadCellResObjFromOpenNarc(v0, v1, param1, 178, TRUE, 27000 + 178);
    SpriteSystem_LoadAnimResObjFromOpenNarc(v0, v1, param1, 86, TRUE, 28000 + 86);
    SpriteSystem_LoadCharResObjFromOpenNarc(v0, v1, param1, 270, TRUE, NNS_G2D_VRAM_TYPE_2DSUB, 25000 + 270);
    SpriteSystem_LoadCellResObjFromOpenNarc(v0, v1, param1, 175, TRUE, 27000 + 175);
    SpriteSystem_LoadAnimResObjFromOpenNarc(v0, v1, param1, 83, TRUE, 28000 + 83);
    SpriteSystem_LoadCharResObjFromOpenNarc(v0, v1, param1, 272, TRUE, NNS_G2D_VRAM_TYPE_2DSUB, 35000 + 272);
    SpriteSystem_LoadCellResObjFromOpenNarc(v0, v1, param1, 176, TRUE, 27000 + 176);
    SpriteSystem_LoadAnimResObjFromOpenNarc(v0, v1, param1, 84, TRUE, 28000 + 84);
}

void ov76_0223D16C(BallCapsuleSystem *ballCapsuleSys)
{
    int v0;
    BallCapsuleEditorSprites *v1;
    SpriteTemplate v2;
    SpriteSystem *v3 = ballCapsuleSys->ballCapsuleEditor.spriteSys;
    SpriteManager *v4 = ballCapsuleSys->ballCapsuleEditor.spriteMan;
    PaletteData *v5 = ballCapsuleSys->ballCapsuleEditor.paletteData;
    v1 = &ballCapsuleSys->editorSprites;

    v2.x = 0;
    v2.y = 0;
    v2.z = 0;
    v2.animIdx = 0;
    v2.priority = 60;
    v2.vramType = NNS_G2D_VRAM_TYPE_2DSUB;
    v2.bgPriority = 1;
    v2.vramTransfer = FALSE;
    v2.resources[4] = SPRITE_RESOURCE_NONE;
    v2.resources[5] = SPRITE_RESOURCE_NONE;

    v0 = 8;

    v2.plttIdx = 1;
    v2.resources[0] = 25000 + 273;
    v2.resources[1] = 26000 + 290;
    v2.resources[2] = 27000 + 177;
    v2.resources[3] = 28000 + 85;
    v1->sprites[v0++] = SpriteSystem_NewSprite(v3, v4, &v2);

    v2.resources[0] = 25000 + 274;
    v2.resources[1] = 26000 + 290;
    v2.resources[2] = 27000 + 178;
    v2.resources[3] = 28000 + 86;
    v1->sprites[v0++] = SpriteSystem_NewSprite(v3, v4, &v2);

    v2.bgPriority = 1;
    v2.plttIdx = 1;
    v2.resources[0] = 35000 + 272;
    v2.resources[1] = 26000 + 290;
    v2.resources[2] = 27000 + 176;
    v2.resources[3] = 28000 + 84;
    v1->sprites[v0++] = SpriteSystem_NewSprite(v3, v4, &v2);

    v2.bgPriority = 1;
    v2.plttIdx = 0;
    v2.resources[0] = 25000 + 270;
    v2.resources[1] = 26000 + 290;
    v2.resources[2] = 27000 + 175;
    v2.resources[3] = 28000 + 83;
    v1->sprites[v0++] = SpriteSystem_NewSprite(v3, v4, &v2);
    v1->sprites[v0++] = SpriteSystem_NewSprite(v3, v4, &v2);

    {
        const s16 v6[][2] = {
            { 20, 23 },
            { 20, 47 },
            { 20, 71 },
            { 20, 95 },
            { 76, 23 },
            { 76, 47 },
            { 76, 71 },
            { 76, 95 },
            { 27, 124 },
            { 67, 124 },
            { 32, 171 },
            { 104, 171 },
            { 192, 171 },
        };
        const s16 v7[][2] = {
            { 0xFF, 0xFF },
            { 0xFF, 0xFF },
            { 0xFF, 0xFF },
            { 0xFF, 0xFF },
            { 0xFF, 0xFF },
            { 0xFF, 0xFF },
            { 0xFF, 0xFF },
            { 0xFF, 0xFF },
            { 13, 10 },
            { 13, 10 },
            { 16, 12 },
            { 28, 12 },
            { 28, 12 },
        };

        for (v0 = 8; v0 < 13; v0++) {
            ManagedSprite_SetPositionXY(ballCapsuleSys->editorSprites.sprites[v0], v6[v0][0], v6[v0][1]);
            ManagedSprite_TickFrame(ballCapsuleSys->editorSprites.sprites[v0]);
            ManagedSprite_SetAnimationFrame(ballCapsuleSys->editorSprites.sprites[v0], 0);
            Rect_SetDimensions(&ballCapsuleSys->ballCapsuleEditor.buttonRects[v0], ballCapsuleSys->editorSprites.sprites[v0], v7[v0][0], v7[v0][1]);
        }
    }
}

void ov76_0223D2F4(BallCapsuleSystem *ballCapsuleSys, int param1)
{
    int v0;

    for (v0 = 0; v0 < 13; v0++) {
        if (ballCapsuleSys->editorSprites.sprites[v0] == NULL) {
            continue;
        }

        ManagedSprite_SetDrawFlag(ballCapsuleSys->editorSprites.sprites[v0], param1);
    }
}

void ov76_0223D318(BallCapsuleSystem *ballCapsuleSys)
{
    int v0;

    for (v0 = 0; v0 < 13; v0++) {
        (void)0;
    }
}

void ov76_0223D31C(BallCapsuleSystem *ballCapsuleSys)
{
    int v0;

    for (v0 = 8; v0 < 13; v0++) {
        Sprite_DeleteAndFreeResources(ballCapsuleSys->editorSprites.sprites[v0]);
    }
}
