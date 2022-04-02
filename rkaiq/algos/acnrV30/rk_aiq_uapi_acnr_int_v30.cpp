#include "acnrV30/rk_aiq_uapi_acnr_int_v30.h"
#include "acnrV30/rk_aiq_types_acnr_algo_prvt_v30.h"

#if 1
#define ACNRV30_CHROMA_SF_STRENGTH_SLOPE_FACTOR (5.0)


XCamReturn
rk_aiq_uapi_acnrV30_SetAttrib(RkAiqAlgoContext *ctx,
                              const rk_aiq_cnr_attrib_v30_t *attr,
                              bool need_sync)
{

    Acnr_Context_V30_t* pCtx = (Acnr_Context_V30_t*)ctx;

    pCtx->eMode = attr->eMode;
    if(pCtx->eMode == ACNRV30_OP_MODE_AUTO) {
        pCtx->stAuto = attr->stAuto;
    } else if(pCtx->eMode == ACNRV30_OP_MODE_MANUAL) {
        pCtx->stManual.stSelect = attr->stManual.stSelect;
    } else if(pCtx->eMode == ACNRV30_OP_MODE_REG_MANUAL) {
        pCtx->stManual.stFix = attr->stManual.stFix;
    }
    pCtx->isReCalculate |= 1;

    return XCAM_RETURN_NO_ERROR;
}

XCamReturn
rk_aiq_uapi_acnrV30_GetAttrib(const RkAiqAlgoContext *ctx,
                              rk_aiq_cnr_attrib_v30_t *attr)
{

    Acnr_Context_V30_t* pCtx = (Acnr_Context_V30_t*)ctx;

    attr->eMode = pCtx->eMode;
    memcpy(&attr->stAuto, &pCtx->stAuto, sizeof(attr->stAuto));
    memcpy(&attr->stManual, &pCtx->stManual, sizeof(attr->stManual));

    return XCAM_RETURN_NO_ERROR;
}



XCamReturn
rk_aiq_uapi_acnrV30_SetChromaSFStrength(const RkAiqAlgoContext *ctx,
                                        const rk_aiq_cnr_strength_v30_t* pStrength)
{
    Acnr_Context_V30_t* pCtx = (Acnr_Context_V30_t*)ctx;

    float fStrength = 1.0f;
    float fslope = ACNRV30_CHROMA_SF_STRENGTH_SLOPE_FACTOR;
    float fPercent = 0.5f;

    fPercent = pStrength->percent;

    if(fPercent <= 0.5) {
        fStrength =  fPercent / 0.5;
    } else {
        if(fPercent >= 0.999999)
            fPercent = 0.999999;
        fStrength = 0.5 * fslope / (1.0 - fPercent) - fslope  + 1;
    }

    pCtx->stStrength = *pStrength;
    pCtx->stStrength.percent = fStrength;
    pCtx->isReCalculate |= 1;

    return XCAM_RETURN_NO_ERROR;
}


XCamReturn
rk_aiq_uapi_acnrV30_GetChromaSFStrength(const RkAiqAlgoContext *ctx,
                                        rk_aiq_cnr_strength_v30_t* pStrength)
{
    Acnr_Context_V30_t* pCtx = (Acnr_Context_V30_t*)ctx;

    float fStrength = 1.0f;
    float fslope = ACNRV30_CHROMA_SF_STRENGTH_SLOPE_FACTOR;
    float fPercent = 0.5;

    fStrength = pCtx->stStrength.percent;


    if(fStrength <= 1) {
        fPercent = fStrength * 0.5;
    } else {
        float tmp = 1.0;
        tmp = 1 - 0.5 * fslope / (fStrength + fslope - 1);
        if(abs(tmp - 0.999999) < 0.000001) {
            tmp = 1.0;
        }
        fPercent = tmp;
    }

    *pStrength = pCtx->stStrength;
    pStrength->percent = fPercent;

    return XCAM_RETURN_NO_ERROR;
}


#endif

