#include "unk_02097B18.h"

#include <nitro.h>
#include <string.h>

#include "constants/heap.h"
#include "constants/pokemon.h"

#include "struct_decls/struct_0202440C_decl.h"
#include "struct_defs/seal_case.h"
#include "struct_defs/struct_02097F18.h"

#include "applications/party_menu/defs.h"
#include "applications/party_menu/main.h"
#include "field/field_system.h"
#include "overlay076/ov76_0223B140.h"
#include "overlay076/ov76_0223B870.h"
#include "overlay076/ov76_0223D338.h"
#include "overlay076/struct_ov76_0223DE00.h"
#include "savedata/save_table.h"

#include "bag.h"
#include "bg_window.h"
#include "field_task.h"
#include "field_transition.h"
#include "g3d_pipeline.h"
#include "game_options.h"
#include "game_overlay.h"
#include "gx_layers.h"
#include "heap.h"
#include "mail.h"
#include "narc.h"
#include "overlay_manager.h"
#include "palette.h"
#include "party.h"
#include "pokemon.h"
#include "pokemon_anim.h"
#include "pokemon_sprite.h"
#include "save_player.h"
#include "savedata.h"
#include "screen_fade.h"
#include "sound.h"
#include "system.h"
#include "touch_pad.h"
#include "touch_screen_actions.h"
#include "tv_episode_segment.h"
#include "unk_0202C9F4.h"
#include "vram_transfer.h"

#include "constdata/const_020F64C0.h"

FS_EXTERN_OVERLAY(overlay11);
FS_EXTERN_OVERLAY(battle_anim);
FS_EXTERN_OVERLAY(ball_capsule);

static int BallCapsuleApp_Init(ApplicationManager *appMan, int *param1);
static int BallCapsuleApp_Main(ApplicationManager *appMan, int *param1);
static int BallCapsuleApp_Exit(ApplicationManager *appMan, int *param1);

const ApplicationManagerTemplate gBallCapsuleSystemAppTemplate = {
    BallCapsuleApp_Init,
    BallCapsuleApp_Main,
    BallCapsuleApp_Exit,
    FS_OVERLAY_ID(ball_capsule),
};

