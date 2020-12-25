
uniform float u_time;
uniform vec2 u_resolution;

void main()
{
    //vec2 coord = (gl_FragCoord.xy * 2.0 - u_resolution) / min(u_resolution.x, u_resolution.y);
    //float color = 0.0;
    //color += 0.1 * (abs(sin(u_time)) +0.1) / length(coord);
    //gl_FragColor = vec4(vec3(color), 1.0);
    gl_FragColor = vec4(1.0, 0.5, 1.0, 1.0);
}