#import "version_directive.glsl"
#import "vertex_layout.glsl"

out vec2 io_uv;
out vec3 io_color;

uniform vec2 u_tex_coords;
uniform vec2 u_tex_ratio;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
  io_uv       = i_uv * u_tex_ratio + u_tex_coords;
  gl_Position = u_projection * u_view * u_model * vec4(i_pos, 1.0);
  // gl_Position = u_model * vec4(i_pos, 1.0);
}
