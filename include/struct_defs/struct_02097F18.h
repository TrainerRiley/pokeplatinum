#ifndef POKEPLATINUM_STRUCT_02097F18_H
#define POKEPLATINUM_STRUCT_02097F18_H

#include "struct_defs/seal_case.h"

#include "game_options.h"
#include "party.h"
#include "pokemon.h"
#include "savedata.h"

enum BallCapsuleAppDataState {
    BALL_CAPSULE_APP_DATA_EXIT = 0,
    BALL_CAPSULE_APP_DATA_LIST,
};

typedef struct {
    int partyCount;
    Pokemon *mons[6];
    Party *party;
    SealCase *sealCase;
    Options *options;
    SaveData *saveData;
    u8 selectedCapsule;
    u8 state;
    u8 padding[2];
} BallCapsuleAppData;

#endif // POKEPLATINUM_STRUCT_02097F18_H
