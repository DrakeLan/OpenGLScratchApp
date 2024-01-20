#version 330

in vec3 wPos;

out vec4 color; 

uniform sampler2D equTexture;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main()
{
    vec2 uv = SampleSphericalMap(normalize(wPos)); // make sure to normalize localPos
    vec3 texColor = texture(equTexture, uv).rgb;

    color = vec4(texColor, 1.0);
    //color = vec4(1.0, 0.0, 0.0, 1.0);
}