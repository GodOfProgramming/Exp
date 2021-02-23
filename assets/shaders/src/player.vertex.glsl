#import "version_directive.glsl"
#import "vertex_layout.glsl"

out vec2 io_uv;
out vec3 io_color;

uniform vec2 u_tex_coords;
uniform vec2 u_tex_ratio;
uniform vec3 u_color;

void main()
{
  io_uv       = i_uv * u_tex_ratio + u_tex_coords;
  io_color    = u_color;
  gl_Position = vec4(i_pos.x, i_pos.y, i_pos.z, 1.0);
}
