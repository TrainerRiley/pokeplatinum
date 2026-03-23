#ifndef POKEPLATINUM_OV76_0223B140_H
#define POKEPLATINUM_OV76_0223B140_H

#include "struct_defs/seal_case.h"

#include "overlay076/struct_ov76_0223DE00.h"

#include "sprite_system.h"
#include "touch_screen.h"

void ov76_0223B184(TouchScreenRect *rect, ManagedSprite *param1, BOOL param2);
void ov76_0223B1E0(BallCapsuleSystem *ballCapsuleSys);
void ov76_0223B208(BallCapsuleSystem *ballCapsuleSys);
int BallCapsuleSystem_AddPlacedSeal(BallCapsuleSystem *ballCapsuleSys, u8 param1);
BOOL BallCapsuleSystem_AnyPlacedSealTapped(BallCapsuleSystem *ballCapsuleSys);
void BallCapsuleSystem_IncreasePlacedSealPriority(BallCapsuleSystem *ballCapsuleSys, u8 param1);
void ov76_0223B36C(BallCapsuleSystem *ballCapsuleSys, u8 param1, u8 param2);
void ov76_0223B400(BallCapsuleSystem *ballCapsuleSys);
BOOL ov76_0223B52C(BallCapsuleSystem *ballCapsuleSys, u8 param1);
BOOL ov76_0223B5C4(BallCapsuleSystem *ballCapsuleSys, enum TouchScreenButtonState state, u8 param2);
void BallCapsuleSystem_RemoveTappedSeal(BallCapsuleSystem *ballCapsuleSys);
void BallCapsuleSystem_SetDrawFlagOnTappedSeal(BallCapsuleSystem *ballCapsuleSys, int param1);
BOOL ov76_0223B6C4(BallCapsuleSystem *ballCapsuleSys, int param1);
void BallCapsuleSystem_RemovePlacedSeal(BallCapsuleSystem *ballCapsuleSys, int param1);
void ov76_0223B758(BallCapsuleSystem *ballCapsuleSys, int param1);
BOOL BallCapsuleSystem_HasCurrentCapsuleBeenEdited(BallCapsuleSystem *ballCapsuleSys);
void BallCapsuleSystem_SetSealPosToSpritePos(BallCapsuleSystem *ballCapsuleSys, int param1);
void BallCapsuleSystem_SaveSelectedCapsuleToSealCase(BallCapsuleSystem *ballCapsuleSys);
void BallCapsuleSystem_SetPlacedSealsOnBallCapsule(BallCapsule *ballCapsule, BallCapsuleSystem *ballCapsuleSys);

#endif // POKEPLATINUM_OV76_0223B140_H
