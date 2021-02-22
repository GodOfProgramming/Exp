#import "version_directive.glsl"

in vec2 io_uv;

out vec4 o_frag_color;

uniform sampler2D tex;

void main()
{
  vec4 color   = texture(tex, io_uv);
  o_frag_color = color;
}
