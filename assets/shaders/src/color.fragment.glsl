#import "version_directive.glsl"

in vec3 io_color;

out vec4 o_frag_color;

void main()
{
  o_frag_color = vec4(io_color, 1.0);
}
