static const uint8_t fs_imgui_image_glsl[352] =
{
	0x46, 0x53, 0x48, 0x0b, 0x6f, 0x1e, 0x3e, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x11, 0x75, // FSH.o.><.......u
	0x5f, 0x69, 0x6d, 0x61, 0x67, 0x65, 0x4c, 0x6f, 0x64, 0x45, 0x6e, 0x61, 0x62, 0x6c, 0x65, 0x64, // _imageLodEnabled
	0x02, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x73, 0x5f, 0x74, 0x65, 0x78, // ...........s_tex
	0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, // Color...........
	0x01, 0x00, 0x00, 0x76, 0x61, 0x72, 0x79, 0x69, 0x6e, 0x67, 0x20, 0x76, 0x65, 0x63, 0x32, 0x20, // ...varying vec2 
	0x76, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x30, 0x3b, 0x0a, 0x75, 0x6e, 0x69, // v_texcoord0;.uni
	0x66, 0x6f, 0x72, 0x6d, 0x20, 0x76, 0x65, 0x63, 0x34, 0x20, 0x75, 0x5f, 0x69, 0x6d, 0x61, 0x67, // form vec4 u_imag
	0x65, 0x4c, 0x6f, 0x64, 0x45, 0x6e, 0x61, 0x62, 0x6c, 0x65, 0x64, 0x3b, 0x0a, 0x75, 0x6e, 0x69, // eLodEnabled;.uni
	0x66, 0x6f, 0x72, 0x6d, 0x20, 0x73, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x72, 0x32, 0x44, 0x20, 0x73, // form sampler2D s
	0x5f, 0x74, 0x65, 0x78, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x3b, 0x0a, 0x76, 0x6f, 0x69, 0x64, 0x20, // _texColor;.void 
	0x6d, 0x61, 0x69, 0x6e, 0x20, 0x28, 0x29, 0x0a, 0x7b, 0x0a, 0x20, 0x20, 0x76, 0x65, 0x63, 0x34, // main ().{.  vec4
	0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, //  tmpvar_1;.  tmp
	0x76, 0x61, 0x72, 0x5f, 0x31, 0x2e, 0x78, 0x79, 0x7a, 0x20, 0x3d, 0x20, 0x74, 0x65, 0x78, 0x74, // var_1.xyz = text
	0x75, 0x72, 0x65, 0x32, 0x44, 0x4c, 0x6f, 0x64, 0x20, 0x28, 0x73, 0x5f, 0x74, 0x65, 0x78, 0x43, // ure2DLod (s_texC
	0x6f, 0x6c, 0x6f, 0x72, 0x2c, 0x20, 0x76, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, // olor, v_texcoord
	0x30, 0x2c, 0x20, 0x75, 0x5f, 0x69, 0x6d, 0x61, 0x67, 0x65, 0x4c, 0x6f, 0x64, 0x45, 0x6e, 0x61, // 0, u_imageLodEna
	0x62, 0x6c, 0x65, 0x64, 0x2e, 0x78, 0x29, 0x2e, 0x78, 0x79, 0x7a, 0x3b, 0x0a, 0x20, 0x20, 0x74, // bled.x).xyz;.  t
	0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, 0x2e, 0x77, 0x20, 0x3d, 0x20, 0x28, 0x30, 0x2e, 0x32, // mpvar_1.w = (0.2
	0x20, 0x2b, 0x20, 0x28, 0x30, 0x2e, 0x38, 0x20, 0x2a, 0x20, 0x75, 0x5f, 0x69, 0x6d, 0x61, 0x67, //  + (0.8 * u_imag
	0x65, 0x4c, 0x6f, 0x64, 0x45, 0x6e, 0x61, 0x62, 0x6c, 0x65, 0x64, 0x2e, 0x79, 0x29, 0x29, 0x3b, // eLodEnabled.y));
	0x0a, 0x20, 0x20, 0x67, 0x6c, 0x5f, 0x46, 0x72, 0x61, 0x67, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x20, // .  gl_FragColor 
	0x3d, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, 0x3b, 0x0a, 0x7d, 0x0a, 0x0a, 0x00, // = tmpvar_1;.}...
};
static const uint8_t fs_imgui_image_essl[372] =
{
	0x46, 0x53, 0x48, 0x0b, 0x6f, 0x1e, 0x3e, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x11, 0x75, // FSH.o.><.......u
	0x5f, 0x69, 0x6d, 0x61, 0x67, 0x65, 0x4c, 0x6f, 0x64, 0x45, 0x6e, 0x61, 0x62, 0x6c, 0x65, 0x64, // _imageLodEnabled
	0x02, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x73, 0x5f, 0x74, 0x65, 0x78, // ...........s_tex
	0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, // Color..........0
	0x01, 0x00, 0x00, 0x76, 0x61, 0x72, 0x79, 0x69, 0x6e, 0x67, 0x20, 0x68, 0x69, 0x67, 0x68, 0x70, // ...varying highp
	0x20, 0x76, 0x65, 0x63, 0x32, 0x20, 0x76, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, //  vec2 v_texcoord
	0x30, 0x3b, 0x0a, 0x75, 0x6e, 0x69, 0x66, 0x6f, 0x72, 0x6d, 0x20, 0x68, 0x69, 0x67, 0x68, 0x70, // 0;.uniform highp
	0x20, 0x76, 0x65, 0x63, 0x34, 0x20, 0x75, 0x5f, 0x69, 0x6d, 0x61, 0x67, 0x65, 0x4c, 0x6f, 0x64, //  vec4 u_imageLod
	0x45, 0x6e, 0x61, 0x62, 0x6c, 0x65, 0x64, 0x3b, 0x0a, 0x75, 0x6e, 0x69, 0x66, 0x6f, 0x72, 0x6d, // Enabled;.uniform
	0x20, 0x73, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x72, 0x32, 0x44, 0x20, 0x73, 0x5f, 0x74, 0x65, 0x78, //  sampler2D s_tex
	0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x3b, 0x0a, 0x76, 0x6f, 0x69, 0x64, 0x20, 0x6d, 0x61, 0x69, 0x6e, // Color;.void main
	0x20, 0x28, 0x29, 0x0a, 0x7b, 0x0a, 0x20, 0x20, 0x6c, 0x6f, 0x77, 0x70, 0x20, 0x76, 0x65, 0x63, //  ().{.  lowp vec
	0x34, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, // 4 tmpvar_1;.  tm
	0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, 0x2e, 0x78, 0x79, 0x7a, 0x20, 0x3d, 0x20, 0x74, 0x65, 0x78, // pvar_1.xyz = tex
	0x74, 0x75, 0x72, 0x65, 0x32, 0x44, 0x4c, 0x6f, 0x64, 0x20, 0x20, 0x20, 0x20, 0x28, 0x73, 0x5f, // ture2DLod    (s_
	0x74, 0x65, 0x78, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x2c, 0x20, 0x76, 0x5f, 0x74, 0x65, 0x78, 0x63, // texColor, v_texc
	0x6f, 0x6f, 0x72, 0x64, 0x30, 0x2c, 0x20, 0x75, 0x5f, 0x69, 0x6d, 0x61, 0x67, 0x65, 0x4c, 0x6f, // oord0, u_imageLo
	0x64, 0x45, 0x6e, 0x61, 0x62, 0x6c, 0x65, 0x64, 0x2e, 0x78, 0x29, 0x2e, 0x78, 0x79, 0x7a, 0x3b, // dEnabled.x).xyz;
	0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, 0x2e, 0x77, 0x20, 0x3d, 0x20, // .  tmpvar_1.w = 
	0x28, 0x30, 0x2e, 0x32, 0x20, 0x2b, 0x20, 0x28, 0x30, 0x2e, 0x38, 0x20, 0x2a, 0x20, 0x75, 0x5f, // (0.2 + (0.8 * u_
	0x69, 0x6d, 0x61, 0x67, 0x65, 0x4c, 0x6f, 0x64, 0x45, 0x6e, 0x61, 0x62, 0x6c, 0x65, 0x64, 0x2e, // imageLodEnabled.
	0x79, 0x29, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x67, 0x6c, 0x5f, 0x46, 0x72, 0x61, 0x67, 0x43, 0x6f, // y));.  gl_FragCo
	0x6c, 0x6f, 0x72, 0x20, 0x3d, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, 0x3b, 0x0a, // lor = tmpvar_1;.
	0x7d, 0x0a, 0x0a, 0x00,                                                                         // }...
};
static const uint8_t fs_imgui_image_spv[1295] =
{
	0x46, 0x53, 0x48, 0x0b, 0x6f, 0x1e, 0x3e, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x11, 0x75, // FSH.o.><.......u
	0x5f, 0x69, 0x6d, 0x61, 0x67, 0x65, 0x4c, 0x6f, 0x64, 0x45, 0x6e, 0x61, 0x62, 0x6c, 0x65, 0x64, // _imageLodEnabled
	0x12, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x73, 0x5f, 0x74, 0x65, 0x78, // ...........s_tex
	0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x30, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x02, 0x22, 0x00, 0xc8, // Color0......."..
	0x04, 0x00, 0x00, 0x03, 0x02, 0x23, 0x07, 0x00, 0x00, 0x01, 0x00, 0x0b, 0x00, 0x08, 0x00, 0xb3, // .....#..........
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0b, // ................
	0x00, 0x06, 0x00, 0x01, 0x00, 0x00, 0x00, 0x47, 0x4c, 0x53, 0x4c, 0x2e, 0x73, 0x74, 0x64, 0x2e, // .......GLSL.std.
	0x34, 0x35, 0x30, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, // 450.............
	0x00, 0x00, 0x00, 0x0f, 0x00, 0x07, 0x00, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x6d, // ...............m
	0x61, 0x69, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x6a, 0x00, 0x00, 0x00, 0x75, 0x00, 0x00, 0x00, 0x10, // ain....j...u....
	0x00, 0x03, 0x00, 0x04, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0x05, // ................
	0x00, 0x00, 0x00, 0xf4, 0x01, 0x00, 0x00, 0x05, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x00, 0x6d, // ...............m
	0x61, 0x69, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x07, 0x00, 0x23, 0x00, 0x00, 0x00, 0x73, // ain........#...s
	0x5f, 0x74, 0x65, 0x78, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x53, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x72, // _texColorSampler
	0x00, 0x00, 0x00, 0x05, 0x00, 0x07, 0x00, 0x26, 0x00, 0x00, 0x00, 0x73, 0x5f, 0x74, 0x65, 0x78, // .......&...s_tex
	0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x54, 0x65, 0x78, 0x74, 0x75, 0x72, 0x65, 0x00, 0x00, 0x00, 0x05, // ColorTexture....
	0x00, 0x06, 0x00, 0x47, 0x00, 0x00, 0x00, 0x55, 0x6e, 0x69, 0x66, 0x6f, 0x72, 0x6d, 0x42, 0x6c, // ...G...UniformBl
	0x6f, 0x63, 0x6b, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x08, 0x00, 0x47, 0x00, 0x00, 0x00, 0x00, // ock........G....
	0x00, 0x00, 0x00, 0x75, 0x5f, 0x69, 0x6d, 0x61, 0x67, 0x65, 0x4c, 0x6f, 0x64, 0x45, 0x6e, 0x61, // ...u_imageLodEna
	0x62, 0x6c, 0x65, 0x64, 0x00, 0x00, 0x00, 0x05, 0x00, 0x03, 0x00, 0x49, 0x00, 0x00, 0x00, 0x00, // bled.......I....
	0x00, 0x00, 0x00, 0x05, 0x00, 0x05, 0x00, 0x6a, 0x00, 0x00, 0x00, 0x76, 0x5f, 0x74, 0x65, 0x78, // .......j...v_tex
	0x63, 0x6f, 0x6f, 0x72, 0x64, 0x30, 0x00, 0x05, 0x00, 0x06, 0x00, 0x75, 0x00, 0x00, 0x00, 0x62, // coord0.....u...b
	0x67, 0x66, 0x78, 0x5f, 0x46, 0x72, 0x61, 0x67, 0x44, 0x61, 0x74, 0x61, 0x30, 0x00, 0x00, 0x47, // gfx_FragData0..G
	0x00, 0x04, 0x00, 0x23, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, // ...#...".......G
	0x00, 0x04, 0x00, 0x23, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x47, // ...#...!.......G
	0x00, 0x04, 0x00, 0x26, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, // ...&...".......G
	0x00, 0x04, 0x00, 0x26, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x48, // ...&...!.......H
	0x00, 0x05, 0x00, 0x47, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x00, // ...G.......#....
	0x00, 0x00, 0x00, 0x47, 0x00, 0x03, 0x00, 0x47, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x47, // ...G...G.......G
	0x00, 0x04, 0x00, 0x49, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, // ...I...".......G
	0x00, 0x04, 0x00, 0x49, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x47, // ...I...!.......G
	0x00, 0x04, 0x00, 0x6a, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, // ...j...........G
	0x00, 0x04, 0x00, 0x75, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x13, // ...u............
	0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x21, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00, 0x02, // .......!........
	0x00, 0x00, 0x00, 0x1a, 0x00, 0x02, 0x00, 0x06, 0x00, 0x00, 0x00, 0x16, 0x00, 0x03, 0x00, 0x07, // ................
	0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x19, 0x00, 0x09, 0x00, 0x08, 0x00, 0x00, 0x00, 0x07, // ... ............
	0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ................
	0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x17, 0x00, 0x04, 0x00, 0x0b, // ................
	0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x17, 0x00, 0x04, 0x00, 0x0e, // ................
	0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x22, // ........... ..."
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x22, // ...........;..."
	0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x25, // ...#....... ...%
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x25, // ...........;...%
	0x00, 0x00, 0x00, 0x26, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x15, 0x00, 0x04, 0x00, 0x29, // ...&...........)
	0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x04, 0x00, 0x29, // ... .......+...)
	0x00, 0x00, 0x00, 0x2e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1b, 0x00, 0x03, 0x00, 0x32, // ...............2
	0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x03, 0x00, 0x47, 0x00, 0x00, 0x00, 0x0e, // ...........G....
	0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x48, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x47, // ... ...H.......G
	0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x48, 0x00, 0x00, 0x00, 0x49, 0x00, 0x00, 0x00, 0x02, // ...;...H...I....
	0x00, 0x00, 0x00, 0x15, 0x00, 0x04, 0x00, 0x4f, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, // .......O... ....
	0x00, 0x00, 0x00, 0x2b, 0x00, 0x04, 0x00, 0x4f, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, // ...+...O...P....
	0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x51, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x07, // ... ...Q........
	0x00, 0x00, 0x00, 0x2b, 0x00, 0x04, 0x00, 0x07, 0x00, 0x00, 0x00, 0x57, 0x00, 0x00, 0x00, 0xcd, // ...+.......W....
	0xcc, 0x4c, 0x3e, 0x2b, 0x00, 0x04, 0x00, 0x07, 0x00, 0x00, 0x00, 0x58, 0x00, 0x00, 0x00, 0xcd, // .L>+.......X....
	0xcc, 0x4c, 0x3f, 0x2b, 0x00, 0x04, 0x00, 0x4f, 0x00, 0x00, 0x00, 0x59, 0x00, 0x00, 0x00, 0x01, // .L?+...O...Y....
	0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x69, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0b, // ... ...i........
	0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x69, 0x00, 0x00, 0x00, 0x6a, 0x00, 0x00, 0x00, 0x01, // ...;...i...j....
	0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x74, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x0e, // ... ...t........
	0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x74, 0x00, 0x00, 0x00, 0x75, 0x00, 0x00, 0x00, 0x03, // ...;...t...u....
	0x00, 0x00, 0x00, 0x36, 0x00, 0x05, 0x00, 0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, // ...6............
	0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x02, 0x00, 0x05, 0x00, 0x00, 0x00, 0x3d, // ...............=
	0x00, 0x04, 0x00, 0x06, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x3d, // .......$...#...=
	0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x27, 0x00, 0x00, 0x00, 0x26, 0x00, 0x00, 0x00, 0x3d, // .......'...&...=
	0x00, 0x04, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x6b, 0x00, 0x00, 0x00, 0x6a, 0x00, 0x00, 0x00, 0x41, // .......k...j...A
	0x00, 0x06, 0x00, 0x51, 0x00, 0x00, 0x00, 0x94, 0x00, 0x00, 0x00, 0x49, 0x00, 0x00, 0x00, 0x2e, // ...Q.......I....
	0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x07, 0x00, 0x00, 0x00, 0x95, // ...P...=........
	0x00, 0x00, 0x00, 0x94, 0x00, 0x00, 0x00, 0x56, 0x00, 0x05, 0x00, 0x32, 0x00, 0x00, 0x00, 0xaf, // .......V...2....
	0x00, 0x00, 0x00, 0x27, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x58, 0x00, 0x07, 0x00, 0x0e, // ...'...$...X....
	0x00, 0x00, 0x00, 0xb2, 0x00, 0x00, 0x00, 0xaf, 0x00, 0x00, 0x00, 0x6b, 0x00, 0x00, 0x00, 0x02, // ...........k....
	0x00, 0x00, 0x00, 0x95, 0x00, 0x00, 0x00, 0x41, 0x00, 0x06, 0x00, 0x51, 0x00, 0x00, 0x00, 0x98, // .......A...Q....
	0x00, 0x00, 0x00, 0x49, 0x00, 0x00, 0x00, 0x2e, 0x00, 0x00, 0x00, 0x59, 0x00, 0x00, 0x00, 0x3d, // ...I.......Y...=
	0x00, 0x04, 0x00, 0x07, 0x00, 0x00, 0x00, 0x99, 0x00, 0x00, 0x00, 0x98, 0x00, 0x00, 0x00, 0x0c, // ................
	0x00, 0x08, 0x00, 0x07, 0x00, 0x00, 0x00, 0x9b, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x32, // ...............2
	0x00, 0x00, 0x00, 0x58, 0x00, 0x00, 0x00, 0x99, 0x00, 0x00, 0x00, 0x57, 0x00, 0x00, 0x00, 0x51, // ...X.......W...Q
	0x00, 0x05, 0x00, 0x07, 0x00, 0x00, 0x00, 0x9e, 0x00, 0x00, 0x00, 0xb2, 0x00, 0x00, 0x00, 0x00, // ................
	0x00, 0x00, 0x00, 0x51, 0x00, 0x05, 0x00, 0x07, 0x00, 0x00, 0x00, 0x9f, 0x00, 0x00, 0x00, 0xb2, // ...Q............
	0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x51, 0x00, 0x05, 0x00, 0x07, 0x00, 0x00, 0x00, 0xa0, // .......Q........
	0x00, 0x00, 0x00, 0xb2, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x50, 0x00, 0x07, 0x00, 0x0e, // ...........P....
	0x00, 0x00, 0x00, 0xa1, 0x00, 0x00, 0x00, 0x9e, 0x00, 0x00, 0x00, 0x9f, 0x00, 0x00, 0x00, 0xa0, // ................
	0x00, 0x00, 0x00, 0x9b, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x03, 0x00, 0x75, 0x00, 0x00, 0x00, 0xa1, // .......>...u....
	0x00, 0x00, 0x00, 0xfd, 0x00, 0x01, 0x00, 0x38, 0x00, 0x01, 0x00, 0x00, 0x00, 0x10, 0x00,       // .......8.......
};
static const uint8_t fs_imgui_image_dx11[471] =
{
	0x46, 0x53, 0x48, 0x0b, 0x6f, 0x1e, 0x3e, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x11, 0x75, // FSH.o.><.......u
	0x5f, 0x69, 0x6d, 0x61, 0x67, 0x65, 0x4c, 0x6f, 0x64, 0x45, 0x6e, 0x61, 0x62, 0x6c, 0x65, 0x64, // _imageLodEnabled
	0x12, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x73, 0x5f, 0x74, 0x65, 0x78, // ...........s_tex
	0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x30, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90, // Color0..........
	0x01, 0x00, 0x00, 0x44, 0x58, 0x42, 0x43, 0x93, 0x1f, 0x8d, 0x42, 0xd8, 0x6d, 0xd1, 0x2f, 0x7c, // ...DXBC...B.m./|
	0x9d, 0x3a, 0xc5, 0x21, 0x8c, 0xa9, 0x66, 0x01, 0x00, 0x00, 0x00, 0x90, 0x01, 0x00, 0x00, 0x03, // .:.!..f.........
	0x00, 0x00, 0x00, 0x2c, 0x00, 0x00, 0x00, 0x84, 0x00, 0x00, 0x00, 0xb8, 0x00, 0x00, 0x00, 0x49, // ...,...........I
	0x53, 0x47, 0x4e, 0x50, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x38, // SGNP...........8
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, // ................
	0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // .......D........
	0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x00, 0x53, // ...............S
	0x56, 0x5f, 0x50, 0x4f, 0x53, 0x49, 0x54, 0x49, 0x4f, 0x4e, 0x00, 0x54, 0x45, 0x58, 0x43, 0x4f, // V_POSITION.TEXCO
	0x4f, 0x52, 0x44, 0x00, 0xab, 0xab, 0xab, 0x4f, 0x53, 0x47, 0x4e, 0x2c, 0x00, 0x00, 0x00, 0x01, // ORD....OSGN,....
	0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ....... ........
	0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x53, // ...............S
	0x56, 0x5f, 0x54, 0x41, 0x52, 0x47, 0x45, 0x54, 0x00, 0xab, 0xab, 0x53, 0x48, 0x45, 0x58, 0xd0, // V_TARGET...SHEX.
	0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x6a, 0x08, 0x00, 0x01, 0x59, // ...P...4...j...Y
	0x00, 0x00, 0x04, 0x46, 0x8e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x5a, // ...F. .........Z
	0x00, 0x00, 0x03, 0x00, 0x60, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x58, 0x18, 0x00, 0x04, 0x00, // ....`......X....
	0x70, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0x55, 0x00, 0x00, 0x62, 0x10, 0x00, 0x03, 0x32, // p......UU..b...2
	0x10, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x65, 0x00, 0x00, 0x03, 0xf2, 0x20, 0x10, 0x00, 0x00, // .......e.... ...
	0x00, 0x00, 0x00, 0x68, 0x00, 0x00, 0x02, 0x01, 0x00, 0x00, 0x00, 0x48, 0x00, 0x00, 0x8e, 0xc2, // ...h.......H....
	0x00, 0x00, 0x80, 0x43, 0x55, 0x15, 0x00, 0x72, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, // ...CU..r.......F
	0x10, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x46, 0x7e, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // .......F~.......
	0x60, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x80, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // `........ ......
	0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x05, 0x72, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, // ...6...r ......F
	0x02, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x0a, 0x82, 0x20, 0x10, 0x00, 0x00, // .......2.... ...
	0x00, 0x00, 0x00, 0x1a, 0x80, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, // ..... ..........
	0x40, 0x00, 0x00, 0xcd, 0xcc, 0x4c, 0x3f, 0x01, 0x40, 0x00, 0x00, 0xcd, 0xcc, 0x4c, 0x3e, 0x3e, // @....L?.@....L>>
	0x00, 0x00, 0x01, 0x00, 0x00, 0x10, 0x00,                                                       // .......
};
static const uint8_t fs_imgui_image_mtl[850] =
{
	0x46, 0x53, 0x48, 0x0b, 0x6f, 0x1e, 0x3e, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x11, 0x73, // FSH.o.><.......s
	0x5f, 0x74, 0x65, 0x78, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x53, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x72, // _texColorSampler
	0x11, 0x01, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x73, 0x5f, 0x74, 0x65, 0x78, // ...........s_tex
	0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x54, 0x65, 0x78, 0x74, 0x75, 0x72, 0x65, 0x11, 0x01, 0xff, 0xff, // ColorTexture....
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x75, 0x5f, 0x69, 0x6d, 0x61, 0x67, 0x65, 0x4c, 0x6f, // .......u_imageLo
	0x64, 0x45, 0x6e, 0x61, 0x62, 0x6c, 0x65, 0x64, 0x12, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // dEnabled........
	0x00, 0x00, 0x0a, 0x73, 0x5f, 0x74, 0x65, 0x78, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x10, 0x00, 0x00, // ...s_texColor...
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd3, 0x02, 0x00, 0x00, 0x23, 0x69, 0x6e, 0x63, 0x6c, // ...........#incl
	0x75, 0x64, 0x65, 0x20, 0x3c, 0x6d, 0x65, 0x74, 0x61, 0x6c, 0x5f, 0x73, 0x74, 0x64, 0x6c, 0x69, // ude <metal_stdli
	0x62, 0x3e, 0x0a, 0x23, 0x69, 0x6e, 0x63, 0x6c, 0x75, 0x64, 0x65, 0x20, 0x3c, 0x73, 0x69, 0x6d, // b>.#include <sim
	0x64, 0x2f, 0x73, 0x69, 0x6d, 0x64, 0x2e, 0x68, 0x3e, 0x0a, 0x0a, 0x75, 0x73, 0x69, 0x6e, 0x67, // d/simd.h>..using
	0x20, 0x6e, 0x61, 0x6d, 0x65, 0x73, 0x70, 0x61, 0x63, 0x65, 0x20, 0x6d, 0x65, 0x74, 0x61, 0x6c, //  namespace metal
	0x3b, 0x0a, 0x0a, 0x73, 0x74, 0x72, 0x75, 0x63, 0x74, 0x20, 0x5f, 0x47, 0x6c, 0x6f, 0x62, 0x61, // ;..struct _Globa
	0x6c, 0x0a, 0x7b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74, 0x34, 0x20, 0x75, // l.{.    float4 u
	0x5f, 0x69, 0x6d, 0x61, 0x67, 0x65, 0x4c, 0x6f, 0x64, 0x45, 0x6e, 0x61, 0x62, 0x6c, 0x65, 0x64, // _imageLodEnabled
	0x3b, 0x0a, 0x7d, 0x3b, 0x0a, 0x0a, 0x73, 0x74, 0x72, 0x75, 0x63, 0x74, 0x20, 0x78, 0x6c, 0x61, // ;.};..struct xla
	0x74, 0x4d, 0x74, 0x6c, 0x4d, 0x61, 0x69, 0x6e, 0x5f, 0x6f, 0x75, 0x74, 0x0a, 0x7b, 0x0a, 0x20, // tMtlMain_out.{. 
	0x20, 0x20, 0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74, 0x34, 0x20, 0x62, 0x67, 0x66, 0x78, 0x5f, 0x46, //    float4 bgfx_F
	0x72, 0x61, 0x67, 0x44, 0x61, 0x74, 0x61, 0x30, 0x20, 0x5b, 0x5b, 0x63, 0x6f, 0x6c, 0x6f, 0x72, // ragData0 [[color
	0x28, 0x30, 0x29, 0x5d, 0x5d, 0x3b, 0x0a, 0x7d, 0x3b, 0x0a, 0x0a, 0x73, 0x74, 0x72, 0x75, 0x63, // (0)]];.};..struc
	0x74, 0x20, 0x78, 0x6c, 0x61, 0x74, 0x4d, 0x74, 0x6c, 0x4d, 0x61, 0x69, 0x6e, 0x5f, 0x69, 0x6e, // t xlatMtlMain_in
	0x0a, 0x7b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74, 0x32, 0x20, 0x76, 0x5f, // .{.    float2 v_
	0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x30, 0x20, 0x5b, 0x5b, 0x75, 0x73, 0x65, 0x72, // texcoord0 [[user
	0x28, 0x6c, 0x6f, 0x63, 0x6e, 0x30, 0x29, 0x5d, 0x5d, 0x3b, 0x0a, 0x7d, 0x3b, 0x0a, 0x0a, 0x66, // (locn0)]];.};..f
	0x72, 0x61, 0x67, 0x6d, 0x65, 0x6e, 0x74, 0x20, 0x78, 0x6c, 0x61, 0x74, 0x4d, 0x74, 0x6c, 0x4d, // ragment xlatMtlM
	0x61, 0x69, 0x6e, 0x5f, 0x6f, 0x75, 0x74, 0x20, 0x78, 0x6c, 0x61, 0x74, 0x4d, 0x74, 0x6c, 0x4d, // ain_out xlatMtlM
	0x61, 0x69, 0x6e, 0x28, 0x78, 0x6c, 0x61, 0x74, 0x4d, 0x74, 0x6c, 0x4d, 0x61, 0x69, 0x6e, 0x5f, // ain(xlatMtlMain_
	0x69, 0x6e, 0x20, 0x69, 0x6e, 0x20, 0x5b, 0x5b, 0x73, 0x74, 0x61, 0x67, 0x65, 0x5f, 0x69, 0x6e, // in in [[stage_in
	0x5d, 0x5d, 0x2c, 0x20, 0x63, 0x6f, 0x6e, 0x73, 0x74, 0x61, 0x6e, 0x74, 0x20, 0x5f, 0x47, 0x6c, // ]], constant _Gl
	0x6f, 0x62, 0x61, 0x6c, 0x26, 0x20, 0x5f, 0x6d, 0x74, 0x6c, 0x5f, 0x75, 0x20, 0x5b, 0x5b, 0x62, // obal& _mtl_u [[b
	0x75, 0x66, 0x66, 0x65, 0x72, 0x28, 0x30, 0x29, 0x5d, 0x5d, 0x2c, 0x20, 0x74, 0x65, 0x78, 0x74, // uffer(0)]], text
	0x75, 0x72, 0x65, 0x32, 0x64, 0x3c, 0x66, 0x6c, 0x6f, 0x61, 0x74, 0x3e, 0x20, 0x73, 0x5f, 0x74, // ure2d<float> s_t
	0x65, 0x78, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x20, 0x5b, 0x5b, 0x74, 0x65, 0x78, 0x74, 0x75, 0x72, // exColor [[textur
	0x65, 0x28, 0x30, 0x29, 0x5d, 0x5d, 0x2c, 0x20, 0x73, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x72, 0x20, // e(0)]], sampler 
	0x73, 0x5f, 0x74, 0x65, 0x78, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x53, 0x61, 0x6d, 0x70, 0x6c, 0x65, // s_texColorSample
	0x72, 0x20, 0x5b, 0x5b, 0x73, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x72, 0x28, 0x30, 0x29, 0x5d, 0x5d, // r [[sampler(0)]]
	0x29, 0x0a, 0x7b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x78, 0x6c, 0x61, 0x74, 0x4d, 0x74, 0x6c, 0x4d, // ).{.    xlatMtlM
	0x61, 0x69, 0x6e, 0x5f, 0x6f, 0x75, 0x74, 0x20, 0x6f, 0x75, 0x74, 0x20, 0x3d, 0x20, 0x7b, 0x7d, // ain_out out = {}
	0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x6f, 0x75, 0x74, 0x2e, 0x62, 0x67, 0x66, 0x78, 0x5f, 0x46, // ;.    out.bgfx_F
	0x72, 0x61, 0x67, 0x44, 0x61, 0x74, 0x61, 0x30, 0x20, 0x3d, 0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74, // ragData0 = float
	0x34, 0x28, 0x73, 0x5f, 0x74, 0x65, 0x78, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x2e, 0x73, 0x61, 0x6d, // 4(s_texColor.sam
	0x70, 0x6c, 0x65, 0x28, 0x73, 0x5f, 0x74, 0x65, 0x78, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x53, 0x61, // ple(s_texColorSa
	0x6d, 0x70, 0x6c, 0x65, 0x72, 0x2c, 0x20, 0x69, 0x6e, 0x2e, 0x76, 0x5f, 0x74, 0x65, 0x78, 0x63, // mpler, in.v_texc
	0x6f, 0x6f, 0x72, 0x64, 0x30, 0x2c, 0x20, 0x6c, 0x65, 0x76, 0x65, 0x6c, 0x28, 0x5f, 0x6d, 0x74, // oord0, level(_mt
	0x6c, 0x5f, 0x75, 0x2e, 0x75, 0x5f, 0x69, 0x6d, 0x61, 0x67, 0x65, 0x4c, 0x6f, 0x64, 0x45, 0x6e, // l_u.u_imageLodEn
	0x61, 0x62, 0x6c, 0x65, 0x64, 0x2e, 0x78, 0x29, 0x29, 0x2e, 0x78, 0x79, 0x7a, 0x2c, 0x20, 0x66, // abled.x)).xyz, f
	0x6d, 0x61, 0x28, 0x30, 0x2e, 0x38, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x31, 0x39, 0x32, // ma(0.80000001192
	0x30, 0x39, 0x32, 0x38, 0x39, 0x35, 0x35, 0x30, 0x37, 0x38, 0x31, 0x32, 0x35, 0x2c, 0x20, 0x5f, // 0928955078125, _
	0x6d, 0x74, 0x6c, 0x5f, 0x75, 0x2e, 0x75, 0x5f, 0x69, 0x6d, 0x61, 0x67, 0x65, 0x4c, 0x6f, 0x64, // mtl_u.u_imageLod
	0x45, 0x6e, 0x61, 0x62, 0x6c, 0x65, 0x64, 0x2e, 0x79, 0x2c, 0x20, 0x30, 0x2e, 0x32, 0x30, 0x30, // Enabled.y, 0.200
	0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x39, 0x38, 0x30, 0x32, 0x33, 0x32, 0x32, 0x33, 0x38, 0x37, // 0000029802322387
	0x36, 0x39, 0x35, 0x33, 0x31, 0x32, 0x35, 0x29, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x72, // 6953125));.    r
	0x65, 0x74, 0x75, 0x72, 0x6e, 0x20, 0x6f, 0x75, 0x74, 0x3b, 0x0a, 0x7d, 0x0a, 0x0a, 0x00, 0x00, // eturn out;.}....
	0x30, 0x00,                                                                                     // 0.
};
extern const uint8_t* fs_imgui_image_pssl;
extern const uint32_t fs_imgui_image_pssl_size;
