#ifndef POKEPLATINUM_UNK_02097B18_H
#define POKEPLATINUM_UNK_02097B18_H

#include "struct_defs/struct_02097F18.h"

#include "field_task.h"
#include "pokemon.h"
#include "savedata.h"

Pokemon *BallCapsuleAppData_GetPokemon(BallCapsuleAppData *appData, int partyIndex);
u8 BallCapsuleAppData_GetSelectedCapsuleIndex(BallCapsuleAppData *appData);
void BallCapsuleAppData_SetSelectedCapsuleIndex(BallCapsuleAppData *appData, u8 index);
u8 BallCapsuleAppData_GetState(BallCapsuleAppData *appData);
void BallCapsuleAppData_SetState(BallCapsuleAppData *appData, u8 param1);
void BeginCapsuleEditorTask(FieldTask *fieldTask, SaveData *saveData);
int SealData_GetCharID(u8 sealID);
int SealData_GetNameID(u8 sealID);
int sub_02098188(u8 param0);
int SealData_IsCharSeal(u8 sealID);
int SealData_GetPrice(u8 sealID);
int SealData_GetDescriptionMessageID(u8 sealID);

#endif // POKEPLATINUM_UNK_02097B18_H
