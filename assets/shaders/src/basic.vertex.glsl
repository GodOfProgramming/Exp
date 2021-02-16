#import "version_directive.glsl"
#import "vertex_layout.glsl"

void main()
{
  gl_Position = vec4(i_pos.x, i_pos.y, i_pos.z, 1.0);
}
