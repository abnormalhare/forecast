#include "SimpleGlobe.h"
#include "DrawUtil.h"
#include "SceneBase.h"
#include "SimpleModel.h"
#include "System.h"
#include "nw4r/g3d/g3d_camera.h"
#include "nw4r/g3d/g3d_light.h"
#include "nw4r/g3d/g3d_scnobj.h"
#include "nw4r/g3d/g3d_scnroot.h"
#include "nw4r/math/math_types.h"
#include "revolution/GX/GXFrameBuf.h"
#include "revolution/GX/GXTypes.h"
#include "revolution/MTX/mtxtypes.h"
#include "revolution/MTX/vec.h"
#include <cstddef>

const f32 lbl_8018F730[10] = {
    1.0f, 2.0f, 5.0f, 8.0f, 12.0f,
    17.0f, 25.0f, 40.0f, 65.0f, 100.0f,
};

const f32 gGlobeZooms[6] = {
    0.0f, 45.0f, 55.0f, 65.0f, 73.0f, 80.0f,
};

f32 speedX = 0.0f;
f32 speedY = 0.0f;

SimpleGlobe::SimpleGlobe() :
mScnRoot(NULL), mView(NULL),
mRotation(0.0f, 0.0f, 0.0f),
unk14(0.0f, gModelRange, 0.0f),
unk20(0.0f), unk24(-gModelRange), unk28(0.0f)
{
    u32 val;

    this->mSpeed.x = 0.0f;
    this->mSpeed.y = 0.0f;

    this->unk74 = 0.0f;
    this->unk78 = 0.0f;
    this->unk7C = 0.0f;
    this->unk80 = 0.0f;
    this->unk84 = 0.0f;
    this->unk88 = 0.0f;

    this->mSpinning = 0;
    this->mZoomIn = 0;
    this->mZoomOut = 0;
    this->mTiltUp = 0;
    this->mTiltDown = 0;
    this->unk95 = 0;
    this->unk96 = 0;
    this->unk97 = 0;
    this->unk98 = 0;
    this->unk99 = 0;
    this->unk9A = 0;
    this->unk9B = 0;

    this->mZoomLevel = 0;
    this->mTiltLevel = 0;

    this->unkB4 = lbl_8018F730[8];
    this->unkB8 = lbl_8018F730[8];
    this->unkBC = 0.0f;
    this->unkC0 = 0.0f;
    this->mZoom = 1.0f;

    this->mScnRoot = nw4r::g3d::ScnRoot::Construct(&gMEM2Allocator, &val, 0x1F, 0x100, 0x80, 0x80);
    this->mScnRoot->SetCurrentCamera(0);

    this->mView = new GlobeView(this->mScnRoot->GetCurrentCamera());
    this->mView->setZoom(this->unkB4);

    this->mGrabbed[0] = 0;
    this->mGrabbed[1] = 0;
    this->mGrabbed[2] = 0;
    this->mGrabbed[3] = 0;
}

SimpleGlobe::~SimpleGlobe() {
    delete this->mView;
    this->mScnRoot->Destroy();
}

void SimpleGlobe::SetRotation(const Vec* rotation, s32 frames) {
    nw4r::g3d::ScnRoot *scnRoot = this->mScnRoot;
    this->mZoomIn = 0;
    this->mZoomOut = 0;
    this->mTiltUp = 0;
    this->mTiltDown = 0;

    while (scnRoot->Size() != 0) {
        u32 i = scnRoot->Size();
        if (i > 0) {
            scnRoot->Remove(--i);
        }
    }

    if (gEarthModel != 0) {
        gEarthModel->Calc();
        this->mScnRoot->Insert(this->mScnRoot->Size(), (nw4r::g3d::ScnObj *)gEarthModel->mScnMdl);
    }

    this->mGrabbed[0] = 0;
    this->mGrabbed[1] = 0;
    this->mGrabbed[2] = 0;
    this->mGrabbed[3] = 0;

    this->mRotation = *rotation;

    this->mSpinning = 0;

    this->mSpeed.x = speedX;
    this->mSpeed.y = speedY;

    this->mZoomLevel = frames;

    this->unkB4 = lbl_8018F730[frames];
    this->unkB8 = lbl_8018F730[frames];

    this->mView->setZoom(lbl_8018F730[frames]);

    this->mView->unk10(&this->mRotation);

    nw4r::g3d::Camera camera = this->mScnRoot->GetCamera(1);
    camera.Init(gRenderMode.fbWidth, gRenderMode.efbHeight, gRenderMode.fbWidth, gRenderMode.xfbHeight, (gWidescreen) ? 0x340 : 0x260, 0x1C8);
    camera.SetPerspective(this->mView->getFOVy(), this->mView->getAspect(), this->mView->getNear(), this->mView->getFar());
    camera.SetScissor(0, 0, gRenderMode.fbWidth, gRenderMode.efbHeight);
    camera.SetViewport(0.0f, 0.0f, (int)gRenderMode.fbWidth, (int)gRenderMode.efbHeight);
}

GXColor InitColor = { 0 };

