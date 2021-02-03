#import "version_directive.glsl"
#import "test_c.glsl"
#import "test_a.glsl"

layout (location = 0) in vec3 in_pos;

void main()
{
  gl_Position = vec4(in_pos.x, in_pos.y, in_pos.z);
}
