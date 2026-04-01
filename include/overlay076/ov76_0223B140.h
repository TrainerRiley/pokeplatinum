#ifndef POKEPLATINUM_OV76_0223B140_H
#define POKEPLATINUM_OV76_0223B140_H

#include "struct_defs/seal_case.h"

#include "overlay076/struct_ov76_0223DE00.h"

#include "sprite_system.h"
#include "touch_screen.h"

void UpdatePlacedSealPos(TouchScreenRect *rect, ManagedSprite *param1, BOOL param2);
void BallCapsuleSystem_UpdatePlacedSealSpriteOffsets(BallCapsuleSystem *ballCapsuleSys);
void BallCapsuleSystem_LoadSelectedCapsulePlacedSeals(BallCapsuleSystem *ballCapsuleSys);
int BallCapsuleSystem_AddPlacedSeal(BallCapsuleSystem *ballCapsuleSys, u8 param1);
BOOL BallCapsuleSystem_AnyPlacedSealTapped(BallCapsuleSystem *ballCapsuleSys);
void BallCapsuleSystem_IncreasePlacedSealPriority(BallCapsuleSystem *ballCapsuleSys, u8 param1);
void BallCapsuleSystem_LoadPlacedSealSpriteData(BallCapsuleSystem *ballCapsuleSys, u8 param1, u8 param2);
void BallCapsuleSystem_SortPlacedSeals(BallCapsuleSystem *ballCapsuleSys);
BOOL BallCapsuleSystem_CreatePlacedSealSprite(BallCapsuleSystem *ballCapsuleSys, u8 param1);
BOOL BallCapsuleSystem_HandlePlacedSealTouchEvent(BallCapsuleSystem *ballCapsuleSys, enum TouchScreenButtonState state, u8 param2);
void BallCapsuleSystem_RemoveTappedSeal(BallCapsuleSystem *ballCapsuleSys);
void BallCapsuleSystem_SetDrawFlagOnTappedSeal(BallCapsuleSystem *ballCapsuleSys, int param1);
BOOL BallCapsuleSystem_IsPlacedSealOnBall(BallCapsuleSystem *ballCapsuleSys, int param1);
void BallCapsuleSystem_RemovePlacedSeal(BallCapsuleSystem *ballCapsuleSys, int param1);
void BallCapsuleSystem_SetPlacedSealOamMode(BallCapsuleSystem *ballCapsuleSys, int param1);
BOOL BallCapsuleSystem_HasCurrentCapsuleBeenEdited(BallCapsuleSystem *ballCapsuleSys);
void BallCapsuleSystem_SetSealPosToSpritePos(BallCapsuleSystem *ballCapsuleSys, int param1);
void BallCapsuleSystem_SaveSelectedCapsuleToSealCase(BallCapsuleSystem *ballCapsuleSys);
void BallCapsuleSystem_SetPlacedSealsOnBallCapsule(BallCapsule *ballCapsule, BallCapsuleSystem *ballCapsuleSys);

#endif // POKEPLATINUM_OV76_0223B140_H
