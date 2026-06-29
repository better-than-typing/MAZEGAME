#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float time;         // seconds elapsed, set every frame from the CPU
uniform float triggerTime;  // the "time" value at the moment the entity appeared. Set ONCE when it triggers.

// How long the full glitch-out lasts, in seconds.
const float glitchDuration = 0.1;

// ---- small hash/noise helpers ----
float hash11(float x) {
    return fract(sin(x * 127.1) * 43758.5453123);
}

float hash21(vec2 p) {
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453123);
}

void main() {
    vec2 uv = TexCoords;

    // --- envelope: 0 before/after the trigger window, ramps up then drops off inside it ---
    float t = time - triggerTime; // seconds since the entity appeared
    float progress = clamp(t / glitchDuration, 0.0, 1.0);

    // fast attack, slower release - feels like a violent slam rather than a fade
    float envelope = 0.0;
    if (t >= 0.0 && t <= glitchDuration) {
        float attack = smoothstep(0.0, 0.08, progress);       // near-instant ramp up
        float release = 1.0 - smoothstep(0.6, 1.0, progress); // holds near-full then drops
        envelope = min(attack, release);
    }

    // nothing happening right now -> just show the clean screen, no cost wasted on effects
    if (envelope <= 0.001) {
        FragColor = texture(screenTexture, uv);
        return;
    }

    // chaos stays pinned near max for most of the duration, this isn't a gentle dial
    float chaos = envelope;

    // --- 1. violent block displacement: screen chopped into chunks that jump around ---
    vec2 blockUV = floor(uv * vec2(24.0, 14.0)) / vec2(24.0, 14.0);
    float blockSeed = floor(time * 30.0); // refreshes ~30x/sec, feels like signal failure
    vec2 blockNoise = vec2(
        hash21(blockUV + blockSeed),
        hash21(blockUV + blockSeed + 99.0)
    );
    vec2 blockJolt = (blockNoise - 0.5) * 0.35 * chaos;
    // most blocks jolt hard during the flash
    blockJolt *= step(0.4, hash21(blockUV + blockSeed * 0.5));

    vec2 distortedUV = uv + blockJolt;

    // --- 2. dense high-frequency jitter on top, refreshing every frame-ish ---
    float jitterSeed = floor(time * 60.0);
    vec2 jitter = (vec2(
        hash21(uv * 13.0 + jitterSeed),
        hash21(uv * 17.0 + jitterSeed + 7.0)
    ) - 0.5) * 0.05 * chaos;
    distortedUV += jitter;

    distortedUV = clamp(distortedUV, 0.0, 1.0);

    // --- 3. strong, fast-oscillating chromatic aberration: channels rip apart ---
    float caStrength = 0.05 * chaos;
    vec2 caDir = vec2(sin(time * 40.0), cos(time * 33.0));
    float r = texture(screenTexture, clamp(distortedUV + caDir * caStrength, 0.0, 1.0)).r;
    float g = texture(screenTexture, clamp(distortedUV - caDir * caStrength * 0.4, 0.0, 1.0)).g;
    float b = texture(screenTexture, clamp(distortedUV - caDir * caStrength, 0.0, 1.0)).b;
    vec3 col = vec3(r, g, b);

    // --- 4. heavy static, enough to nearly bury the image ---
    float grain = hash21(uv * 400.0 + jitterSeed) - 0.5;
    col = mix(col, vec3(0.5) + grain * 1.2, 0.45 * chaos);

    // --- 5. constant full-row tearing across most of the screen ---
    float tearBand = floor(uv.y * 30.0) / 30.0;
    float tearRoll = hash21(vec2(tearBand, blockSeed));
    float tearTrigger = step(0.45, tearRoll) * chaos; // most bands torn during the flash
    if (tearTrigger > 0.5) {
        vec2 tearUV = uv;
        tearUV.x = fract(tearUV.x + (hash11(tearBand + blockSeed) - 0.5) * 1.3);
        // occasionally pull from a totally different row too, like signal crosstalk
        tearUV.y = fract(tearUV.y + (hash11(tearBand * 3.1 + blockSeed) - 0.5) * 0.15);
        vec3 tearCol = texture(screenTexture, tearUV).rgb;
        col = mix(col, tearCol, 0.85);
    }

    // --- 6. rapid strobing invert, like the signal is failing entirely ---
    float invertSeed = floor(time * 24.0);
    float invertTrigger = step(0.6, hash11(invertSeed)) * chaos;
    col = mix(col, 1.0 - col, invertTrigger * 0.8);

    // --- 7. flickering near-black frames mixed in, like total signal dropout ---
    float dropoutSeed = floor(time * 18.0);
    float dropout = step(0.85, hash11(dropoutSeed + 50.0)) * chaos;
    col = mix(col, vec3(0.0), dropout * 0.9);

    // --- 8. sickly strobing color cast so it doesn't just look like clean TV static ---
    float hueFlicker = hash11(floor(time * 20.0) + 200.0);
    vec3 colorCast = mix(vec3(1.0, 0.15, 0.15), vec3(0.6, 1.0, 0.5), hueFlicker); // red/sick-green strobe
    col = mix(col, col * colorCast, 0.3 * chaos);

    // --- 9. crush contrast/brightness so most of the frame reads as noise, not image ---
    col = (col - 0.5) * (1.0 + 0.6 * chaos) + 0.5;

    // --- blend the whole glitched result over the clean frame using the envelope ---
    vec3 cleanCol = texture(screenTexture, uv).rgb;
    vec3 finalCol = mix(cleanCol, col, envelope);

    FragColor = vec4(clamp(finalCol, 0.0, 1.0), 1.0);
}
