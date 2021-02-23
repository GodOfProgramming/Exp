#import "version_directive.glsl"
#import "vertex_layout.glsl"

out vec2 io_uv;
out vec3 io_color;

uniform vec2 u_tex_coords;
uniform vec2 u_tex_ratio;
uniform vec3 u_color;
uniform mat4 u_transform;

void main()
{
  io_uv       = i_uv * u_tex_ratio + u_tex_coords;
  io_color    = u_color;
  gl_Position = u_transform * vec4(i_pos, 1.0);
}
