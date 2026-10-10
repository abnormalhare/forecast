#include "nw4r/g3d/g3d_camera.h"
#include "nw4r/math/math_types.h"

class GlobeView {
public:
    GlobeView(nw4r::g3d::Camera camera);
    virtual ~GlobeView();

    void unk10(nw4r::math::VEC3 *);
    void unk14();

    nw4r::g3d::Camera *getCamera() { return &mCamera; }
    nw4r::math::VEC3 *getUnk() { return &mUnk; }
    nw4r::math::VEC3  *getPosition() { return &mPosition; }
    nw4r::math::VEC3  *getOrientation() { return &mOrientation; }
    nw4r::math::VEC3  *getLightPos() { return &mLightPos; }
    nw4r::math::VEC3  *getLightDir() { return &mLightDir; }
    u8 getResetting() { return mResetting; }
    f32 getFOVy() { return mFOVy; }
    f32 getAspect() { return mAspect; }
    f32 getNear() { return mNear; }
    f32 getFar() { return mFar; }
    f32 getZoom() { return mZoom; }

    void setZoom(f32 zoom) { mZoom = zoom; }

private:
    nw4r::g3d::Camera mCamera; // at 0x4
    u8 unk8[0x84 - 0x8];       // at 0x8
    nw4r::math::VEC3 mUnk;     // at 0x84
    nw4r::math::VEC3 mPosition;             // at 0x90
    nw4r::math::VEC3 mOrientation;          // at 0x9C
    nw4r::math::VEC3 mLightPos;             // at 0xA8
    nw4r::math::VEC3 mLightDir;             // at 0xB4
    u8 mResetting;             // at 0xC0
    u8 unkC1[0xC4 - 0xC1];     // at 0xC1
    f32 mFOVy;                 // at 0xC4
    f32 mAspect;               // at 0xC8
    f32 mNear;                 // at 0xCC
    f32 mFar;                  // at 0xD0
    f32 mZoom;                 // at 0xD4
    u8 unkD8[0xEC - 0xD8];     // at 0xD8
};
