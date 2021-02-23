#import "version_directive.glsl"
#import "vertex_layout.glsl"

out vec3 io_color;

uniform vec3 u_colors[4];
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
  io_color    = i_norm - u_colors[gl_VertexID];
  gl_Position = u_projection * u_view * u_model * vec4(i_pos, 1.0);
}