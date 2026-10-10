#ifndef CHANNEL_SIMPLE_GLOBE_H
#define CHANNEL_SIMPLE_GLOBE_H
#include "Vec3.h"
#include "Vector2.h"
#include "nw4r/g3d/g3d_scnroot.h"
#include <types.h>
#include <nw4r/g3d/g3d_camera.h>
#include <revolution/MTX.h>
#include "GlobeView.h"
#include "nw4r/math/math_types.h"

// d_scene's m_pSimpleGlobe (size 0xD0)
class SimpleGlobe {
public:
    SimpleGlobe();
    ~SimpleGlobe();

    void Calc();
    void SetRotation(const Vec* rotation, s32 frames);
    void SetZoom(s32 level, s32 frames);
    void SetMode(s32 mode);
    void SetSpeed(f32 speed);
    void Setup(const Vec* rotation);
    void DrawModel();
    void Draw();
    void ClearInput();
    void UpdateView();
    void UpdateFacing();
    void UpdateLight();
    void UpdateZoom(const s32* sounds);
    void SyncZoom();
    BOOL UpdateGrab(s32 chan);
    s32 UpdateDrag(s32 chan);
    BOOL IsDefaultView();
    void UpdateTilt(u32 arg, const s32* sounds);
    void UpdateRotation(u32 stop);
    void SetZoomLevel(s32 level);
    void PlayRotateSound(u32 id);
    void unk();

    GlobeView* GetView() {
        return mView;
    }

    nw4r::g3d::ScnRoot* mScnRoot; // at 0x0
    GlobeView* mView;             // at 0x4
    nw4r::math::VEC3 mRotation;   // at 0x8
    nw4r::math::VEC3 unk14;       // at 0x14
    f32 unk20;                    // at 0x20
    f32 unk24;                    // at 0x24
    f32 unk28;                    // at 0x28
    Vector2 unk2C[4];             // at 0x2C
    Vector2 unk4C[4];             // at 0x4C
    Vec2 mSpeed;                  // at 0x6C, rotation speed
    f32 unk74;
    f32 unk78;
    f32 unk7C;
    f32 unk80;
    f32 unk84;
    f32 unk88;
    u8 mGrabbed[4];               // at 0x8C
    u8 mSpinning;                 // at 0x90
    u8 mZoomIn;                   // at 0x91
    u8 mZoomOut;                  // at 0x92
    u8 mTiltUp;                   // at 0x93
    u8 mTiltDown;                 // at 0x94
    u8 unk95;
    u8 unk96;
    u8 unk97;
    u8 unk98;
    u8 unk99;
    u8 unk9A;
    u8 unk9B;
    s32 mZoomLevel;               // at 0x9C
    s32 mTiltLevel;               // at 0xA0
    u8 unkA4[0xB4 - 0xA4];        // at 0xA4
    f32 unkB4;
    f32 unkB8;
    f32 unkBC;
    f32 unkC0;
    f32 unkC4;
    f32 mZoom;                    // at 0xC8
    u8 unkCC[0xD0 - 0xCC];        // at 0xCC
};


extern SimpleGlobe* gSimpleGlobe;

#endif