static int BallCapsuleApp_Init(ApplicationManager *appMan, int *param1)
{
    BallCapsuleSystem *ballCapsuleSys;
    BallCapsuleAppData *appData;

    Heap_Create(HEAP_ID_APPLICATION, HEAP_ID_BALL_CAPSULE_SYSTEM, 0x80000);
    ov76_0223EB20(HEAP_ID_BALL_CAPSULE_SYSTEM);
    BallCapsuleSystem_SetDefaultBlend();

    ballCapsuleSys = ApplicationManager_NewData(appMan, sizeof(BallCapsuleSystem), HEAP_ID_BALL_CAPSULE_SYSTEM);
    memset(ballCapsuleSys, 0, sizeof(BallCapsuleSystem));

    ballCapsuleSys->ballCapsuleEditor.pipelineBuffers = ov76_0223BE6C();
    appData = ApplicationManager_Args(appMan);
    ballCapsuleSys->appData = appData;
    ballCapsuleSys->narc = NARC_ctor(NARC_INDEX_POKETOOL__POKE_EDIT__PL_POKE_DATA, HEAP_ID_BALL_CAPSULE_SYSTEM);
    ballCapsuleSys->mon = Pokemon_New(HEAP_ID_BALL_CAPSULE_SYSTEM);
    ballCapsuleSys->ballCapsuleEditor.unk_00 = 0xFF;
    ballCapsuleSys->sealCasePages.currentPage = 0;

    int extraPage = 0;
    int sealCount = (SealCase_CountUniqueSeals(ballCapsuleSys->appData->sealCase));

    if (sealCount % SEALS_PER_PAGE) {
        extraPage = 1;
    }

    ballCapsuleSys->sealCasePages.totalPages = (SealCase_CountUniqueSeals(ballCapsuleSys->appData->sealCase) / SEALS_PER_PAGE) + extraPage;

    if (ballCapsuleSys->sealCasePages.totalPages > (SEAL_ID_MAX / SEALS_PER_PAGE)) {
        ballCapsuleSys->sealCasePages.totalPages = (SEAL_ID_MAX / SEALS_PER_PAGE);
    }

    ballCapsuleSys->selectedCapsules[0] = BallCapsuleAppData_GetSelectedCapsuleIndex(ballCapsuleSys->appData);
    ballCapsuleSys->selectedCapsules[1] = BallCapsuleAppData_GetSelectedCapsuleIndex(ballCapsuleSys->appData);

    {
        int i;
        int capsuleId;
        BallCapsule *capsule;

        ballCapsuleSys->sealCounts = SealCase_GetSealsObtained(ballCapsuleSys->appData->sealCase);

        for (i = 0; i < TOTAL_CAPSULES; i++) {
            capsule = SealCase_GetCapsuleById(ballCapsuleSys->appData->sealCase, i);
            ballCapsuleSys->capsuleData[i].partyIndex = BALL_CAPSULE_INVALID_PARTY_INDEX;
            ballCapsuleSys->capsuleData[i].ballCapsule = capsule;
        }

        for (i = 0; i < MAX_PARTY_SIZE; i++) {
            if (ballCapsuleSys->appData->mons[i] == NULL) {
                continue;
            }

            capsuleId = Pokemon_GetValue(ballCapsuleSys->appData->mons[i], MON_DATA_BALL_CAPSULE_ID, 0);

            if (capsuleId != 0) {
                ballCapsuleSys->capsuleData[capsuleId - 1].partyIndex = i;
            }
        }
    }

    ballCapsuleSys->ballCapsuleEditor.bgConfig = BgConfig_New(HEAP_ID_BALL_CAPSULE_SYSTEM);
    VramTransfer_New(64, HEAP_ID_BALL_CAPSULE_SYSTEM);
    ballCapsuleSys->ballCapsuleEditor.paletteData = PaletteData_New(HEAP_ID_BALL_CAPSULE_SYSTEM);
    PaletteData_SetAutoTransparent(ballCapsuleSys->ballCapsuleEditor.paletteData, 1);
    PaletteData_AllocBuffer(ballCapsuleSys->ballCapsuleEditor.paletteData, 0, 0x200, HEAP_ID_BALL_CAPSULE_SYSTEM);
    PaletteData_AllocBuffer(ballCapsuleSys->ballCapsuleEditor.paletteData, 1, 0x200, HEAP_ID_BALL_CAPSULE_SYSTEM);
    PaletteData_AllocBuffer(ballCapsuleSys->ballCapsuleEditor.paletteData, 2, 0x200, HEAP_ID_BALL_CAPSULE_SYSTEM);
    PaletteData_AllocBuffer(ballCapsuleSys->ballCapsuleEditor.paletteData, 3, 0x200, HEAP_ID_BALL_CAPSULE_SYSTEM);

    ov76_0223EB64(ballCapsuleSys->ballCapsuleEditor.bgConfig);
    ov76_0223BF10();

    ballCapsuleSys->ballCapsuleEditor.monSpriteMan = PokemonSpriteManager_New(HEAP_ID_BALL_CAPSULE_SYSTEM);
    ballCapsuleSys->ballCapsuleEditor.animManager = PokemonAnimManager_New(HEAP_ID_BALL_CAPSULE_SYSTEM, 1, FALSE);

    int v7 = Options_Frame(ballCapsuleSys->appData->options);
    ov76_0223C8EC(ballCapsuleSys->ballCapsuleEditor.bgConfig, ballCapsuleSys->ballCapsuleEditor.paletteData, v7);
    ov76_0223C974(ballCapsuleSys->ballCapsuleEditor.bgConfig, ballCapsuleSys->ballCapsuleEditor.paletteData, v7);

    ov76_0223C398(&ballCapsuleSys->ballCapsuleEditor);

    {
        u32 v8;

        EnableTouchPad();
        v8 = InitializeTouchPad(4);

        if (v8 != 1) {
            (void)0;
        }
    }

    ov76_0223DCC0(ballCapsuleSys);
    SetVBlankCallback(ov76_0223ECB0, ballCapsuleSys);
    ov76_0223B8A8(ballCapsuleSys);
    Sound_SetSceneAndPlayBGM(SOUND_SCENE_SUB_59, SEQ_NONE, 0);

    return 1;
}

