#ifndef CHANNEL_VCMV_WWW_H
#define CHANNEL_VCMV_WWW_H

#include <types.h>
#include <revolution/OS/OSTime.h>

BOOL GetCurrentCalendarTime(OSCalendarTime* time);

void wwwUnlinkedFunction(void);

struct WWWNandFile;
struct WWWLZState;

void wwwAddFonts(void);
s32 wwwCreateParentDirs(WWWNandFile* file);
s32 wwwWriteNandFile(WWWNandFile* file);
void wwwSaveSettings(void);
void wwwLoadSettings(void);
void wwwLZRead(u8* dst, WWWLZState* state, u32 n);
s32 wwwLoadLibrary(void);
s32 wwwLoadFonts(void);
s32 wwwInitialize(void);
void wwwShutdown(void);

#endif
