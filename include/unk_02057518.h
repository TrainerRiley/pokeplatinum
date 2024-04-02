#ifndef POKEPLATINUM_UNK_02057518_H
#define POKEPLATINUM_UNK_02057518_H

#include "field/field_system_decl.h"
#include "overlay023/struct_ov23_0224A294.h"

CommPlayerData * CommPlayerData_Get(void);
BOOL CommPlayerManager_Init(void * param0, FieldSystem * param1, BOOL param2);
void CommPlayerMan_Reset(void);
void CommPlayerMan_Restart(void);
void CommPlayerMan_Delete(BOOL param0);
void CommPlayerMan_Reinit(void);
void CommPlayerMan_Stop(void);
void CommPlayer_InitPersonal(void);
void CommPlayer_CopyPersonal(int param0);
void CommPlayer_SendXZPos(BOOL param0, int param1, int param2);
void CommPlayer_SendPos(BOOL param0);
void CommPlayer_SendPosServer(BOOL param0);
u32 CommPlayer_Size(void);
void CommPlayer_Destory(u8 param0, BOOL param1, BOOL param2);
BOOL sub_02057FAC(void);
void sub_02057FC4(BOOL param0);
void sub_02058018(int param0, int param1, void * param2, void * param3);
void sub_0205805C(FieldSystem * param0, BOOL param1);
BOOL sub_02058108(int param0, int param1);
void sub_02058488(int param0, int param1, void * param2, void * param3);
void sub_0205853C(int param0, int param1, void * param2, void * param3);
int sub_020585A4(void);
void sub_020585A8(int param0, int param1, void * param2, void * param3);
void sub_02058B0C(int param0, int param1, BOOL param2);
void sub_02058B7C(int param0);
void sub_02058B94(int param0);
void sub_02058BA8(int param0, int param1, BOOL param2);
void sub_02058BE8(int param0);
int sub_02058C3C(void);
BOOL sub_02058C40(void);
BOOL sub_02058C90(int param0);
int sub_02058CA0(int param0);
int sub_02058CF4(int param0);
int sub_02058D48(int param0);
int sub_02058D68(int param0);
int sub_02058D88(int param0);
int sub_02058DC0(int param0);
int sub_02058DF8(int param0);
int sub_02058E4C(int param0);
int sub_02058EA0(int param0);
int sub_02058EC0(int param0);
int sub_02058EE0(int param0);
int sub_02058F18(int param0);
int sub_02058F50(int param0);
int sub_02058F80(int param0);
void sub_02058FB0(int param0, int param1);
void sub_02058FE4(int param0, int param1);
int sub_0205900C(int param0, int param1);
void sub_02059058(int param0, BOOL param1);
BOOL sub_02059094(int param0);
BOOL sub_020590C4(void);
void sub_02059180(int param0, int param1, void * param2, void * param3);
void sub_02059240(void);
BOOL sub_0205928C(void);
int sub_02059328(int param0);
void sub_02059354(int param0, int param1);
void sub_02059378(int param0);
void sub_02059390(int param0, int param1);
void sub_020593B4(int param0);
int sub_020593CC(int param0);
int sub_020593E0(int param0);
void sub_020593F4(int param0);
void sub_02059424(int param0, int param1);
int sub_02059444(int param0);
void sub_02059464(int param0);
void sub_0205948C(int param0);
void sub_020594EC(void);
void sub_020594FC(void);
void sub_02059514(void);
void sub_02059524(void);
void sub_02059570(void);
void sub_020595A4(void);
void sub_02059624(void);
void sub_02059638(BOOL param0);

#endif // POKEPLATINUM_UNK_02057518_H
