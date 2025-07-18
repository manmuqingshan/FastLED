#pragma once

#include "fl/stdint.h"
#include <vector>

#include "platforms/wasm/compiler/Arduino.h"
#include "platforms/wasm/js.h"

#include "crgb.h"
#include "eorder.h"
#include "fl/namespace.h"
#include "fl/singleton.h"
#include "pixel_controller.h"
#include "pixel_iterator.h"

#include "active_strip_data.h"

#include "strip_id_map.h"

namespace fl {

#define FASTLED_HAS_CLOCKLESS 1

template <int DATA_PIN, int T1, int T2, int T3, EOrder RGB_ORDER = RGB,
          int XTRA0 = 0, bool FLIP = false, int WAIT_TIME = 0>
class ClocklessController : public CPixelLEDController<RGB_ORDER> {
  public:
    virtual void init() {}
    ClocklessController() { mId = StripIdMap::addOrGetId(this); }

  protected:
    virtual void showPixels(PixelController<RGB_ORDER> &pixels) {
        // FASTLED_WARN("\n\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n\nshowPixels\n\n");
        mRgb.clear();
        ActiveStripData &ch_data = fl::Singleton<ActiveStripData>::instance();
        pixels.disableColorAdjustment();
        PixelController<RGB> pixels_rgb = pixels; // Converts to RGB pixels
        auto iterator = pixels_rgb.as_iterator(RgbwInvalid());
        mRgb.resize(iterator.size() * 3);
        auto out_iterator = mRgb.data();
        while (iterator.has(1)) {
            uint8_t r, g, b;
            iterator.loadAndScaleRGB(&r, &g, &b);
            out_iterator[0] = r;
            out_iterator[1] = g;
            out_iterator[2] = b;
            out_iterator += 3;
            iterator.advanceData();
        }
        const uint8_t *rgb = mRgb.data();
        ch_data.update(mId, millis(), rgb, mRgb.size());
    }

  private:
    int mId = 0;
    std::vector<uint8_t> mRgb;
};

} // namespace fl