void SimpleGlobe::Setup(const Vec* rotation) {
    this->SetRotation(rotation, this->mZoomLevel);
    if (gEarthModel != NULL) {
        gEarthModel->Calc();
    }

    nw4r::g3d::LightSet lightSet = this->mScnRoot->GetLightSet(0);
    lightSet.SelectLightObj(0, 0);
    lightSet.SelectLightObj(1, -1);
    lightSet.SelectLightObj(2, -1);
    lightSet.SelectLightObj(3, -1);
    lightSet.SelectLightObj(4, -1);
    lightSet.SelectLightObj(5, -1);
    lightSet.SelectLightObj(6, -1);
    lightSet.SelectLightObj(7, -1);
    lightSet.SelectAmbLightObj(-1);

    nw4r::g3d::LightObj *lightObj = lightSet.GetLightObj(0);
    lightObj->Clear();

    lightObj->InitLightColor(InitColor);
    lightObj->InitLightAttnA(1.0f, 0.0f, 0.0f);
    lightObj->InitLightAttnK(1.0f, 0.0f, 0.0f);
    lightObj->Enable();

    GXColor copyColor = { 0, 0, 0, 0xFF };
    GXSetCopyClear(copyColor, 0xFFFFFF);
}

void SimpleGlobe::DrawModel() {
    if (gEarthModel) {
        gEarthModel->Draw();
    }
}

void SimpleGlobe::Draw() {
    if (this->mScnRoot) {
        this->mScnRoot->DrawOpa();
        this->mScnRoot->DrawXlu();
    }

    if (gGlobeAlpha) {
        SetDefaultGXState();
        SetOrthoProjection();

        Rect rect(0.0f, 0.0f, (gWidescreen) ? 0x340 : 0x260, 456.0f);
        GXColor color;
        color.r = 0;
        color.g = 0;
        color.b = 0;
        color.a = gGlobeAlpha;
        DrawRect(&rect, &color);
    }
}

// inlined in constructor?
void SimpleGlobe::ClearInput() {
    this->mZoomIn = 0;
    this->mZoomOut = 0;
    this->mTiltUp = 0;
    this->mTiltDown = 0;

    nw4r::g3d::ScnRoot *scnRoot = this->mScnRoot;
    while (scnRoot->Size() != 0) {
        u32 i = scnRoot->Size();
        if (i > 0) {
            scnRoot->Remove(--i);
        }
    }

    if (gEarthModel != 0) {
        gEarthModel->Calc();
        this->mScnRoot->Insert(this->mScnRoot->Size(), (nw4r::g3d::ScnObj *)gEarthModel->mScnMdl);
    }
}

void SimpleGlobe::UpdateView() {
    if (this->mView) {
        this->unk();
        this->mView->unk14();
    }
}

// TODO
void SimpleGlobe::UpdateFacing() {
    if (gEarthModel) {
        gEarthModel->UpdateMtx();
    }

    nw4r::math::VEC3 out1;
    nw4r::math::VEC3Sub(&out1, this->mView->getUnk(), this->mView->getLightPos());

    nw4r::math::VEC3 out2;
    nw4r::math::VEC3Sub(&out2, &this->unk14, this->mView->getLightPos());

    PSVECNormalize(reinterpret_cast<Vec *>(&out1), reinterpret_cast<Vec *>(&out1));
    PSVECNormalize(reinterpret_cast<Vec *>(&this->unk14), reinterpret_cast<Vec *>(&this->unk14));
    PSVECNormalize(reinterpret_cast<Vec *>(&out2), reinterpret_cast<Vec *>(&out2));
}

void SimpleGlobe::UpdateLight() {
    if (!this->mView) return;
    if (!this->mScnRoot) return;

    GlobeView *view = this->mView;

    nw4r::g3d::LightSet lightSet = this->mScnRoot->GetLightSet(0);
    if (lightSet.getSetting()->GetNumLightObj() == 0) return;

    for (int i = 0; i < lightSet.getSetting()->GetNumLightObj(); i++) {
        nw4r::g3d::LightObj *lightObj = lightSet.GetLightObj(i);
        if (lightObj) {
            lightObj->InitLightPos(view->getLightPos()->x, view->getLightPos()->y, view->getLightPos()->z);
            lightObj->InitLightDir(view->getLightDir()->x, view->getLightDir()->y, view->getLightDir()->z);
        }
    }
}

void SimpleGlobe::Calc() {
    if (!this->mScnRoot) return;

    this->mScnRoot->UpdateFrame();
    this->mScnRoot->CalcWorld();
    this->mScnRoot->CalcMaterial();
    this->mScnRoot->CalcView();
    this->mScnRoot->GatherDrawScnObj();
    this->mScnRoot->ZSort();
}

void SimpleGlobe::SyncZoom() {
    if (this->mView != 0) {
        this->mZoom = this->mView->getZoom();
    }
}

nw4r::math::VEC3 GetPosition(GlobeView *view) {
    return *view->getPosition();
}

nw4r::math::VEC3 GetOrientation(GlobeView *view) {
    return *view->getOrientation();
}