static int BallCapsuleApp_Main(ApplicationManager *appMan, int *param1)
{
    BallCapsuleSystem *ballCapsuleSys = ApplicationManager_Data(appMan);

    switch (*param1) {
    case 0:
        if (IsScreenFadeDone() == TRUE) {
            *param1 = 1;
        }
        break;
    case 1: {
        BOOL v1 = ov76_0223D550(ballCapsuleSys);

        if (v1 == 0) {
            *param1 = 2;
            break;
        }

        PokemonSpriteManager_DrawSprites(ballCapsuleSys->ballCapsuleEditor.monSpriteMan);
        ov76_0223BF50();
    } break;
    case 2:
        if (IsScreenFadeDone() == TRUE) {
            return 1;
        }
        break;
    }

    return 0;
}

static int BallCapsuleApp_Exit(ApplicationManager *appMan, int *param1)
{
    BallCapsuleSystem *ballCapsuleSys = ApplicationManager_Data(appMan);

    GXLayers_EngineAToggleLayers(GX_PLANEMASK_BG0, 0);
    GXLayers_EngineAToggleLayers(GX_PLANEMASK_BG1, 0);
    GXLayers_EngineAToggleLayers(GX_PLANEMASK_BG2, 0);
    GXLayers_EngineAToggleLayers(GX_PLANEMASK_BG3, 0);
    GXLayers_EngineBToggleLayers(GX_PLANEMASK_BG0, 0);
    GXLayers_EngineBToggleLayers(GX_PLANEMASK_BG1, 0);
    GXLayers_EngineBToggleLayers(GX_PLANEMASK_BG2, 0);
    GXLayers_EngineBToggleLayers(GX_PLANEMASK_BG3, 0);
    Bg_FreeTilemapBuffer(ballCapsuleSys->ballCapsuleEditor.bgConfig, 1);
    Bg_FreeTilemapBuffer(ballCapsuleSys->ballCapsuleEditor.bgConfig, 2);
    Bg_FreeTilemapBuffer(ballCapsuleSys->ballCapsuleEditor.bgConfig, 3);
    Bg_FreeTilemapBuffer(ballCapsuleSys->ballCapsuleEditor.bgConfig, 4);
    Bg_FreeTilemapBuffer(ballCapsuleSys->ballCapsuleEditor.bgConfig, 5);
    Bg_FreeTilemapBuffer(ballCapsuleSys->ballCapsuleEditor.bgConfig, 6);
    Bg_FreeTilemapBuffer(ballCapsuleSys->ballCapsuleEditor.bgConfig, 7);
    Heap_Free(ballCapsuleSys->ballCapsuleEditor.bgConfig);
    PaletteData_FreeBuffer(ballCapsuleSys->ballCapsuleEditor.paletteData, 0);
    PaletteData_FreeBuffer(ballCapsuleSys->ballCapsuleEditor.paletteData, 1);
    PaletteData_FreeBuffer(ballCapsuleSys->ballCapsuleEditor.paletteData, 2);
    PaletteData_FreeBuffer(ballCapsuleSys->ballCapsuleEditor.paletteData, 3);
    PaletteData_Free(ballCapsuleSys->ballCapsuleEditor.paletteData);
    BallCapsuleAppData_SetSelectedCapsuleIndex(ballCapsuleSys->appData, ballCapsuleSys->selectedCapsules[0]);
    Heap_Free(ballCapsuleSys->mon);
    BallCapsuleSystem_RemoveTappedSeal(ballCapsuleSys);
    TouchScreenActions_Free(ballCapsuleSys->ballCapsuleEditor.touchScreenActions);
    PokemonSpriteManager_Free(ballCapsuleSys->ballCapsuleEditor.monSpriteMan);
    PokemonAnimManager_Free(ballCapsuleSys->ballCapsuleEditor.animManager);
    ov76_0223B8C4(ballCapsuleSys);
    ov76_0223C424(&ballCapsuleSys->ballCapsuleEditor);
    VramTransfer_Free();
    G3DPipelineBuffers_Free(ballCapsuleSys->ballCapsuleEditor.pipelineBuffers);
    ov76_0223EB54(HEAP_ID_BALL_CAPSULE_SYSTEM);
    NARC_dtor(ballCapsuleSys->narc);
    ApplicationManager_FreeData(appMan);

    {
        u32 v1;

        v1 = DisableTouchPad();

        if (v1 != 1) {
            (void)0;
        }
    }

    Heap_Destroy(HEAP_ID_BALL_CAPSULE_SYSTEM);
    Overlay_UnloadByID(FS_OVERLAY_ID(overlay11));
    Overlay_UnloadByID(FS_OVERLAY_ID(battle_anim));

    return 1;
}

