#ifndef POKEPLATINUM_UNK_02097B18_H
#define POKEPLATINUM_UNK_02097B18_H

#include "struct_defs/struct_02097F18.h"

#include "field_task.h"
#include "pokemon.h"
#include "savedata.h"

Pokemon *BallCapsuleAppData_GetPokemon(BallCapsuleAppData *appData, int param1);
u8 BallCapsuleAppData_GetSelectedCapsuleIndex(BallCapsuleAppData *appData);
void BallCapsuleAppData_SetSelectedCapsuleIndex(BallCapsuleAppData *appData, u8 param1);
u8 sub_02097F28(BallCapsuleAppData *appData);
void sub_02097F30(BallCapsuleAppData *appData, u8 param1);
void BeginCapsuleEditorTask(FieldTask *param0, SaveData *saveData);
int sub_02098140(u8 param0);
int SealData_GetNameID(u8 param0);
int sub_02098188(u8 param0);
int SealData_IsCharSeal(u8 param0);
int SealData_GetPrice(u8 param0);
int SealData_GetDescriptionMessageID(u8 param0);

#endif // POKEPLATINUM_UNK_02097B18_H
