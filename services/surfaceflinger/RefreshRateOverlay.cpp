/*
 * Copyright 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "RefreshRateOverlay.h"
#include "Client.h"
#include "Layer.h"

namespace android {

using RefreshRateType = scheduler::RefreshRateConfigs::RefreshRateType;

RefreshRateOverlay::RefreshRateOverlay(SurfaceFlinger& flinger)
      : mFlinger(flinger), mClient(new Client(&mFlinger)) {
    createLayer();
}

bool RefreshRateOverlay::createLayer() {
    const status_t ret =
            mFlinger.createLayer(String8("RefreshRateOverlay"), mClient, 0, 0,
                                 PIXEL_FORMAT_RGBA_8888, ISurfaceComposerClient::eFXSurfaceColor,
                                 LayerMetadata(), &mIBinder, &mGbp, &mLayer);
    if (ret) {
        ALOGE("failed to color layer");
        return false;
    }

    mLayer = mClient->getLayerUser(mIBinder);
    mLayer->setCrop_legacy(Rect(0, 0, 200, 100), true);
    mLayer->setLayer(INT32_MAX - 2);

    return true;
}

void RefreshRateOverlay::changeRefreshRate(RefreshRateType type) {
    const half3& color = (type == RefreshRateType::PERFORMANCE) ? GREEN : RED;
    mLayer->setColor(color);
    mFlinger.setTransactionFlags(eTransactionMask);
}

}; // namespace android