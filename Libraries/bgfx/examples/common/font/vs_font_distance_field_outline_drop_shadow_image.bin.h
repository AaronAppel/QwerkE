static const uint8_t vs_font_distance_field_outline_drop_shadow_image_glsl[634] =
{
	0x56, 0x53, 0x48, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x9e, 0x67, 0x7a, 0xc3, 0x01, 0x00, 0x0f, 0x75, // VSH......gz....u
	0x5f, 0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x56, 0x69, 0x65, 0x77, 0x50, 0x72, 0x6f, 0x6a, 0x04, 0x01, // _modelViewProj..
	0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4d, 0x02, 0x00, 0x00, 0x61, 0x74, 0x74, 0x72, // ........M...attr
	0x69, 0x62, 0x75, 0x74, 0x65, 0x20, 0x76, 0x65, 0x63, 0x34, 0x20, 0x61, 0x5f, 0x63, 0x6f, 0x6c, // ibute vec4 a_col
	0x6f, 0x72, 0x30, 0x3b, 0x0a, 0x61, 0x74, 0x74, 0x72, 0x69, 0x62, 0x75, 0x74, 0x65, 0x20, 0x76, // or0;.attribute v
	0x65, 0x63, 0x34, 0x20, 0x61, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x31, 0x3b, 0x0a, 0x61, 0x74, // ec4 a_color1;.at
	0x74, 0x72, 0x69, 0x62, 0x75, 0x74, 0x65, 0x20, 0x76, 0x65, 0x63, 0x32, 0x20, 0x61, 0x5f, 0x70, // tribute vec2 a_p
	0x6f, 0x73, 0x69, 0x74, 0x69, 0x6f, 0x6e, 0x3b, 0x0a, 0x61, 0x74, 0x74, 0x72, 0x69, 0x62, 0x75, // osition;.attribu
	0x74, 0x65, 0x20, 0x76, 0x65, 0x63, 0x34, 0x20, 0x61, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, // te vec4 a_texcoo
	0x72, 0x64, 0x30, 0x3b, 0x0a, 0x61, 0x74, 0x74, 0x72, 0x69, 0x62, 0x75, 0x74, 0x65, 0x20, 0x76, // rd0;.attribute v
	0x65, 0x63, 0x34, 0x20, 0x61, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x31, 0x3b, // ec4 a_texcoord1;
	0x0a, 0x61, 0x74, 0x74, 0x72, 0x69, 0x62, 0x75, 0x74, 0x65, 0x20, 0x76, 0x65, 0x63, 0x34, 0x20, // .attribute vec4 
	0x61, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x32, 0x3b, 0x0a, 0x76, 0x61, 0x72, // a_texcoord2;.var
	0x79, 0x69, 0x6e, 0x67, 0x20, 0x76, 0x65, 0x63, 0x34, 0x20, 0x76, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, // ying vec4 v_colo
	0x72, 0x30, 0x3b, 0x0a, 0x76, 0x61, 0x72, 0x79, 0x69, 0x6e, 0x67, 0x20, 0x76, 0x65, 0x63, 0x34, // r0;.varying vec4
	0x20, 0x76, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x31, 0x3b, 0x0a, 0x76, 0x61, 0x72, 0x79, 0x69, //  v_color1;.varyi
	0x6e, 0x67, 0x20, 0x76, 0x65, 0x63, 0x34, 0x20, 0x76, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, // ng vec4 v_texcoo
	0x72, 0x64, 0x30, 0x3b, 0x0a, 0x76, 0x61, 0x72, 0x79, 0x69, 0x6e, 0x67, 0x20, 0x76, 0x65, 0x63, // rd0;.varying vec
	0x34, 0x20, 0x76, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x31, 0x3b, 0x0a, 0x76, // 4 v_texcoord1;.v
	0x61, 0x72, 0x79, 0x69, 0x6e, 0x67, 0x20, 0x76, 0x65, 0x63, 0x34, 0x20, 0x76, 0x5f, 0x74, 0x65, // arying vec4 v_te
	0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x32, 0x3b, 0x0a, 0x75, 0x6e, 0x69, 0x66, 0x6f, 0x72, 0x6d, // xcoord2;.uniform
	0x20, 0x6d, 0x61, 0x74, 0x34, 0x20, 0x75, 0x5f, 0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x56, 0x69, 0x65, //  mat4 u_modelVie
	0x77, 0x50, 0x72, 0x6f, 0x6a, 0x3b, 0x0a, 0x76, 0x6f, 0x69, 0x64, 0x20, 0x6d, 0x61, 0x69, 0x6e, // wProj;.void main
	0x20, 0x28, 0x29, 0x0a, 0x7b, 0x0a, 0x20, 0x20, 0x76, 0x65, 0x63, 0x34, 0x20, 0x74, 0x6d, 0x70, //  ().{.  vec4 tmp
	0x76, 0x61, 0x72, 0x5f, 0x31, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, // var_1;.  tmpvar_
	0x31, 0x2e, 0x7a, 0x77, 0x20, 0x3d, 0x20, 0x76, 0x65, 0x63, 0x32, 0x28, 0x30, 0x2e, 0x30, 0x2c, // 1.zw = vec2(0.0,
	0x20, 0x31, 0x2e, 0x30, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, //  1.0);.  tmpvar_
	0x31, 0x2e, 0x78, 0x79, 0x20, 0x3d, 0x20, 0x61, 0x5f, 0x70, 0x6f, 0x73, 0x69, 0x74, 0x69, 0x6f, // 1.xy = a_positio
	0x6e, 0x3b, 0x0a, 0x20, 0x20, 0x67, 0x6c, 0x5f, 0x50, 0x6f, 0x73, 0x69, 0x74, 0x69, 0x6f, 0x6e, // n;.  gl_Position
	0x20, 0x3d, 0x20, 0x28, 0x75, 0x5f, 0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x56, 0x69, 0x65, 0x77, 0x50, //  = (u_modelViewP
	0x72, 0x6f, 0x6a, 0x20, 0x2a, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, 0x29, 0x3b, // roj * tmpvar_1);
	0x0a, 0x20, 0x20, 0x76, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x30, 0x20, 0x3d, // .  v_texcoord0 =
	0x20, 0x61, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x30, 0x3b, 0x0a, 0x20, 0x20, //  a_texcoord0;.  
	0x76, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x31, 0x20, 0x3d, 0x20, 0x61, 0x5f, // v_texcoord1 = a_
	0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x31, 0x3b, 0x0a, 0x20, 0x20, 0x76, 0x5f, 0x74, // texcoord1;.  v_t
	0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x32, 0x20, 0x3d, 0x20, 0x61, 0x5f, 0x74, 0x65, 0x78, // excoord2 = a_tex
	0x63, 0x6f, 0x6f, 0x72, 0x64, 0x32, 0x3b, 0x0a, 0x20, 0x20, 0x76, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, // coord2;.  v_colo
	0x72, 0x30, 0x20, 0x3d, 0x20, 0x61, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x30, 0x3b, 0x0a, 0x20, // r0 = a_color0;. 
	0x20, 0x76, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x31, 0x20, 0x3d, 0x20, 0x61, 0x5f, 0x63, 0x6f, //  v_color1 = a_co
	0x6c, 0x6f, 0x72, 0x31, 0x3b, 0x0a, 0x7d, 0x0a, 0x0a, 0x00,                                     // lor1;.}...
};
static const uint8_t vs_font_distance_field_outline_drop_shadow_image_essl[712] =
{
	0x56, 0x53, 0x48, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x9e, 0x67, 0x7a, 0xc3, 0x01, 0x00, 0x0f, 0x75, // VSH......gz....u
	0x5f, 0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x56, 0x69, 0x65, 0x77, 0x50, 0x72, 0x6f, 0x6a, 0x04, 0x01, // _modelViewProj..
	0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9b, 0x02, 0x00, 0x00, 0x61, 0x74, 0x74, 0x72, // ............attr
	0x69, 0x62, 0x75, 0x74, 0x65, 0x20, 0x68, 0x69, 0x67, 0x68, 0x70, 0x20, 0x76, 0x65, 0x63, 0x34, // ibute highp vec4
	0x20, 0x61, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x30, 0x3b, 0x0a, 0x61, 0x74, 0x74, 0x72, 0x69, //  a_color0;.attri
	0x62, 0x75, 0x74, 0x65, 0x20, 0x68, 0x69, 0x67, 0x68, 0x70, 0x20, 0x76, 0x65, 0x63, 0x34, 0x20, // bute highp vec4 
	0x61, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x31, 0x3b, 0x0a, 0x61, 0x74, 0x74, 0x72, 0x69, 0x62, // a_color1;.attrib
	0x75, 0x74, 0x65, 0x20, 0x68, 0x69, 0x67, 0x68, 0x70, 0x20, 0x76, 0x65, 0x63, 0x32, 0x20, 0x61, // ute highp vec2 a
	0x5f, 0x70, 0x6f, 0x73, 0x69, 0x74, 0x69, 0x6f, 0x6e, 0x3b, 0x0a, 0x61, 0x74, 0x74, 0x72, 0x69, // _position;.attri
	0x62, 0x75, 0x74, 0x65, 0x20, 0x68, 0x69, 0x67, 0x68, 0x70, 0x20, 0x76, 0x65, 0x63, 0x34, 0x20, // bute highp vec4 
	0x61, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x30, 0x3b, 0x0a, 0x61, 0x74, 0x74, // a_texcoord0;.att
	0x72, 0x69, 0x62, 0x75, 0x74, 0x65, 0x20, 0x68, 0x69, 0x67, 0x68, 0x70, 0x20, 0x76, 0x65, 0x63, // ribute highp vec
	0x34, 0x20, 0x61, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x31, 0x3b, 0x0a, 0x61, // 4 a_texcoord1;.a
	0x74, 0x74, 0x72, 0x69, 0x62, 0x75, 0x74, 0x65, 0x20, 0x68, 0x69, 0x67, 0x68, 0x70, 0x20, 0x76, // ttribute highp v
	0x65, 0x63, 0x34, 0x20, 0x61, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x32, 0x3b, // ec4 a_texcoord2;
	0x0a, 0x76, 0x61, 0x72, 0x79, 0x69, 0x6e, 0x67, 0x20, 0x68, 0x69, 0x67, 0x68, 0x70, 0x20, 0x76, // .varying highp v
	0x65, 0x63, 0x34, 0x20, 0x76, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x30, 0x3b, 0x0a, 0x76, 0x61, // ec4 v_color0;.va
	0x72, 0x79, 0x69, 0x6e, 0x67, 0x20, 0x68, 0x69, 0x67, 0x68, 0x70, 0x20, 0x76, 0x65, 0x63, 0x34, // rying highp vec4
	0x20, 0x76, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x31, 0x3b, 0x0a, 0x76, 0x61, 0x72, 0x79, 0x69, //  v_color1;.varyi
	0x6e, 0x67, 0x20, 0x68, 0x69, 0x67, 0x68, 0x70, 0x20, 0x76, 0x65, 0x63, 0x34, 0x20, 0x76, 0x5f, // ng highp vec4 v_
	0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x30, 0x3b, 0x0a, 0x76, 0x61, 0x72, 0x79, 0x69, // texcoord0;.varyi
	0x6e, 0x67, 0x20, 0x68, 0x69, 0x67, 0x68, 0x70, 0x20, 0x76, 0x65, 0x63, 0x34, 0x20, 0x76, 0x5f, // ng highp vec4 v_
	0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x31, 0x3b, 0x0a, 0x76, 0x61, 0x72, 0x79, 0x69, // texcoord1;.varyi
	0x6e, 0x67, 0x20, 0x68, 0x69, 0x67, 0x68, 0x70, 0x20, 0x76, 0x65, 0x63, 0x34, 0x20, 0x76, 0x5f, // ng highp vec4 v_
	0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x32, 0x3b, 0x0a, 0x75, 0x6e, 0x69, 0x66, 0x6f, // texcoord2;.unifo
	0x72, 0x6d, 0x20, 0x68, 0x69, 0x67, 0x68, 0x70, 0x20, 0x6d, 0x61, 0x74, 0x34, 0x20, 0x75, 0x5f, // rm highp mat4 u_
	0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x56, 0x69, 0x65, 0x77, 0x50, 0x72, 0x6f, 0x6a, 0x3b, 0x0a, 0x76, // modelViewProj;.v
	0x6f, 0x69, 0x64, 0x20, 0x6d, 0x61, 0x69, 0x6e, 0x20, 0x28, 0x29, 0x0a, 0x7b, 0x0a, 0x20, 0x20, // oid main ().{.  
	0x68, 0x69, 0x67, 0x68, 0x70, 0x20, 0x76, 0x65, 0x63, 0x34, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, // highp vec4 tmpva
	0x72, 0x5f, 0x31, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, 0x2e, // r_1;.  tmpvar_1.
	0x7a, 0x77, 0x20, 0x3d, 0x20, 0x76, 0x65, 0x63, 0x32, 0x28, 0x30, 0x2e, 0x30, 0x2c, 0x20, 0x31, // zw = vec2(0.0, 1
	0x2e, 0x30, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, 0x2e, // .0);.  tmpvar_1.
	0x78, 0x79, 0x20, 0x3d, 0x20, 0x61, 0x5f, 0x70, 0x6f, 0x73, 0x69, 0x74, 0x69, 0x6f, 0x6e, 0x3b, // xy = a_position;
	0x0a, 0x20, 0x20, 0x67, 0x6c, 0x5f, 0x50, 0x6f, 0x73, 0x69, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x3d, // .  gl_Position =
	0x20, 0x28, 0x75, 0x5f, 0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x56, 0x69, 0x65, 0x77, 0x50, 0x72, 0x6f, //  (u_modelViewPro
	0x6a, 0x20, 0x2a, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, 0x29, 0x3b, 0x0a, 0x20, // j * tmpvar_1);. 
	0x20, 0x76, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x30, 0x20, 0x3d, 0x20, 0x61, //  v_texcoord0 = a
	0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x30, 0x3b, 0x0a, 0x20, 0x20, 0x76, 0x5f, // _texcoord0;.  v_
	0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x31, 0x20, 0x3d, 0x20, 0x61, 0x5f, 0x74, 0x65, // texcoord1 = a_te
	0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x31, 0x3b, 0x0a, 0x20, 0x20, 0x76, 0x5f, 0x74, 0x65, 0x78, // xcoord1;.  v_tex
	0x63, 0x6f, 0x6f, 0x72, 0x64, 0x32, 0x20, 0x3d, 0x20, 0x61, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, // coord2 = a_texco
	0x6f, 0x72, 0x64, 0x32, 0x3b, 0x0a, 0x20, 0x20, 0x76, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x30, // ord2;.  v_color0
	0x20, 0x3d, 0x20, 0x61, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x30, 0x3b, 0x0a, 0x20, 0x20, 0x76, //  = a_color0;.  v
	0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x31, 0x20, 0x3d, 0x20, 0x61, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, // _color1 = a_colo
	0x72, 0x31, 0x3b, 0x0a, 0x7d, 0x0a, 0x0a, 0x00,                                                 // r1;.}...
};
static const uint8_t vs_font_distance_field_outline_drop_shadow_image_spv[1700] =
{
	0x56, 0x53, 0x48, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x9e, 0x67, 0x7a, 0xc3, 0x01, 0x00, 0x0f, 0x75, // VSH......gz....u
	0x5f, 0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x56, 0x69, 0x65, 0x77, 0x50, 0x72, 0x6f, 0x6a, 0x04, 0x01, // _modelViewProj..
	0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x06, 0x00, 0x00, 0x03, 0x02, 0x23, 0x07, // ........h.....#.
	0x00, 0x00, 0x01, 0x00, 0x0b, 0x00, 0x08, 0x00, 0x96, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ................
	0x11, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x06, 0x00, 0x01, 0x00, 0x00, 0x00, // ................
	0x47, 0x4c, 0x53, 0x4c, 0x2e, 0x73, 0x74, 0x64, 0x2e, 0x34, 0x35, 0x30, 0x00, 0x00, 0x00, 0x00, // GLSL.std.450....
	0x0e, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x11, 0x00, // ................
	0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x6d, 0x61, 0x69, 0x6e, 0x00, 0x00, 0x00, 0x00, // ........main....
	0x43, 0x00, 0x00, 0x00, 0x46, 0x00, 0x00, 0x00, 0x4a, 0x00, 0x00, 0x00, 0x4d, 0x00, 0x00, 0x00, // C...F...J...M...
	0x50, 0x00, 0x00, 0x00, 0x53, 0x00, 0x00, 0x00, 0x64, 0x00, 0x00, 0x00, 0x67, 0x00, 0x00, 0x00, // P...S...d...g...
	0x6a, 0x00, 0x00, 0x00, 0x6d, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x73, 0x00, 0x00, 0x00, // j...m...p...s...
	0x03, 0x00, 0x03, 0x00, 0x05, 0x00, 0x00, 0x00, 0xf4, 0x01, 0x00, 0x00, 0x05, 0x00, 0x04, 0x00, // ................
	0x04, 0x00, 0x00, 0x00, 0x6d, 0x61, 0x69, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x06, 0x00, // ....main........
	0x2c, 0x00, 0x00, 0x00, 0x55, 0x6e, 0x69, 0x66, 0x6f, 0x72, 0x6d, 0x42, 0x6c, 0x6f, 0x63, 0x6b, // ,...UniformBlock
	0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x07, 0x00, 0x2c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........,.......
	0x75, 0x5f, 0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x56, 0x69, 0x65, 0x77, 0x50, 0x72, 0x6f, 0x6a, 0x00, // u_modelViewProj.
	0x05, 0x00, 0x03, 0x00, 0x2e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x05, 0x00, // ................
	0x43, 0x00, 0x00, 0x00, 0x61, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x30, 0x00, 0x00, 0x00, 0x00, // C...a_color0....
	0x05, 0x00, 0x05, 0x00, 0x46, 0x00, 0x00, 0x00, 0x61, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x31, // ....F...a_color1
	0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x05, 0x00, 0x4a, 0x00, 0x00, 0x00, 0x61, 0x5f, 0x70, 0x6f, // ........J...a_po
	0x73, 0x69, 0x74, 0x69, 0x6f, 0x6e, 0x00, 0x00, 0x05, 0x00, 0x05, 0x00, 0x4d, 0x00, 0x00, 0x00, // sition......M...
	0x61, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x30, 0x00, 0x05, 0x00, 0x05, 0x00, // a_texcoord0.....
	0x50, 0x00, 0x00, 0x00, 0x61, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x31, 0x00, // P...a_texcoord1.
	0x05, 0x00, 0x05, 0x00, 0x53, 0x00, 0x00, 0x00, 0x61, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, // ....S...a_texcoo
	0x72, 0x64, 0x32, 0x00, 0x05, 0x00, 0x0a, 0x00, 0x64, 0x00, 0x00, 0x00, 0x40, 0x65, 0x6e, 0x74, // rd2.....d...@ent
	0x72, 0x79, 0x50, 0x6f, 0x69, 0x6e, 0x74, 0x4f, 0x75, 0x74, 0x70, 0x75, 0x74, 0x2e, 0x67, 0x6c, // ryPointOutput.gl
	0x5f, 0x50, 0x6f, 0x73, 0x69, 0x74, 0x69, 0x6f, 0x6e, 0x00, 0x00, 0x00, 0x05, 0x00, 0x09, 0x00, // _Position.......
	0x67, 0x00, 0x00, 0x00, 0x40, 0x65, 0x6e, 0x74, 0x72, 0x79, 0x50, 0x6f, 0x69, 0x6e, 0x74, 0x4f, // g...@entryPointO
	0x75, 0x74, 0x70, 0x75, 0x74, 0x2e, 0x76, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x30, 0x00, 0x00, // utput.v_color0..
	0x05, 0x00, 0x09, 0x00, 0x6a, 0x00, 0x00, 0x00, 0x40, 0x65, 0x6e, 0x74, 0x72, 0x79, 0x50, 0x6f, // ....j...@entryPo
	0x69, 0x6e, 0x74, 0x4f, 0x75, 0x74, 0x70, 0x75, 0x74, 0x2e, 0x76, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, // intOutput.v_colo
	0x72, 0x31, 0x00, 0x00, 0x05, 0x00, 0x0a, 0x00, 0x6d, 0x00, 0x00, 0x00, 0x40, 0x65, 0x6e, 0x74, // r1......m...@ent
	0x72, 0x79, 0x50, 0x6f, 0x69, 0x6e, 0x74, 0x4f, 0x75, 0x74, 0x70, 0x75, 0x74, 0x2e, 0x76, 0x5f, // ryPointOutput.v_
	0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x30, 0x00, 0x00, 0x00, 0x05, 0x00, 0x0a, 0x00, // texcoord0.......
	0x70, 0x00, 0x00, 0x00, 0x40, 0x65, 0x6e, 0x74, 0x72, 0x79, 0x50, 0x6f, 0x69, 0x6e, 0x74, 0x4f, // p...@entryPointO
	0x75, 0x74, 0x70, 0x75, 0x74, 0x2e, 0x76, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, // utput.v_texcoord
	0x31, 0x00, 0x00, 0x00, 0x05, 0x00, 0x0a, 0x00, 0x73, 0x00, 0x00, 0x00, 0x40, 0x65, 0x6e, 0x74, // 1.......s...@ent
	0x72, 0x79, 0x50, 0x6f, 0x69, 0x6e, 0x74, 0x4f, 0x75, 0x74, 0x70, 0x75, 0x74, 0x2e, 0x76, 0x5f, // ryPointOutput.v_
	0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x32, 0x00, 0x00, 0x00, 0x48, 0x00, 0x04, 0x00, // texcoord2...H...
	0x2c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x48, 0x00, 0x05, 0x00, // ,...........H...
	0x2c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ,.......#.......
	0x48, 0x00, 0x05, 0x00, 0x2c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, // H...,...........
	0x10, 0x00, 0x00, 0x00, 0x47, 0x00, 0x03, 0x00, 0x2c, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, // ....G...,.......
	0x47, 0x00, 0x04, 0x00, 0x2e, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // G.......".......
	0x47, 0x00, 0x04, 0x00, 0x2e, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // G.......!.......
	0x47, 0x00, 0x04, 0x00, 0x43, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // G...C...........
	0x47, 0x00, 0x04, 0x00, 0x46, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // G...F...........
	0x47, 0x00, 0x04, 0x00, 0x4a, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, // G...J...........
	0x47, 0x00, 0x04, 0x00, 0x4d, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, // G...M...........
	0x47, 0x00, 0x04, 0x00, 0x50, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, // G...P...........
	0x47, 0x00, 0x04, 0x00, 0x53, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, // G...S...........
	0x47, 0x00, 0x04, 0x00, 0x64, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // G...d...........
	0x47, 0x00, 0x04, 0x00, 0x67, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // G...g...........
	0x47, 0x00, 0x04, 0x00, 0x6a, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // G...j...........
	0x47, 0x00, 0x04, 0x00, 0x6d, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, // G...m...........
	0x47, 0x00, 0x04, 0x00, 0x70, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, // G...p...........
	0x47, 0x00, 0x04, 0x00, 0x73, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, // G...s...........
	0x13, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x21, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00, // ........!.......
	0x02, 0x00, 0x00, 0x00, 0x16, 0x00, 0x03, 0x00, 0x06, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, // ............ ...
	0x17, 0x00, 0x04, 0x00, 0x07, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, // ................
	0x17, 0x00, 0x04, 0x00, 0x09, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, // ................
	0x15, 0x00, 0x04, 0x00, 0x17, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // ........ .......
	0x2b, 0x00, 0x04, 0x00, 0x06, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, // +..............?
	0x2b, 0x00, 0x04, 0x00, 0x06, 0x00, 0x00, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // +...............
	0x2b, 0x00, 0x04, 0x00, 0x17, 0x00, 0x00, 0x00, 0x26, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // +.......&.......
	0x18, 0x00, 0x04, 0x00, 0x2b, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, // ....+...........
	0x1e, 0x00, 0x03, 0x00, 0x2c, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, // ....,...+... ...
	0x2d, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x2c, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, // -.......,...;...
	0x2d, 0x00, 0x00, 0x00, 0x2e, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, // -........... ...
	0x2f, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, // /.......+... ...
	0x42, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, // B...........;...
	0x42, 0x00, 0x00, 0x00, 0x43, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, // B...C.......;...
	0x42, 0x00, 0x00, 0x00, 0x46, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, // B...F....... ...
	0x49, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, // I...........;...
	0x49, 0x00, 0x00, 0x00, 0x4a, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, // I...J.......;...
	0x42, 0x00, 0x00, 0x00, 0x4d, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, // B...M.......;...
	0x42, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, // B...P.......;...
	0x42, 0x00, 0x00, 0x00, 0x53, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, // B...S....... ...
	0x63, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, // c...........;...
	0x63, 0x00, 0x00, 0x00, 0x64, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, // c...d.......;...
	0x63, 0x00, 0x00, 0x00, 0x67, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, // c...g.......;...
	0x63, 0x00, 0x00, 0x00, 0x6a, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, // c...j.......;...
	0x63, 0x00, 0x00, 0x00, 0x6d, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, // c...m.......;...
	0x63, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, // c...p.......;...
	0x63, 0x00, 0x00, 0x00, 0x73, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x36, 0x00, 0x05, 0x00, // c...s.......6...
	0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, // ................
	0xf8, 0x00, 0x02, 0x00, 0x05, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x07, 0x00, 0x00, 0x00, // ........=.......
	0x44, 0x00, 0x00, 0x00, 0x43, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x07, 0x00, 0x00, 0x00, // D...C...=.......
	0x47, 0x00, 0x00, 0x00, 0x46, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x09, 0x00, 0x00, 0x00, // G...F...=.......
	0x4b, 0x00, 0x00, 0x00, 0x4a, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x07, 0x00, 0x00, 0x00, // K...J...=.......
	0x4e, 0x00, 0x00, 0x00, 0x4d, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x07, 0x00, 0x00, 0x00, // N...M...=.......
	0x51, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x07, 0x00, 0x00, 0x00, // Q...P...=.......
	0x54, 0x00, 0x00, 0x00, 0x53, 0x00, 0x00, 0x00, 0x51, 0x00, 0x05, 0x00, 0x06, 0x00, 0x00, 0x00, // T...S...Q.......
	0x90, 0x00, 0x00, 0x00, 0x4b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x51, 0x00, 0x05, 0x00, // ....K.......Q...
	0x06, 0x00, 0x00, 0x00, 0x91, 0x00, 0x00, 0x00, 0x4b, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // ........K.......
	0x50, 0x00, 0x07, 0x00, 0x07, 0x00, 0x00, 0x00, 0x92, 0x00, 0x00, 0x00, 0x90, 0x00, 0x00, 0x00, // P...............
	0x91, 0x00, 0x00, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00, 0x41, 0x00, 0x05, 0x00, // ............A...
	0x2f, 0x00, 0x00, 0x00, 0x93, 0x00, 0x00, 0x00, 0x2e, 0x00, 0x00, 0x00, 0x26, 0x00, 0x00, 0x00, // /...........&...
	0x3d, 0x00, 0x04, 0x00, 0x2b, 0x00, 0x00, 0x00, 0x94, 0x00, 0x00, 0x00, 0x93, 0x00, 0x00, 0x00, // =...+...........
	0x90, 0x00, 0x05, 0x00, 0x07, 0x00, 0x00, 0x00, 0x95, 0x00, 0x00, 0x00, 0x92, 0x00, 0x00, 0x00, // ................
	0x94, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x03, 0x00, 0x64, 0x00, 0x00, 0x00, 0x95, 0x00, 0x00, 0x00, // ....>...d.......
	0x3e, 0x00, 0x03, 0x00, 0x67, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x03, 0x00, // >...g...D...>...
	0x6a, 0x00, 0x00, 0x00, 0x47, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x03, 0x00, 0x6d, 0x00, 0x00, 0x00, // j...G...>...m...
	0x4e, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x03, 0x00, 0x70, 0x00, 0x00, 0x00, 0x51, 0x00, 0x00, 0x00, // N...>...p...Q...
	0x3e, 0x00, 0x03, 0x00, 0x73, 0x00, 0x00, 0x00, 0x54, 0x00, 0x00, 0x00, 0xfd, 0x00, 0x01, 0x00, // >...s...T.......
	0x38, 0x00, 0x01, 0x00, 0x00, 0x06, 0x05, 0x00, 0x06, 0x00, 0x01, 0x00, 0x10, 0x00, 0x11, 0x00, // 8...............
	0x12, 0x00, 0x40, 0x00,                                                                         // ..@.
};
static const uint8_t vs_font_distance_field_outline_drop_shadow_image_dx11[876] =
{
	0x56, 0x53, 0x48, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x9e, 0x67, 0x7a, 0xc3, 0x01, 0x00, 0x0f, 0x75, // VSH......gz....u
	0x5f, 0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x56, 0x69, 0x65, 0x77, 0x50, 0x72, 0x6f, 0x6a, 0x04, 0x00, // _modelViewProj..
	0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x03, 0x00, 0x00, 0x44, 0x58, 0x42, 0x43, // ........0...DXBC
	0xae, 0x48, 0x4a, 0xd1, 0x28, 0x9e, 0x06, 0xfe, 0x68, 0xe6, 0x57, 0xe9, 0x22, 0xe0, 0x0e, 0xe3, // .HJ.(...h.W."...
	0x01, 0x00, 0x00, 0x00, 0x30, 0x03, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x2c, 0x00, 0x00, 0x00, // ....0.......,...
	0xe4, 0x00, 0x00, 0x00, 0xa0, 0x01, 0x00, 0x00, 0x49, 0x53, 0x47, 0x4e, 0xb0, 0x00, 0x00, 0x00, // ........ISGN....
	0x06, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x98, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ................
	0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x00, 0x00, // ................
	0x98, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, // ................
	0x01, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x00, 0x00, 0x9e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ................
	0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x00, // ................
	0xa7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, // ................
	0x03, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x00, 0x00, 0xa7, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // ................
	0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x00, 0x00, // ................
	0xa7, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, // ................
	0x05, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x00, 0x00, 0x43, 0x4f, 0x4c, 0x4f, 0x52, 0x00, 0x50, 0x4f, // ........COLOR.PO
	0x53, 0x49, 0x54, 0x49, 0x4f, 0x4e, 0x00, 0x54, 0x45, 0x58, 0x43, 0x4f, 0x4f, 0x52, 0x44, 0x00, // SITION.TEXCOORD.
	0x4f, 0x53, 0x47, 0x4e, 0xb4, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, // OSGN............
	0x98, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, // ................
	0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0xa4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ................
	0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, // ................
	0xa4, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, // ................
	0x02, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0xaa, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ................
	0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, // ................
	0xaa, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, // ................
	0x04, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0xaa, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, // ................
	0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, // ................
	0x53, 0x56, 0x5f, 0x50, 0x4f, 0x53, 0x49, 0x54, 0x49, 0x4f, 0x4e, 0x00, 0x43, 0x4f, 0x4c, 0x4f, // SV_POSITION.COLO
	0x52, 0x00, 0x54, 0x45, 0x58, 0x43, 0x4f, 0x4f, 0x52, 0x44, 0x00, 0xab, 0x53, 0x48, 0x45, 0x58, // R.TEXCOORD..SHEX
	0x88, 0x01, 0x00, 0x00, 0x50, 0x00, 0x01, 0x00, 0x62, 0x00, 0x00, 0x00, 0x6a, 0x08, 0x00, 0x01, // ....P...b...j...
	0x59, 0x00, 0x00, 0x04, 0x46, 0x8e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, // Y...F. .........
	0x5f, 0x00, 0x00, 0x03, 0xf2, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5f, 0x00, 0x00, 0x03, // _..........._...
	0xf2, 0x10, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x5f, 0x00, 0x00, 0x03, 0x32, 0x10, 0x10, 0x00, // ........_...2...
	0x02, 0x00, 0x00, 0x00, 0x5f, 0x00, 0x00, 0x03, 0xf2, 0x10, 0x10, 0x00, 0x03, 0x00, 0x00, 0x00, // ...._...........
	0x5f, 0x00, 0x00, 0x03, 0xf2, 0x10, 0x10, 0x00, 0x04, 0x00, 0x00, 0x00, 0x5f, 0x00, 0x00, 0x03, // _..........._...
	0xf2, 0x10, 0x10, 0x00, 0x05, 0x00, 0x00, 0x00, 0x67, 0x00, 0x00, 0x04, 0xf2, 0x20, 0x10, 0x00, // ........g.... ..
	0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x65, 0x00, 0x00, 0x03, 0xf2, 0x20, 0x10, 0x00, // ........e.... ..
	0x01, 0x00, 0x00, 0x00, 0x65, 0x00, 0x00, 0x03, 0xf2, 0x20, 0x10, 0x00, 0x02, 0x00, 0x00, 0x00, // ....e.... ......
	0x65, 0x00, 0x00, 0x03, 0xf2, 0x20, 0x10, 0x00, 0x03, 0x00, 0x00, 0x00, 0x65, 0x00, 0x00, 0x03, // e.... ......e...
	0xf2, 0x20, 0x10, 0x00, 0x04, 0x00, 0x00, 0x00, 0x65, 0x00, 0x00, 0x03, 0xf2, 0x20, 0x10, 0x00, // . ......e.... ..
	0x05, 0x00, 0x00, 0x00, 0x68, 0x00, 0x00, 0x02, 0x01, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x08, // ....h.......8...
	0xf2, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x56, 0x15, 0x10, 0x00, 0x02, 0x00, 0x00, 0x00, // ........V.......
	0x46, 0x8e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x0a, // F. .........2...
	0xf2, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x8e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, // ........F. .....
	0x00, 0x00, 0x00, 0x00, 0x06, 0x10, 0x10, 0x00, 0x02, 0x00, 0x00, 0x00, 0x46, 0x0e, 0x10, 0x00, // ............F...
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xf2, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, // ......... ......
	0x46, 0x0e, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x8e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, // F.......F. .....
	0x03, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x05, 0xf2, 0x20, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, // ....6.... ......
	0x46, 0x1e, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x05, 0xf2, 0x20, 0x10, 0x00, // F.......6.... ..
	0x02, 0x00, 0x00, 0x00, 0x46, 0x1e, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x05, // ....F.......6...
	0xf2, 0x20, 0x10, 0x00, 0x03, 0x00, 0x00, 0x00, 0x46, 0x1e, 0x10, 0x00, 0x03, 0x00, 0x00, 0x00, // . ......F.......
	0x36, 0x00, 0x00, 0x05, 0xf2, 0x20, 0x10, 0x00, 0x04, 0x00, 0x00, 0x00, 0x46, 0x1e, 0x10, 0x00, // 6.... ......F...
	0x04, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x05, 0xf2, 0x20, 0x10, 0x00, 0x05, 0x00, 0x00, 0x00, // ....6.... ......
	0x46, 0x1e, 0x10, 0x00, 0x05, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x01, 0x00, 0x06, 0x05, 0x00, // F.......>.......
	0x06, 0x00, 0x01, 0x00, 0x10, 0x00, 0x11, 0x00, 0x12, 0x00, 0x40, 0x00,                         // ..........@.
};
static const uint8_t vs_font_distance_field_outline_drop_shadow_image_mtl[1354] =
{
	0x56, 0x53, 0x48, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x9e, 0x67, 0x7a, 0xc3, 0x01, 0x00, 0x0f, 0x75, // VSH......gz....u
	0x5f, 0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x56, 0x69, 0x65, 0x77, 0x50, 0x72, 0x6f, 0x6a, 0x04, 0x01, // _modelViewProj..
	0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x05, 0x00, 0x00, 0x23, 0x69, 0x6e, 0x63, // ............#inc
	0x6c, 0x75, 0x64, 0x65, 0x20, 0x3c, 0x6d, 0x65, 0x74, 0x61, 0x6c, 0x5f, 0x73, 0x74, 0x64, 0x6c, // lude <metal_stdl
	0x69, 0x62, 0x3e, 0x0a, 0x23, 0x69, 0x6e, 0x63, 0x6c, 0x75, 0x64, 0x65, 0x20, 0x3c, 0x73, 0x69, // ib>.#include <si
	0x6d, 0x64, 0x2f, 0x73, 0x69, 0x6d, 0x64, 0x2e, 0x68, 0x3e, 0x0a, 0x0a, 0x75, 0x73, 0x69, 0x6e, // md/simd.h>..usin
	0x67, 0x20, 0x6e, 0x61, 0x6d, 0x65, 0x73, 0x70, 0x61, 0x63, 0x65, 0x20, 0x6d, 0x65, 0x74, 0x61, // g namespace meta
	0x6c, 0x3b, 0x0a, 0x0a, 0x73, 0x74, 0x72, 0x75, 0x63, 0x74, 0x20, 0x5f, 0x47, 0x6c, 0x6f, 0x62, // l;..struct _Glob
	0x61, 0x6c, 0x0a, 0x7b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74, 0x34, 0x78, // al.{.    float4x
	0x34, 0x20, 0x75, 0x5f, 0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x56, 0x69, 0x65, 0x77, 0x50, 0x72, 0x6f, // 4 u_modelViewPro
	0x6a, 0x3b, 0x0a, 0x7d, 0x3b, 0x0a, 0x0a, 0x73, 0x74, 0x72, 0x75, 0x63, 0x74, 0x20, 0x78, 0x6c, // j;.};..struct xl
	0x61, 0x74, 0x4d, 0x74, 0x6c, 0x4d, 0x61, 0x69, 0x6e, 0x5f, 0x6f, 0x75, 0x74, 0x0a, 0x7b, 0x0a, // atMtlMain_out.{.
	0x09, 0x66, 0x6c, 0x6f, 0x61, 0x74, 0x20, 0x62, 0x67, 0x66, 0x78, 0x5f, 0x6d, 0x65, 0x74, 0x61, // .float bgfx_meta
	0x6c, 0x5f, 0x70, 0x6f, 0x69, 0x6e, 0x74, 0x53, 0x69, 0x7a, 0x65, 0x20, 0x5b, 0x5b, 0x70, 0x6f, // l_pointSize [[po
	0x69, 0x6e, 0x74, 0x5f, 0x73, 0x69, 0x7a, 0x65, 0x5d, 0x5d, 0x20, 0x3d, 0x20, 0x31, 0x3b, 0x0a, // int_size]] = 1;.
	0x20, 0x20, 0x20, 0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74, 0x34, 0x20, 0x5f, 0x65, 0x6e, 0x74, 0x72, //     float4 _entr
	0x79, 0x50, 0x6f, 0x69, 0x6e, 0x74, 0x4f, 0x75, 0x74, 0x70, 0x75, 0x74, 0x5f, 0x76, 0x5f, 0x63, // yPointOutput_v_c
	0x6f, 0x6c, 0x6f, 0x72, 0x30, 0x20, 0x5b, 0x5b, 0x75, 0x73, 0x65, 0x72, 0x28, 0x6c, 0x6f, 0x63, // olor0 [[user(loc
	0x6e, 0x30, 0x29, 0x5d, 0x5d, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74, // n0)]];.    float
	0x34, 0x20, 0x5f, 0x65, 0x6e, 0x74, 0x72, 0x79, 0x50, 0x6f, 0x69, 0x6e, 0x74, 0x4f, 0x75, 0x74, // 4 _entryPointOut
	0x70, 0x75, 0x74, 0x5f, 0x76, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x31, 0x20, 0x5b, 0x5b, 0x75, // put_v_color1 [[u
	0x73, 0x65, 0x72, 0x28, 0x6c, 0x6f, 0x63, 0x6e, 0x31, 0x29, 0x5d, 0x5d, 0x3b, 0x0a, 0x20, 0x20, // ser(locn1)]];.  
	0x20, 0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74, 0x34, 0x20, 0x5f, 0x65, 0x6e, 0x74, 0x72, 0x79, 0x50, //   float4 _entryP
	0x6f, 0x69, 0x6e, 0x74, 0x4f, 0x75, 0x74, 0x70, 0x75, 0x74, 0x5f, 0x76, 0x5f, 0x74, 0x65, 0x78, // ointOutput_v_tex
	0x63, 0x6f, 0x6f, 0x72, 0x64, 0x30, 0x20, 0x5b, 0x5b, 0x75, 0x73, 0x65, 0x72, 0x28, 0x6c, 0x6f, // coord0 [[user(lo
	0x63, 0x6e, 0x32, 0x29, 0x5d, 0x5d, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x66, 0x6c, 0x6f, 0x61, // cn2)]];.    floa
	0x74, 0x34, 0x20, 0x5f, 0x65, 0x6e, 0x74, 0x72, 0x79, 0x50, 0x6f, 0x69, 0x6e, 0x74, 0x4f, 0x75, // t4 _entryPointOu
	0x74, 0x70, 0x75, 0x74, 0x5f, 0x76, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x31, // tput_v_texcoord1
	0x20, 0x5b, 0x5b, 0x75, 0x73, 0x65, 0x72, 0x28, 0x6c, 0x6f, 0x63, 0x6e, 0x33, 0x29, 0x5d, 0x5d, //  [[user(locn3)]]
	0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74, 0x34, 0x20, 0x5f, 0x65, 0x6e, // ;.    float4 _en
	0x74, 0x72, 0x79, 0x50, 0x6f, 0x69, 0x6e, 0x74, 0x4f, 0x75, 0x74, 0x70, 0x75, 0x74, 0x5f, 0x76, // tryPointOutput_v
	0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x32, 0x20, 0x5b, 0x5b, 0x75, 0x73, 0x65, // _texcoord2 [[use
	0x72, 0x28, 0x6c, 0x6f, 0x63, 0x6e, 0x34, 0x29, 0x5d, 0x5d, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, // r(locn4)]];.    
	0x66, 0x6c, 0x6f, 0x61, 0x74, 0x34, 0x20, 0x67, 0x6c, 0x5f, 0x50, 0x6f, 0x73, 0x69, 0x74, 0x69, // float4 gl_Positi
	0x6f, 0x6e, 0x20, 0x5b, 0x5b, 0x70, 0x6f, 0x73, 0x69, 0x74, 0x69, 0x6f, 0x6e, 0x5d, 0x5d, 0x3b, // on [[position]];
	0x0a, 0x7d, 0x3b, 0x0a, 0x0a, 0x73, 0x74, 0x72, 0x75, 0x63, 0x74, 0x20, 0x78, 0x6c, 0x61, 0x74, // .};..struct xlat
	0x4d, 0x74, 0x6c, 0x4d, 0x61, 0x69, 0x6e, 0x5f, 0x69, 0x6e, 0x0a, 0x7b, 0x0a, 0x20, 0x20, 0x20, // MtlMain_in.{.   
	0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74, 0x34, 0x20, 0x61, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x30, //  float4 a_color0
	0x20, 0x5b, 0x5b, 0x61, 0x74, 0x74, 0x72, 0x69, 0x62, 0x75, 0x74, 0x65, 0x28, 0x30, 0x29, 0x5d, //  [[attribute(0)]
	0x5d, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74, 0x34, 0x20, 0x61, 0x5f, // ];.    float4 a_
	0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x31, 0x20, 0x5b, 0x5b, 0x61, 0x74, 0x74, 0x72, 0x69, 0x62, 0x75, // color1 [[attribu
	0x74, 0x65, 0x28, 0x31, 0x29, 0x5d, 0x5d, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x66, 0x6c, 0x6f, // te(1)]];.    flo
	0x61, 0x74, 0x32, 0x20, 0x61, 0x5f, 0x70, 0x6f, 0x73, 0x69, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x5b, // at2 a_position [
	0x5b, 0x61, 0x74, 0x74, 0x72, 0x69, 0x62, 0x75, 0x74, 0x65, 0x28, 0x32, 0x29, 0x5d, 0x5d, 0x3b, // [attribute(2)]];
	0x0a, 0x20, 0x20, 0x20, 0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74, 0x34, 0x20, 0x61, 0x5f, 0x74, 0x65, // .    float4 a_te
	0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x30, 0x20, 0x5b, 0x5b, 0x61, 0x74, 0x74, 0x72, 0x69, 0x62, // xcoord0 [[attrib
	0x75, 0x74, 0x65, 0x28, 0x33, 0x29, 0x5d, 0x5d, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x66, 0x6c, // ute(3)]];.    fl
	0x6f, 0x61, 0x74, 0x34, 0x20, 0x61, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x31, // oat4 a_texcoord1
	0x20, 0x5b, 0x5b, 0x61, 0x74, 0x74, 0x72, 0x69, 0x62, 0x75, 0x74, 0x65, 0x28, 0x34, 0x29, 0x5d, //  [[attribute(4)]
	0x5d, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74, 0x34, 0x20, 0x61, 0x5f, // ];.    float4 a_
	0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x32, 0x20, 0x5b, 0x5b, 0x61, 0x74, 0x74, 0x72, // texcoord2 [[attr
	0x69, 0x62, 0x75, 0x74, 0x65, 0x28, 0x35, 0x29, 0x5d, 0x5d, 0x3b, 0x0a, 0x7d, 0x3b, 0x0a, 0x0a, // ibute(5)]];.};..
	0x76, 0x65, 0x72, 0x74, 0x65, 0x78, 0x20, 0x78, 0x6c, 0x61, 0x74, 0x4d, 0x74, 0x6c, 0x4d, 0x61, // vertex xlatMtlMa
	0x69, 0x6e, 0x5f, 0x6f, 0x75, 0x74, 0x20, 0x78, 0x6c, 0x61, 0x74, 0x4d, 0x74, 0x6c, 0x4d, 0x61, // in_out xlatMtlMa
	0x69, 0x6e, 0x28, 0x78, 0x6c, 0x61, 0x74, 0x4d, 0x74, 0x6c, 0x4d, 0x61, 0x69, 0x6e, 0x5f, 0x69, // in(xlatMtlMain_i
	0x6e, 0x20, 0x69, 0x6e, 0x20, 0x5b, 0x5b, 0x73, 0x74, 0x61, 0x67, 0x65, 0x5f, 0x69, 0x6e, 0x5d, // n in [[stage_in]
	0x5d, 0x2c, 0x20, 0x63, 0x6f, 0x6e, 0x73, 0x74, 0x61, 0x6e, 0x74, 0x20, 0x5f, 0x47, 0x6c, 0x6f, // ], constant _Glo
	0x62, 0x61, 0x6c, 0x26, 0x20, 0x5f, 0x6d, 0x74, 0x6c, 0x5f, 0x75, 0x20, 0x5b, 0x5b, 0x62, 0x75, // bal& _mtl_u [[bu
	0x66, 0x66, 0x65, 0x72, 0x28, 0x30, 0x29, 0x5d, 0x5d, 0x29, 0x0a, 0x7b, 0x0a, 0x20, 0x20, 0x20, // ffer(0)]]).{.   
	0x20, 0x78, 0x6c, 0x61, 0x74, 0x4d, 0x74, 0x6c, 0x4d, 0x61, 0x69, 0x6e, 0x5f, 0x6f, 0x75, 0x74, //  xlatMtlMain_out
	0x20, 0x6f, 0x75, 0x74, 0x20, 0x3d, 0x20, 0x7b, 0x7d, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x6f, //  out = {};.    o
	0x75, 0x74, 0x2e, 0x67, 0x6c, 0x5f, 0x50, 0x6f, 0x73, 0x69, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x3d, // ut.gl_Position =
	0x20, 0x5f, 0x6d, 0x74, 0x6c, 0x5f, 0x75, 0x2e, 0x75, 0x5f, 0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x56, //  _mtl_u.u_modelV
	0x69, 0x65, 0x77, 0x50, 0x72, 0x6f, 0x6a, 0x20, 0x2a, 0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74, 0x34, // iewProj * float4
	0x28, 0x69, 0x6e, 0x2e, 0x61, 0x5f, 0x70, 0x6f, 0x73, 0x69, 0x74, 0x69, 0x6f, 0x6e, 0x2c, 0x20, // (in.a_position, 
	0x30, 0x2e, 0x30, 0x2c, 0x20, 0x31, 0x2e, 0x30, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x6f, // 0.0, 1.0);.    o
	0x75, 0x74, 0x2e, 0x5f, 0x65, 0x6e, 0x74, 0x72, 0x79, 0x50, 0x6f, 0x69, 0x6e, 0x74, 0x4f, 0x75, // ut._entryPointOu
	0x74, 0x70, 0x75, 0x74, 0x5f, 0x76, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x30, 0x20, 0x3d, 0x20, // tput_v_color0 = 
	0x69, 0x6e, 0x2e, 0x61, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x30, 0x3b, 0x0a, 0x20, 0x20, 0x20, // in.a_color0;.   
	0x20, 0x6f, 0x75, 0x74, 0x2e, 0x5f, 0x65, 0x6e, 0x74, 0x72, 0x79, 0x50, 0x6f, 0x69, 0x6e, 0x74, //  out._entryPoint
	0x4f, 0x75, 0x74, 0x70, 0x75, 0x74, 0x5f, 0x76, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x31, 0x20, // Output_v_color1 
	0x3d, 0x20, 0x69, 0x6e, 0x2e, 0x61, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x31, 0x3b, 0x0a, 0x20, // = in.a_color1;. 
	0x20, 0x20, 0x20, 0x6f, 0x75, 0x74, 0x2e, 0x5f, 0x65, 0x6e, 0x74, 0x72, 0x79, 0x50, 0x6f, 0x69, //    out._entryPoi
	0x6e, 0x74, 0x4f, 0x75, 0x74, 0x70, 0x75, 0x74, 0x5f, 0x76, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, // ntOutput_v_texco
	0x6f, 0x72, 0x64, 0x30, 0x20, 0x3d, 0x20, 0x69, 0x6e, 0x2e, 0x61, 0x5f, 0x74, 0x65, 0x78, 0x63, // ord0 = in.a_texc
	0x6f, 0x6f, 0x72, 0x64, 0x30, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x6f, 0x75, 0x74, 0x2e, 0x5f, // oord0;.    out._
	0x65, 0x6e, 0x74, 0x72, 0x79, 0x50, 0x6f, 0x69, 0x6e, 0x74, 0x4f, 0x75, 0x74, 0x70, 0x75, 0x74, // entryPointOutput
	0x5f, 0x76, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x31, 0x20, 0x3d, 0x20, 0x69, // _v_texcoord1 = i
	0x6e, 0x2e, 0x61, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x31, 0x3b, 0x0a, 0x20, // n.a_texcoord1;. 
	0x20, 0x20, 0x20, 0x6f, 0x75, 0x74, 0x2e, 0x5f, 0x65, 0x6e, 0x74, 0x72, 0x79, 0x50, 0x6f, 0x69, //    out._entryPoi
	0x6e, 0x74, 0x4f, 0x75, 0x74, 0x70, 0x75, 0x74, 0x5f, 0x76, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, // ntOutput_v_texco
	0x6f, 0x72, 0x64, 0x32, 0x20, 0x3d, 0x20, 0x69, 0x6e, 0x2e, 0x61, 0x5f, 0x74, 0x65, 0x78, 0x63, // ord2 = in.a_texc
	0x6f, 0x6f, 0x72, 0x64, 0x32, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x72, 0x65, 0x74, 0x75, 0x72, // oord2;.    retur
	0x6e, 0x20, 0x6f, 0x75, 0x74, 0x3b, 0x0a, 0x7d, 0x0a, 0x0a, 0x00, 0x06, 0x05, 0x00, 0x06, 0x00, // n out;.}........
	0x01, 0x00, 0x10, 0x00, 0x11, 0x00, 0x12, 0x00, 0x40, 0x00,                                     // ........@.
};
extern const uint8_t* vs_font_distance_field_outline_drop_shadow_image_pssl;
extern const uint32_t vs_font_distance_field_outline_drop_shadow_image_pssl_size;