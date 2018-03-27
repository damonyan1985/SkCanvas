/*
 * Copyright 2018 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/**************************************************************************************************
 *** This file was autogenerated from GrYUVtoRGBEffect.fp; do not modify.
 **************************************************************************************************/
#include "GrYUVtoRGBEffect.h"
#if SK_SUPPORT_GPU

static const float kJPEGConversionMatrix[16] = {
        1.0f, 0.0f,   1.402f, -0.703749f, 1.0f, -0.344136f, -0.714136f, 0.531211f,
        1.0f, 1.772f, 0.0f,   -0.889475f, 0.0f, 0.0f,       0.0f,       1.0};

static const float kRec601ConversionMatrix[16] = {
        1.164f, 0.0f,   1.596f, -0.87075f, 1.164f, -0.391f, -0.813f, 0.52925f,
        1.164f, 2.018f, 0.0f,   -1.08175f, 0.0f,   0.0f,    0.0f,    1.0};

static const float kRec709ConversionMatrix[16] = {
        1.164f, 0.0f,   1.793f, -0.96925f, 1.164f, -0.213f, -0.533f, 0.30025f,
        1.164f, 2.112f, 0.0f,   -1.12875f, 0.0f,   0.0f,    0.0f,    1.0f};

std::unique_ptr<GrFragmentProcessor> GrYUVtoRGBEffect::Make(sk_sp<GrTextureProxy> yProxy,
                                                            sk_sp<GrTextureProxy>
                                                                    uProxy,
                                                            sk_sp<GrTextureProxy>
                                                                    vProxy,
                                                            const SkISize sizes[3],
                                                            SkYUVColorSpace colorSpace,
                                                            bool nv12) {
    SkScalar w[3], h[3];
    w[0] = SkIntToScalar(sizes[0].fWidth);
    h[0] = SkIntToScalar(sizes[0].fHeight);
    w[1] = SkIntToScalar(sizes[1].fWidth);
    h[1] = SkIntToScalar(sizes[1].fHeight);
    w[2] = SkIntToScalar(sizes[2].fWidth);
    h[2] = SkIntToScalar(sizes[2].fHeight);
    SkMatrix yTransform = SkMatrix::I();
    SkMatrix uTransform = SkMatrix::MakeScale(w[1] / w[0], h[1] / h[0]);
    SkMatrix vTransform = SkMatrix::MakeScale(w[2] / w[0], h[2] / h[0]);
    GrSamplerState::Filter uvFilterMode =
            ((sizes[1].fWidth != sizes[0].fWidth) || (sizes[1].fHeight != sizes[0].fHeight) ||
             (sizes[2].fWidth != sizes[0].fWidth) || (sizes[2].fHeight != sizes[0].fHeight))
                    ? GrSamplerState::Filter::kBilerp
                    : GrSamplerState::Filter::kNearest;
    SkMatrix44 mat(SkMatrix44::kUninitialized_Constructor);
    switch (colorSpace) {
        case kJPEG_SkYUVColorSpace:
            mat.setColMajorf(kJPEGConversionMatrix);
            break;
        case kRec601_SkYUVColorSpace:
            mat.setColMajorf(kRec601ConversionMatrix);
            break;
        case kRec709_SkYUVColorSpace:
            mat.setColMajorf(kRec709ConversionMatrix);
            break;
    }
    return std::unique_ptr<GrFragmentProcessor>(new GrYUVtoRGBEffect(
            std::move(yProxy), yTransform, std::move(uProxy), uTransform, std::move(vProxy),
            vTransform, mat, nv12, GrSamplerState(GrSamplerState::WrapMode::kClamp, uvFilterMode)));
}
#include "glsl/GrGLSLFragmentProcessor.h"
#include "glsl/GrGLSLFragmentShaderBuilder.h"
#include "glsl/GrGLSLProgramBuilder.h"
#include "GrTexture.h"
#include "SkSLCPP.h"
#include "SkSLUtil.h"
class GrGLSLYUVtoRGBEffect : public GrGLSLFragmentProcessor {
public:
    GrGLSLYUVtoRGBEffect() {}
    void emitCode(EmitArgs& args) override {
        GrGLSLFPFragmentBuilder* fragBuilder = args.fFragBuilder;
        const GrYUVtoRGBEffect& _outer = args.fFp.cast<GrYUVtoRGBEffect>();
        (void)_outer;
        auto ySamplerTransform = _outer.ySamplerTransform();
        (void)ySamplerTransform;
        auto uSamplerTransform = _outer.uSamplerTransform();
        (void)uSamplerTransform;
        auto vSamplerTransform = _outer.vSamplerTransform();
        (void)vSamplerTransform;
        auto colorSpaceMatrix = _outer.colorSpaceMatrix();
        (void)colorSpaceMatrix;
        auto nv12 = _outer.nv12();
        (void)nv12;
        fColorSpaceMatrixVar =
                args.fUniformHandler->addUniform(kFragment_GrShaderFlag, kHalf4x4_GrSLType,
                                                 kDefault_GrSLPrecision, "colorSpaceMatrix");
        SkString sk_TransformedCoords2D_0 = fragBuilder->ensureCoords2D(args.fTransformedCoords[0]);
        SkString sk_TransformedCoords2D_1 = fragBuilder->ensureCoords2D(args.fTransformedCoords[1]);
        SkString sk_TransformedCoords2D_2 = fragBuilder->ensureCoords2D(args.fTransformedCoords[2]);
        fragBuilder->codeAppendf(
                "@if (%s) {\n    %s = half4(texture(%s, %s).%s.x, texture(%s, %s).%s.xy, 1.0) * "
                "%s;\n} else {\n    %s = half4(texture(%s, %s).%s.x, texture(%s, %s).%s.x, "
                "texture(%s, %s).%s.x, 1.0) * %s;\n}\n",
                (_outer.nv12() ? "true" : "false"), args.fOutputColor,
                fragBuilder->getProgramBuilder()->samplerVariable(args.fTexSamplers[0]).c_str(),
                sk_TransformedCoords2D_0.c_str(),
                fragBuilder->getProgramBuilder()->samplerSwizzle(args.fTexSamplers[0]).c_str(),
                fragBuilder->getProgramBuilder()->samplerVariable(args.fTexSamplers[1]).c_str(),
                sk_TransformedCoords2D_1.c_str(),
                fragBuilder->getProgramBuilder()->samplerSwizzle(args.fTexSamplers[1]).c_str(),
                args.fUniformHandler->getUniformCStr(fColorSpaceMatrixVar), args.fOutputColor,
                fragBuilder->getProgramBuilder()->samplerVariable(args.fTexSamplers[0]).c_str(),
                sk_TransformedCoords2D_0.c_str(),
                fragBuilder->getProgramBuilder()->samplerSwizzle(args.fTexSamplers[0]).c_str(),
                fragBuilder->getProgramBuilder()->samplerVariable(args.fTexSamplers[1]).c_str(),
                sk_TransformedCoords2D_1.c_str(),
                fragBuilder->getProgramBuilder()->samplerSwizzle(args.fTexSamplers[1]).c_str(),
                fragBuilder->getProgramBuilder()->samplerVariable(args.fTexSamplers[2]).c_str(),
                sk_TransformedCoords2D_2.c_str(),
                fragBuilder->getProgramBuilder()->samplerSwizzle(args.fTexSamplers[2]).c_str(),
                args.fUniformHandler->getUniformCStr(fColorSpaceMatrixVar));
    }

private:
    void onSetData(const GrGLSLProgramDataManager& pdman,
                   const GrFragmentProcessor& _proc) override {
        const GrYUVtoRGBEffect& _outer = _proc.cast<GrYUVtoRGBEffect>();
        {
            float colorSpaceMatrixValue[16];
            _outer.colorSpaceMatrix().asColMajorf(colorSpaceMatrixValue);
            pdman.setMatrix4f(fColorSpaceMatrixVar, colorSpaceMatrixValue);
        }
    }
    UniformHandle fColorSpaceMatrixVar;
};
GrGLSLFragmentProcessor* GrYUVtoRGBEffect::onCreateGLSLInstance() const {
    return new GrGLSLYUVtoRGBEffect();
}
void GrYUVtoRGBEffect::onGetGLSLProcessorKey(const GrShaderCaps& caps,
                                             GrProcessorKeyBuilder* b) const {
    b->add32((int32_t)fNv12);
}
bool GrYUVtoRGBEffect::onIsEqual(const GrFragmentProcessor& other) const {
    const GrYUVtoRGBEffect& that = other.cast<GrYUVtoRGBEffect>();
    (void)that;
    if (fYSampler != that.fYSampler) return false;
    if (fYSamplerTransform != that.fYSamplerTransform) return false;
    if (fUSampler != that.fUSampler) return false;
    if (fUSamplerTransform != that.fUSamplerTransform) return false;
    if (fVSampler != that.fVSampler) return false;
    if (fVSamplerTransform != that.fVSamplerTransform) return false;
    if (fColorSpaceMatrix != that.fColorSpaceMatrix) return false;
    if (fNv12 != that.fNv12) return false;
    return true;
}
GrYUVtoRGBEffect::GrYUVtoRGBEffect(const GrYUVtoRGBEffect& src)
        : INHERITED(kGrYUVtoRGBEffect_ClassID, src.optimizationFlags())
        , fYSampler(src.fYSampler)
        , fYSamplerTransform(src.fYSamplerTransform)
        , fUSampler(src.fUSampler)
        , fUSamplerTransform(src.fUSamplerTransform)
        , fVSampler(src.fVSampler)
        , fVSamplerTransform(src.fVSamplerTransform)
        , fColorSpaceMatrix(src.fColorSpaceMatrix)
        , fNv12(src.fNv12)
        , fYSamplerCoordTransform(src.fYSamplerCoordTransform)
        , fUSamplerCoordTransform(src.fUSamplerCoordTransform)
        , fVSamplerCoordTransform(src.fVSamplerCoordTransform) {
    this->addTextureSampler(&fYSampler);
    this->addTextureSampler(&fUSampler);
    this->addTextureSampler(&fVSampler);
    this->addCoordTransform(&fYSamplerCoordTransform);
    this->addCoordTransform(&fUSamplerCoordTransform);
    this->addCoordTransform(&fVSamplerCoordTransform);
}
std::unique_ptr<GrFragmentProcessor> GrYUVtoRGBEffect::clone() const {
    return std::unique_ptr<GrFragmentProcessor>(new GrYUVtoRGBEffect(*this));
}
#endif