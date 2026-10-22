#include "Sound.h"
#include "System.h"
#include "nw4r/snd/snd_DvdSoundArchive.h"
#include "nw4r/snd/snd_FxReverbHi.h"
#include "nw4r/snd/snd_MemorySoundArchive.h"
#include "nw4r/snd/snd_SoundArchive.h"
#include "nw4r/snd/snd_SoundArchivePlayer.h"
#include "nw4r/snd/snd_SoundHandle.h"
#include "nw4r/snd/snd_SoundHeap.h"
#include "nw4r/snd/snd_SoundSystem.h"
#include "revolution/AI/ai.h"
#include "revolution/AX/AX.h"
#include "revolution/OS/OSError.h"
#include "revolution/SC/scapi.h"

typedef void *(*SoundAllocFunc)(u32);
typedef void (*SoundMemFunc)(void*);

void *DefaultSoundAlloc(u32 size);
void fn_80041884(void* data, void* heap, void *(*alloc_func)(u32), void (*mem_func)(void *));
void fn_800418A8(u32 arg1, void* data, u32 arg3, void* heap, u32 arg5, void* (*alloc_func)(u32), void (*mem_func)(void*));
void fn_80041C38();
void fn_80041F54();
void fn_800420E4(u8 sound_mode);

nw4r::snd::SoundHandle gAmbientSounds[5];

Sound::Sound(const char* path, void* heap) {
    this->mData = LoadContentFile(gUnk80330B64, path, 0x20, NULL, gMEM2Heap);
    this->mHeap = heap;

    if (this->mData) {
        fn_80041884(this->mData, this->mHeap, DefaultSoundAlloc, MEM2Free);
        fn_800420E4(SCGetSoundMode());
    }
}

void *DefaultSoundAlloc(u32 size) {
    return MEM2Alloc(size, 0x20);
}

Sound::~Sound() {
    if (this->mData) {
        fn_80041C38();
        MEM2Free(this->mData);
        this->mData = NULL;
    }
}

void UpdateSound() {}

void CalcSound() {
    fn_80041F54();
}

void fn_80041884(void* data, void* heap, void* (*alloc_func)(u32), void (*mem_func)(void*)) {
    fn_800418A8(1, data, 0, heap, 0, alloc_func, mem_func);
}

u8 lbl_80330C38 = 0;
u8 lbl_80330C39 = 0;
SoundAllocFunc gAllocFunc = 0;                  // 0x3C
SoundMemFunc gMemFunc = 0;                      // 0x40
void *gMemSoundArchive = NULL;                  // 0x44
nw4r::snd::SoundArchivePlayer *gArchivePlayer = NULL;  // 0x48
u32 *gIDK = NULL;  // 0x4C, something that inherits SoundArchive...
nw4r::snd::SoundHeap *gSoundHeap = NULL;        // 0x50
nw4r::snd::FxReverbHi *gFxReverb = NULL;        // 0x6C

void fn_800418A8(u8 arg1, void* data, const char *arg3, void* heap, u32 arg5, void* (*alloc_func)(u32), void (*mem_func)(void*)) {
    if (lbl_80330C39 != 0) return;

    lbl_80330C39 = 1;
    lbl_80330C38 = arg1;
    gAllocFunc = alloc_func;
    gMemFunc = mem_func;

    nw4r::snd::MemorySoundArchive *mem_sound_archive = NULL;
    nw4r::snd::DvdSoundArchive *dvd_sound_archive = NULL;
    if (arg1 != 0) {
        gMemSoundArchive = mem_sound_archive = new nw4r::snd::MemorySoundArchive();
    } else {
        gMemSoundArchive = dvd_sound_archive = new nw4r::snd::DvdSoundArchive();
    }

    gSoundHeap = new nw4r::snd::SoundHeap();
    gArchivePlayer = new nw4r::snd::SoundArchivePlayer();
    gIDK = new u32(0);
    gFxReverb = new nw4r::snd::FxReverbHi();

    if (!AICheckInit()) {
        AIInit(NULL);
        AXInit();
    }

    nw4r::snd::SoundSystem::InitSoundSystem(4, 3);

    if (lbl_80330C38 != 0) {
        mem_sound_archive->Setup(data);
    } else {
        if (!dvd_sound_archive->Open(arg3)) {
            //                          __LINE__
            OSPanic("sound_manager.cpp", 0x19D, "Cannot open Sound Archive File");
        }
    }
}
