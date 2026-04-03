#ifndef POKEPLATINUM_OV76_0223B140_H
#define POKEPLATINUM_OV76_0223B140_H

#include "struct_defs/seal_case.h"

#include "overlay076/struct_ov76_0223DE00.h"

#include "sprite_system.h"
#include "touch_screen.h"

void UpdatePlacedSealPos(TouchScreenRect *rect, ManagedSprite *sprite, BOOL movingSeal);
void BallCapsuleSystem_UpdatePlacedSealSpriteOffsets(BallCapsuleSystem *ballCapsuleSys);
void BallCapsuleSystem_LoadSelectedCapsulePlacedSeals(BallCapsuleSystem *ballCapsuleSys);
int BallCapsuleSystem_AddPlacedSeal(BallCapsuleSystem *ballCapsuleSys, u8 sealPageIndex);
BOOL BallCapsuleSystem_AnyPlacedSealTapped(BallCapsuleSystem *ballCapsuleSys);
void BallCapsuleSystem_IncreasePlacedSealPriority(BallCapsuleSystem *ballCapsuleSys, u8 placedSealIndex);
void BallCapsuleSystem_LoadPlacedSealSpriteData(BallCapsuleSystem *ballCapsuleSys, u8 sealID, u8 placedSealIndex);
void BallCapsuleSystem_SortPlacedSeals(BallCapsuleSystem *ballCapsuleSys);
BOOL BallCapsuleSystem_CreatePlacedSealSprite(BallCapsuleSystem *ballCapsuleSys, u8 placedSealIndex);
BOOL BallCapsuleSystem_HandlePlacedSealTouchEvent(BallCapsuleSystem *ballCapsuleSys, enum TouchScreenButtonState touchScreenState, u8 placedSealIndex);
void BallCapsuleSystem_RemoveTappedSeal(BallCapsuleSystem *ballCapsuleSys);
void BallCapsuleSystem_SetDrawFlagOnTappedSeal(BallCapsuleSystem *ballCapsuleSys, int flag);
BOOL BallCapsuleSystem_IsPlacedSealOnBall(BallCapsuleSystem *ballCapsuleSys, int placedSealIndex);
void BallCapsuleSystem_RemovePlacedSeal(BallCapsuleSystem *ballCapsuleSys, int placedSealIndex);
void BallCapsuleSystem_SetPlacedSealOamMode(BallCapsuleSystem *ballCapsuleSys, int placedSealIndex);
BOOL BallCapsuleSystem_HasCurrentCapsuleBeenEdited(BallCapsuleSystem *ballCapsuleSys);
void BallCapsuleSystem_SetSealPosToSpritePos(BallCapsuleSystem *ballCapsuleSys, int placedSealIndex);
void BallCapsuleSystem_SaveSelectedCapsuleToSealCase(BallCapsuleSystem *ballCapsuleSys);
void BallCapsuleSystem_SetPlacedSealsOnBallCapsule(BallCapsule *ballCapsule, BallCapsuleSystem *ballCapsuleSys);

#endif // POKEPLATINUM_OV76_0223B140_H