Pokemon *BallCapsuleAppData_GetPokemon(BallCapsuleAppData *appData, int partyIndex)
{
    if (appData->partyCount < partyIndex) {
        GF_ASSERT(0);
        partyIndex = 0;
    }

    return appData->mons[partyIndex];
}

u8 BallCapsuleAppData_GetSelectedCapsuleIndex(BallCapsuleAppData *appData)
{
    return appData->selectedCapsule;
}

void BallCapsuleAppData_SetSelectedCapsuleIndex(BallCapsuleAppData *appData, u8 index)
{
    appData->selectedCapsule = index;
}

u8 BallCapsuleAppData_GetState(BallCapsuleAppData *appData)
{
    return appData->state;
}

void BallCapsuleAppData_SetState(BallCapsuleAppData *appData, u8 param1)
{
    appData->state = param1;
}

typedef struct {
    void *unused_00;
    void *unused_04;

    BallCapsuleAppData *appData;
    PartyMenu *partyMenu;
    SaveData *saveData;
    int state;
} BallCapsuleEditorTask;

enum BallCapsuleEditorState {
    BALL_CAPSULE_EDITOR_INIT = 0,
    BALL_CAPSULE_EDITOR_START,
    BALL_CAPSULE_EDITOR_2,
    BALL_CAPSULE_EDITOR_PARTY,
    BALL_CAPSULE_EDITOR_SAVE_TO_MON,
    BALL_CAPSULE_EDITOR_EXIT,
    BALL_CAPSULE_EDITOR_SHUTDOWN,
};

static BOOL sub_02097F38(FieldTask *fieldTask)
{
    BallCapsuleEditorTask *editorTask = FieldTask_GetEnv(fieldTask);
    BallCapsuleAppData *appData = editorTask->appData;
    FieldSystem *fieldSystem = FieldTask_GetFieldSystem(fieldTask);

    switch (editorTask->state) {
    case BALL_CAPSULE_EDITOR_INIT:

        FieldTransition_FinishMap(fieldTask);
        appData->sealCase = SaveData_GetSealCase(editorTask->saveData);
        BallCapsuleAppData_SetSelectedCapsuleIndex(appData, 0);

        appData->party = SaveData_GetParty(editorTask->saveData);
        int partySize = Party_GetCurrentCount(appData->party);
        appData->partyCount = partySize;

        int i;
        for (i = 0; i < partySize; i++) {
            appData->mons[i] = Party_GetPokemonBySlotIndex(appData->party, i);
        }

        for (; i < MAX_PARTY_SIZE; i++) {
            appData->mons[i] = NULL;
        }
        editorTask->state = BALL_CAPSULE_EDITOR_START;
        break;

    case BALL_CAPSULE_EDITOR_START:
        FieldTask_RunApplication(fieldTask, &gBallCapsuleSystemAppTemplate, appData);
        editorTask->state = BALL_CAPSULE_EDITOR_2;
        break;

    case BALL_CAPSULE_EDITOR_2: {
        u8 v5 = BallCapsuleAppData_GetState(editorTask->appData);

        switch (v5) {
        default:
            GF_ASSERT(0);
        case 0:
            editorTask->state = BALL_CAPSULE_EDITOR_EXIT;
            break;

        case 1:
            editorTask->state = BALL_CAPSULE_EDITOR_PARTY;
            break;
        }
    } break;

    case BALL_CAPSULE_EDITOR_PARTY: {
        PartyMenu *partyMenu = editorTask->partyMenu;

        partyMenu->party = appData->party;
        partyMenu->bag = SaveData_GetBag(editorTask->saveData);
        partyMenu->mailbox = SaveData_GetMailbox(editorTask->saveData);
        partyMenu->selectedMonSlot = 0;
        partyMenu->type = PARTY_MENU_TYPE_BASIC;
        partyMenu->mode = PARTY_MENU_MODE_BALL_SEAL;
        partyMenu->options = appData->options;

        FieldTask_RunApplication(fieldTask, &gPokemonPartyAppTemplate, partyMenu);
        editorTask->state = BALL_CAPSULE_EDITOR_SAVE_TO_MON;
    } break;

    case BALL_CAPSULE_EDITOR_SAVE_TO_MON: {
        PartyMenu *partyMenu = editorTask->partyMenu;
        Pokemon *mon;
        BallCapsule *ballCapsule;
        BallSeal *ballSeal;
        TVBroadcast *broadcast;
        int sealID;
        int selectedIndex;

        selectedIndex = BallCapsuleAppData_GetSelectedCapsuleIndex(editorTask->appData) + 1;

        if (partyMenu->selectedMonSlot != 7) {
            mon = BallCapsuleAppData_GetPokemon(editorTask->appData, partyMenu->selectedMonSlot);

            Pokemon_SetValue(mon, MON_DATA_BALL_CAPSULE_ID, (u8 *)&selectedIndex);
            Pokemon_SetValue(mon, MON_DATA_BALL_CAPSULE, SealCase_GetCapsuleById(appData->sealCase, selectedIndex - 1));

            ballCapsule = SealCase_GetCapsuleById(appData->sealCase, selectedIndex - 1);
            ballSeal = BallCapsule_GetBallSeals(ballCapsule, 0);
            sealID = BallSeal_GetSealType(ballSeal);
            sealID = SealData_GetNameID(sealID);
            broadcast = SaveData_GetTVBroadcast(fieldSystem->saveData);

            FieldSystem_SaveTVEpisodeSegment_SealClubShow(broadcast, mon, sealID);
        }
    }
        editorTask->state = BALL_CAPSULE_EDITOR_START;
        break;

    case BALL_CAPSULE_EDITOR_EXIT:
        FieldTransition_StartMap(fieldTask);
        editorTask->state = BALL_CAPSULE_EDITOR_SHUTDOWN;
        break;

    case BALL_CAPSULE_EDITOR_SHUTDOWN:
        Heap_Free(editorTask->partyMenu);
        Heap_Free(editorTask->appData);
        Heap_Free(editorTask);
        return TRUE;
    }

    return FALSE;
}

