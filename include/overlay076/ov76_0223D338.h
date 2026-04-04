#ifndef POKEPLATINUM_OV76_0223D338_H
#define POKEPLATINUM_OV76_0223D338_H

#include "struct_defs/seal_case.h"

#include "overlay076/struct_ov76_0223DE00.h"

#include "bg_window.h"
#include "sprite_system.h"
#include "touch_screen.h"

void BallCapsuleSystem_SaveCapsuleToSelectedSlot(BallCapsuleSystem *ballCapsuleSys);
void BallCapsuleSystem_LoadSelectedCapsule(BallCapsuleSystem *ballCapsuleSys);
void BallCapsuleSystem_LoadSealCounts(BallCapsuleSystem *ballCapsuleSys);
void BallCapsuleSystem_SetSealCounts(BallCapsuleSystem *ballCapsuleSys);
void BallCapsuleSystem_SetDefaultBlend(void);
void BallCapsuleSystem_LoadFirstPokemon(BallCapsuleSystem *ballCapsuleSys);
int BallCapsule_ContainsSeals(BallCapsule *ballCapsule);
int BallCapsuleSystem_GetCapsuleStatus(BallCapsuleSystem *ballCapsuleSys, int param1);
BOOL ov76_0223D550(BallCapsuleSystem *ballCapsuleSys);
void BallCapsuleSystem_SetButtonSpriteState(ManagedSprite *param0, enum TouchScreenButtonState screenState);
void BallCapsuleSystem_EditorTouchScreenHandler(u32 param0, enum TouchScreenButtonState param1, void *param2);
BOOL ov76_0223DCB0(BallCapsuleSystem *ballCapsuleSys);
void BallCapsuleSystem_SetTouchScreenActive(BallCapsuleSystem *ballCapsuleSys, BOOL param1);
void ov76_0223DCC0(BallCapsuleSystem *ballCapsuleSys);
void BallCapsuleSystem_InitializeButtons(BallCapsuleSystem *ballCapsuleSys);
void BallCapsuleSystem_LoadPokemonSprite(BallCapsuleSystem *ballCapsuleSys);
void BallCapsuleSystem_SetPokemonHidden(BallCapsuleSystem *ballCapsuleSys, int param1);
void BallCapsuleSystem_DeletePokemonSprite(BallCapsuleSystem *ballCapsuleSys);
void BallCapsuleSystem_RemoveCapsuleFromPokemon(BallCapsuleSystem *ballCapsuleSys, int capsuleIndex);
void ov76_0223EB20(int param0);
void ov76_0223EB54(int param0);
void ov76_0223EB64(BgConfig *param0);
void ov76_0223ECB0(void *param0);

#endif // POKEPLATINUM_OV76_0223D338_H
