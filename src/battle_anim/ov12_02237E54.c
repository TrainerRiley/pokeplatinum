#include "battle_anim/ov12_02237E54.h"

#include <nitro.h>
#include <string.h>

#include "constants/ball_capsule.h"

#include "struct_defs/seal_case.h"

#include "unk_0202C9F4.h"
#include "unk_02097B18.h"

int Distance(s16 x1, s16 y1, s16 x2, s16 y2)
{
    s16 xDist = (x1 - x2);
    s16 yDist = (y1 - y2) * -1;

    return FX_Sqrt(((yDist * yDist) + (xDist * xDist)) * FX32_ONE) / FX32_ONE;
}

int BallSeal_DistanceFromCapsuleCenter(BallSeal *seal)
{
    s16 x = BallSeal_GetX(seal);
    s16 y = BallSeal_GetY(seal);

    return Distance(x, y, BALL_CAPSULE_EDITOR_BALL_CENTER_X, BALL_CAPSULE_EDITOR_BALL_CENTER_Y);
}

int ov12_02237EA0(BallSeal *seal)
{
    const u8 v5[] = {
        0,
        8,
        14,
        20,
        20,
        16,
        16,
    };

    int sealID = BallSeal_GetSealType(seal);
    int isChar = SealData_IsCharSeal(sealID);
    int v1;

    if (isChar == 0) {
        int dist = BallSeal_DistanceFromCapsuleCenter(seal);
        int v4;
        if (dist >= (BALL_CAPSULE_EDITOR_BALL_RADIUS - 4)) {
            v4 = 3;
        } else {
            v4 = ((dist + 1) / 20);

            if (v4 >= (BALL_CAPSULE_EDITOR_BALL_RADIUS / 20)) {
                v4 = 3;
            }
        }

        v1 = v5[v4];
    } else {
        v1 = v5[1];
    }

    return v1;
}
