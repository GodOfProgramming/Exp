#import "version_directive.glsl"

layout(location = 0) in vec3 i_pos;

out vec3 io_color;

uniform vec3 u_colors[4];

void main()
{
  io_color    = u_colors[gl_VertexID];
  gl_Position = vec4(i_pos.x, i_pos.y, i_pos.z, 1.0);
}