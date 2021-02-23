#import "version_directive.glsl"
//

in vec2 io_uv;
in vec3 io_color;

out vec4 o_frag_color;

uniform sampler2D tex;

void main()
{
  vec4 color   = texture(tex, io_uv);
  color        = color * vec4(io_color, 1.0);
  o_frag_color = color;
}
