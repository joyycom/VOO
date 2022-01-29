R"(
precision mediump float;
uniform sampler2D uTexture0;
varying vec2 vTexCoord;
uniform vec2 uSrcTexSize;

vec4 cubicHermite(vec4 A, vec4 B, vec4 C, vec4 D, float t) {
    float t2 = t * t;
    float t3 = t * t * t;
    vec4 a = -A / 2.0 + (3.0 * B) / 2.0 - (3.0 * C) / 2.0 + D / 2.0;
    vec4 b = A - (5.0 * B) / 2.0 + 2.0 * C - D / 2.0;
    vec4 c = -A / 2.0 + C / 2.0;
    vec4 d = B;
    vec4 rgb = a * t3 + b * t2 + c * t + d;
    return rgb;
}

vec4 bicubic(vec2 P) {
    vec2 c_onePixel = (1.0 / uSrcTexSize);
    vec2 c_twoPixels = (2.0 / uSrcTexSize);
    vec2 pixel = P * uSrcTexSize + 0.5;
    vec2 frac = fract(pixel);
    pixel = floor(pixel) / uSrcTexSize - vec2(c_onePixel / 2.0);
    vec4 C01 = texture2D(uTexture0, pixel + vec2(-c_onePixel.x, 0.0));
    vec4 C11 = texture2D(uTexture0, pixel + vec2(0.0, 0.0));
    vec4 C21 = texture2D(uTexture0, pixel + vec2(c_onePixel.x, 0.0));
    vec4 C31 = texture2D(uTexture0, pixel + vec2(c_twoPixels.x, 0.0));
    vec4 C02 = texture2D(uTexture0, pixel + vec2(-c_onePixel.x, c_onePixel.y));
    vec4 C12 = texture2D(uTexture0, pixel + vec2(0.0, c_onePixel.y));
    vec4 C22 = texture2D(uTexture0, pixel + vec2(c_onePixel.x, c_onePixel.y));
    vec4 C32 = texture2D(uTexture0, pixel + vec2(c_twoPixels.x, c_onePixel.y));
    vec4 CP1X = cubicHermite(C01, C11, C21, C31, frac.x);
    vec4 CP2X = cubicHermite(C02, C12, C22, C32, frac.x);
    return mix(CP1X, CP2X, frac.y);
}

void main(void) {
    gl_FragColor = bicubic(vTexCoord);
}
)"
