#include "fl/corkscrew.h"
#include "fl/math.h"

#define TWO_PI (PI * 2.0)

namespace fl {

// Corkscrew-to-cylindrical projection function
void Corkscrew::generateMap(const Corkscrew::Input &input,
                            Corkscrew::Output &output) {
    // Calculate vertical segments based on number of turns
    // For a single turn (2π), we want exactly 1 vertical segment
    // For two turns (4π), we want exactly 2 vertical segments
    uint16_t verticalSegments = round(input.totalAngle / TWO_PI);

    // Determine cylindrical dimensions
    output.height = verticalSegments;
    output.width = ceil(input.totalCircumference);

    output.mapping.clear();
    output.mapping.reserve(output.width * output.height);

    // Clear and prepare LED mapping if numLeds is specified
    output.ledMapping.clear();
    if (input.numLeds > 0) {
        output.ledMapping.reserve(input.numLeds);
    }

    // Corrected super sampling step size
    float thetaStep = 0.5f / output.width;
    float hStep = 0.5f / output.height;

    // Precompute angle per segment
    float anglePerSegment = input.totalAngle / verticalSegments;

    // Loop over cylindrical pixels
    for (uint16_t h = 0; h < output.height; ++h) {
        float segmentOffset = input.offsetCircumference * h;
        for (uint16_t w = 0; w < output.width; ++w) {
            vec2f sample = {0, 0};
            // 2x2 supersampling
            for (uint8_t ssH = 0; ssH < 2; ++ssH) {
                for (uint8_t ssW = 0; ssW < 2; ++ssW) {
                    float theta = (w + 0.5f + ssW * thetaStep) / output.width;
                    float height = (h + 0.5f + ssH * hStep) / output.height;

                    // Corkscrew projection (θ,h)
                    float corkscrewTheta = theta * TWO_PI + anglePerSegment * h;
                    float corkscrewH = height * verticalSegments;

                    // Apply circumference offset
                    float corkscrewCircumference = fmodf(
                        corkscrewTheta * input.totalCircumference / TWO_PI +
                            segmentOffset,
                        input.totalCircumference);

                    // Accumulate samples
                    sample.x += corkscrewCircumference;
                    sample.y += corkscrewH;
                }
            }

            // Average the supersampled points
            sample.x *= 0.25f;
            sample.y *= 0.25f;

            output.mapping.push_back(sample);

            // Optionally compact the mapping into Vec2u8 format
            if (input.compact) {
                fl::vec2u8 compactSample = {
                    static_cast<uint8_t>((sample.x / input.totalCircumference) *
                                         255),
                    static_cast<uint8_t>((sample.y / verticalSegments) * 255)};
                output.mappingCompact.push_back(compactSample);
            }
        }
    }
    
    // Generate LED mapping if numLeds is specified
    if (input.numLeds > 0) {
        for (uint16_t i = 0; i < input.numLeds; ++i) {
            // Calculate position along the corkscrew (0.0 to 1.0)
            float position = static_cast<float>(i) / (input.numLeds - 1);
            
            // Calculate angle and height
            float angle = position * input.totalAngle;
            float height = position * verticalSegments;
            
            // Calculate circumference position
            float circumference = fmodf(
                angle * input.totalCircumference / TWO_PI,
                input.totalCircumference);
            
            // Store the mapping
            output.ledMapping.push_back({circumference, height});
        }
    }
}

} // namespace fl