void BeginCapsuleEditorTask(FieldTask *fieldTask, SaveData *saveData)
{
    BallCapsuleEditorTask *editorTask = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(BallCapsuleEditorTask));
    memset(editorTask, 0, sizeof(BallCapsuleEditorTask));

    editorTask->saveData = saveData;

    editorTask->appData = Heap_Alloc(HEAP_ID_FIELD2, sizeof(BallCapsuleAppData));
    memset(editorTask->appData, 0, sizeof(BallCapsuleAppData));
    editorTask->appData->options = SaveData_GetOptions(saveData);
    editorTask->appData->saveData = saveData;

    editorTask->partyMenu = Heap_Alloc(HEAP_ID_FIELD2, sizeof(PartyMenu));
    memset(editorTask->partyMenu, 0, sizeof(PartyMenu));

    FieldTask_InitCall(fieldTask, sub_02097F38, editorTask);
}

typedef struct {
    u16 charID;
    u8 nameID;
    u8 unused_03;
    u8 unk_04;
    u8 isChar; // A-Z, '?', '!' seals
    u16 price;
    u8 descriptionID;
} SealData;

static const SealData sealData[SEAL_ID_MAX] = {
    { 0xB8, 0x0, 0x125, 0x25, 0x0, 0x3E7, 0x0 },
    { 0xB9, 0x1, 0x125, 0x25, 0x0, 0x32, 0x0 },
    { 0xBA, 0x2, 0x125, 0x26, 0x0, 0x32, 0x1 },
    { 0xBB, 0x3, 0x125, 0x27, 0x0, 0x32, 0x2 },
    { 0xBC, 0x4, 0x125, 0x28, 0x0, 0x32, 0x3 },
    { 0xBD, 0x5, 0x125, 0x29, 0x0, 0x64, 0x4 },
    { 0xBE, 0x6, 0x125, 0x2A, 0x0, 0x64, 0x5 },
    { 0xC0, 0x7, 0x125, 0x2B, 0x0, 0x32, 0x6 },
    { 0xBF, 0x8, 0x125, 0x2C, 0x0, 0x32, 0x7 },
    { 0xC2, 0x9, 0x125, 0x2D, 0x0, 0x32, 0x8 },
    { 0xC1, 0xA, 0x125, 0x2E, 0x0, 0x32, 0x9 },
    { 0xC4, 0xB, 0x125, 0x2F, 0x0, 0x64, 0xA },
    { 0xC3, 0xC, 0x125, 0x30, 0x0, 0x64, 0xB },
    { 0xC5, 0xD, 0x125, 0x31, 0x0, 0x64, 0xC },
    { 0xC6, 0xE, 0x125, 0x32, 0x0, 0x64, 0xD },
    { 0xC7, 0xF, 0x125, 0x33, 0x0, 0x64, 0xE },
    { 0xC8, 0x4D, 0x125, 0x34, 0x0, 0x64, 0xF },
    { 0xC9, 0x10, 0x125, 0x39, 0x0, 0x64, 0x10 },
    { 0xCA, 0x11, 0x125, 0x3A, 0x0, 0x64, 0x11 },
    { 0xCB, 0x12, 0x125, 0x3B, 0x0, 0x64, 0x12 },
    { 0xCC, 0x13, 0x125, 0x3C, 0x0, 0x64, 0x13 },
    { 0xCD, 0x14, 0x125, 0x35, 0x0, 0x64, 0x14 },
    { 0xCE, 0x15, 0x125, 0x36, 0x0, 0x64, 0x15 },
    { 0xCF, 0x16, 0x125, 0x37, 0x0, 0x64, 0x16 },
    { 0xD0, 0x17, 0x125, 0x38, 0x0, 0x64, 0x17 },
    { 0xD1, 0x18, 0x125, 0x3D, 0x0, 0x32, 0x18 },
    { 0xD2, 0x19, 0x125, 0x3E, 0x0, 0x32, 0x19 },
    { 0xD3, 0x1A, 0x125, 0x3F, 0x0, 0x32, 0x1A },
    { 0xD4, 0x1B, 0x125, 0x40, 0x0, 0x32, 0x1B },
    { 0xD6, 0x1C, 0x125, 0x41, 0x0, 0x32, 0x1C },
    { 0xD5, 0x1D, 0x125, 0x42, 0x0, 0x32, 0x1D },
    { 0xD8, 0x1E, 0x125, 0x43, 0x0, 0x32, 0x1E },
    { 0xD7, 0x1F, 0x125, 0x44, 0x0, 0x32, 0x1F },
    { 0xD9, 0x20, 0x125, 0x45, 0x0, 0x32, 0x20 },
    { 0xDA, 0x21, 0x125, 0x46, 0x0, 0x32, 0x21 },
    { 0xDB, 0x22, 0x125, 0x47, 0x0, 0x64, 0x22 },
    { 0xDC, 0x23, 0x125, 0x48, 0x0, 0x64, 0x23 },
    { 0xDD, 0x24, 0x125, 0x49, 0x0, 0x32, 0x24 },
    { 0xDE, 0x25, 0x125, 0x4A, 0x0, 0x32, 0x25 },
    { 0xDF, 0x26, 0x125, 0x4B, 0x0, 0x32, 0x26 },
    { 0xE0, 0x27, 0x125, 0x4C, 0x0, 0x32, 0x27 },
    { 0xE1, 0x28, 0x125, 0x4D, 0x0, 0x32, 0x28 },
    { 0xE2, 0x29, 0x125, 0x4E, 0x0, 0x32, 0x29 },
    { 0xE3, 0x2A, 0x125, 0x4F, 0x0, 0x32, 0x2A },
    { 0xE4, 0x2B, 0x125, 0x50, 0x0, 0x32, 0x2B },
    { 0xE5, 0x2C, 0x125, 0x51, 0x0, 0x32, 0x2C },
    { 0xE6, 0x2D, 0x125, 0x52, 0x0, 0x32, 0x2D },
    { 0xE7, 0x2E, 0x125, 0x53, 0x0, 0x32, 0x2E },
    { 0xE8, 0x2F, 0x125, 0x54, 0x0, 0x32, 0x2F },
    { 0xE9, 0x30, 0x125, 0x55, 0x0, 0x32, 0x30 },
    { 0xEA, 0x31, 0x125, 0x56, 0x1, 0x0, 0x0 },
    { 0xEB, 0x32, 0x125, 0x57, 0x1, 0x0, 0x0 },
    { 0xEC, 0x33, 0x125, 0x58, 0x1, 0x0, 0x0 },
    { 0xED, 0x34, 0x125, 0x59, 0x1, 0x0, 0x0 },
    { 0xEE, 0x35, 0x125, 0x5A, 0x1, 0x0, 0x0 },
    { 0xEF, 0x36, 0x125, 0x5B, 0x1, 0x0, 0x0 },
    { 0xF0, 0x37, 0x125, 0x5C, 0x1, 0x0, 0x0 },
    { 0xF1, 0x38, 0x125, 0x5D, 0x1, 0x0, 0x0 },
    { 0xF2, 0x39, 0x125, 0x5E, 0x1, 0x0, 0x0 },
    { 0xF3, 0x3A, 0x125, 0x5F, 0x1, 0x0, 0x0 },
    { 0xF4, 0x3B, 0x125, 0x60, 0x1, 0x0, 0x0 },
    { 0xF5, 0x3C, 0x125, 0x61, 0x1, 0x0, 0x0 },
    { 0xF6, 0x3D, 0x125, 0x62, 0x1, 0x0, 0x0 },
    { 0xF7, 0x3E, 0x125, 0x63, 0x1, 0x0, 0x0 },
    { 0xF8, 0x3F, 0x125, 0x64, 0x1, 0x0, 0x0 },
    { 0xF9, 0x40, 0x125, 0x65, 0x1, 0x0, 0x0 },
    { 0xFA, 0x41, 0x125, 0x66, 0x1, 0x0, 0x0 },
    { 0xFB, 0x42, 0x125, 0x67, 0x1, 0x0, 0x0 },
    { 0xFC, 0x43, 0x125, 0x68, 0x1, 0x0, 0x0 },
    { 0xFD, 0x44, 0x125, 0x69, 0x1, 0x0, 0x0 },
    { 0xFE, 0x45, 0x125, 0x6A, 0x1, 0x0, 0x0 },
    { 0xFF, 0x46, 0x125, 0x6B, 0x1, 0x0, 0x0 },
    { 0x100, 0x47, 0x125, 0x6C, 0x1, 0x0, 0x0 },
    { 0x101, 0x48, 0x125, 0x6D, 0x1, 0x0, 0x0 },
    { 0x102, 0x49, 0x125, 0x6E, 0x1, 0x0, 0x0 },
    { 0x103, 0x4A, 0x125, 0x6F, 0x1, 0x0, 0x0 },
    { 0x104, 0x4B, 0x125, 0x70, 0x1, 0x0, 0x0 },
    { 0x105, 0x4C, 0x125, 0x71, 0x1, 0x0, 0x0 },
    { 0x106, 0x4E, 0x125, 0x72, 0x0, 0x0, 0x31 },
    { 0x107, 0x4F, 0x125, 0x73, 0x0, 0x0, 0x32 },
    { 0x108, 0x50, 0x125, 0x74, 0x0, 0x0, 0x33 }
};

int SealData_GetCharID(u8 sealID)
{
    GF_ASSERT(sealID < (sizeof(sealData)));
    return sealData[sealID].charID;
}

int SealData_GetNameID(u8 sealID)
{
    GF_ASSERT(sealID < (sizeof(sealData)));
    return sealData[sealID].nameID;
}

int sub_02098188(u8 sealID)
{
    GF_ASSERT(sealID < (sizeof(sealData)));
    return sealData[sealID].unk_04;
}

// Is a character seal (A-Z, '?', or '!')
int SealData_IsCharSeal(u8 sealID)
{
    GF_ASSERT(sealID < (sizeof(sealData)));
    return sealData[sealID].isChar;
}

int SealData_GetPrice(u8 sealID)
{
    GF_ASSERT(sealID < (sizeof(sealData)));
    return sealData[sealID].price;
}

int SealData_GetDescriptionMessageID(u8 sealID)
{
    GF_ASSERT(sealID < (sizeof(sealData)));
    return sealData[sealID].descriptionID;
}
