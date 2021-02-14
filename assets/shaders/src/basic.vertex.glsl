#import "version_directive.glsl"

layout(location = 0) in vec3 i_pos;

void main()
{
  gl_Position = vec4(i_pos.x, i_pos.y, i_pos.z, 1.0);
}
