#ifndef POKEPLATINUM_UNK_02013B10_H
#define POKEPLATINUM_UNK_02013B10_H

#include "struct_decls/struct_02013B10_decl.h"
#include "struct_decls/struct_02013BE0_decl.h"
#include "constants/heap.h"

#define BUFFER_MANAGER_MODE_SINGLE 0
#define BUFFER_MANAGER_MODE_DOUBLE 1

#define SCREEN_SCROLL_MANAGER_BUFFER_SIZE 192
#define SCREEN_SCROLL_MANAGER_SCALING_FACTOR 100
#define SCREEN_SCROLL_MANAGER_MAX_SCROLL (SCREEN_SCROLL_MANAGER_BUFFER_SIZE * SCREEN_SCROLL_MANAGER_SCALING_FACTOR)

enum BufferManagerDMAType {
    BUFFER_MANAGER_DMA_TYPE_16BIT = 0,
    BUFFER_MANAGER_DMA_TYPE_32BIT = 1
};

BufferManager *BufferManager_New(enum HeapId heapID, void *buffer1, void *buffer2);
void BufferManager_Delete(BufferManager *bufferManager);
void *BufferManager_GetWriteBuffer(const BufferManager *bufferManager);
const void *BufferManager_GetReadBuffer(const BufferManager *bufferManager);
void BufferManager_SetMode(BufferManager *bufferManager, u8 mode);
void BufferManager_SwapBuffers(BufferManager *bufferManager);
void BufferManager_StopDMA(void);
void BufferManager_StartDMA(const void *src, void *dst, int size, enum BufferManagerDMAType dmaType);
ScreenScrollManager *ScreenScrollManager_New(enum HeapId heapID);
void ScreenScrollManager_ScrollX(ScreenScrollManager *screenScrollMgr, u8 start, u8 end, u16 angleIncrement, fx32 amplitude, s16 speed, u32 bg, u32 initValue, u32 priority);
void ScreenScrollManager_ScrollY(ScreenScrollManager *screenScrollMgr, u8 start, u8 end, u16 angleIncrement, fx32 amplitude, s16 speed, u32 bg, u32 initValue, u32 priority);
void ScreenScrollManager_Stop(ScreenScrollManager *screenScrollMgr);
void ScreenScrollManager_Delete(ScreenScrollManager *screenScrollMgr);
void *ScreenScrollManager_GetWriteBuffer(ScreenScrollManager *screenScrollMgr);
void ScreenScrollManager_SwapBuffers(ScreenScrollManager *screenScrollMgr);
void ScreenScrollManager_RestartDMA(ScreenScrollManager *screenScrollMgr);
void ScreenScrollManager_CreateSineTable(s16 *dest, u32 entryCount, u16 angleIncrement, fx32 amplitude);

#endif // POKEPLATINUM_UNK_02013B10_H
