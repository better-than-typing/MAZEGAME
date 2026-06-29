#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float time;            // seconds elapsed, set every frame from the CPU
uniform float glitchIntensity;  // 0.0 = calm, 1.0 = full chaos. Drive this from gameplay (e.g. monster proximity, sanity meter)

// ---- small hash/noise helpers ----
float hash11(float x) {
    return fract(sin(x * 127.1) * 43758.5453123);
}

float hash21(vec2 p) {
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453123);
}

// --- THIS IS DONE BY AI ---

void main() {
    vec2 uv = TexCoords;

    // --- master timing: random burst windows instead of a constant wobble ---
    // a slow-changing "burst" value that occasionally spikes near 1.0
    float burstSeed = floor(time * 6.0); // changes ~6x per second
    float burst = step(0.92, hash11(burstSeed)) * glitchIntensity;
    float steady = 0.15 * glitchIntensity; // small always-on unease, scales with intensity

    float chaos = steady + burst; // overall glitch strength for this frame

    // --- 1. block/scanline displacement ---
    // chop the screen into horizontal bands; each band gets a random horizontal jolt
    float bandY = floor(uv.y * 40.0) / 40.0;
    float bandNoise = hash21(vec2(bandY, floor(time * 12.0)));
    float bandShift = (bandNoise - 0.5) * 0.06 * chaos;
    // only a handful of bands actually shift each tick, like tape tearing
    bandShift *= step(0.85, bandNoise);

    vec2 distortedUV = uv + vec2(bandShift, 0.0);

    // --- 2. fine high-frequency jitter on top (sensor static feel) ---
    float jitter = (hash21(uv * vec2(time, time * 1.37)) - 0.5) * 0.003 * chaos;
    distortedUV += vec2(jitter, jitter * 0.5);

    // clamp so we don't sample garbage outside the framebuffer
    distortedUV = clamp(distortedUV, 0.0, 1.0);

    // --- 3. chromatic aberration / RGB channel split ---
    float caStrength = (0.004 + 0.01 * burst) * glitchIntensity;
    vec2 caDir = vec2(sin(time * 3.0), cos(time * 2.0));
    float r = texture(screenTexture, clamp(distortedUV + caDir * caStrength, 0.0, 1.0)).r;
    float g = texture(screenTexture, distortedUV).g;
    float b = texture(screenTexture, clamp(distortedUV - caDir * caStrength, 0.0, 1.0)).b;
    vec3 col = vec3(r, g, b);

    // --- 4. grainy static noise, stronger during bursts ---
    float grain = hash21(uv * (time + 0.001)) - 0.5;
    col += grain * (0.04 + 0.25 * burst) * glitchIntensity;

    // --- 5. occasional full-row tearing (a band of the image replaced by a shifted/garbled copy) ---
    float tearBand = floor(uv.y * 18.0) / 18.0;
    float tearTrigger = step(0.965, hash21(vec2(tearBand, floor(time * 9.0)))) * glitchIntensity;
    if (tearTrigger > 0.5) {
        vec2 tearUV = uv;
        tearUV.x = fract(tearUV.x + (hash11(tearBand + time) - 0.5) * 0.4);
        col = texture(screenTexture, tearUV).rgb;
        col += grain * 0.3; // dirty up the tear so it doesn't look clean
    }

    // --- 6. rare full-screen invert / flash, like a corrupted frame ---
    float invertTrigger = step(0.995, hash11(floor(time * 14.0))) * glitchIntensity;
    col = mix(col, 1.0 - col, invertTrigger);

    // --- 7. desaturate slightly and push toward sickly green/grey for dread ---
    float luma = dot(col, vec3(0.299, 0.587, 0.114));
    col = mix(col, vec3(luma), 0.25);
    col *= vec3(0.92, 1.0, 0.9);

    // --- 8. heavy dark vignette closing in around the edges ---
    vec2 centered = uv - 0.5;
    float vignette = 1.0 - smoothstep(0.25, 0.75, length(centered));
    vignette = mix(1.0, vignette, 0.85);
    // vignette pulses tighter during bursts, like the screen is closing in
    vignette -= burst * 0.15 * (1.0 - vignette);
    col *= vignette;

    FragColor = vec4(clamp(col, 0.0, 1.0), 1.0);
}
