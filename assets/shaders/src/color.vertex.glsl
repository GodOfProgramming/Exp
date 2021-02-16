#import "version_directive.glsl"
#import "vertex_layout.glsl"

out vec3 io_color;

uniform vec3 u_colors[4];

void main()
{
  io_color    = i_norm - u_colors[gl_VertexID];
  gl_Position = vec4(i_pos.x, i_pos.y, i_pos.z, 1.0);
}