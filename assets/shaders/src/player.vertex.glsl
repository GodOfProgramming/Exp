#import "version_directive.glsl"
#import "vertex_layout.glsl"

out vec2 io_uv;

uniform vec2 u_tex_coords;

void main()
{
  io_uv       = i_uv;  // + u_tex_coords;
  gl_Position = vec4(i_pos.x, i_pos.y, i_pos.z, 1.0);
}
