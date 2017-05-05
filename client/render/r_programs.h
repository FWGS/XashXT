/*
r_programs.h - compiled CG programs
Copyright (C) 2011 Uncle Mike

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/
#ifndef R_PROGRAMS_H
#define R_PROGRAMS_H

const char fp_screen_source[] = 
"!!ARBfp1.0\n"
"OPTION ARB_precision_hint_fastest;\n"
"PARAM c0 = {0.32000000, 0.59000000, 0.090000000, 0};\n"
"TEMP R0;\n"
"TXP R0, fragment.texcoord[0], texture[0], 2D;\n"
"DP3 result.color, c0, R0;\n"
"MOV result.color.w, fragment.color.w;\n"
"END";

const char fp_shadow_source[] =
"!!ARBfp1.0"
"OPTION ARB_fragment_program_shadow;"
"OPTION ARB_precision_hint_fastest;"
"PARAM c[5] = {"
"{0, -0.001953125},"
"{-0.001953125, 0},"
"{0.001953125, 0},"
"{0, 0.001953125},"
"{5, 1}};"
"TEMP R0;"
"TEMP R1;"
"RCP R0.x, fragment.texcoord[2].w;"
"MUL R0.xyz, fragment.texcoord[2], R0.x;"
"TEX R0.w, R0, texture[2], SHADOW2D;"
"ADD R1.xyz, R0, c[0];"
"TEX R1.w, R1, texture[2], SHADOW2D;"
"ADD R0.w, R1.w, R0.w;"
"ADD R1.xyz, R0, c[1];"
"TEX R1.w, R1, texture[2], SHADOW2D;"
"ADD R0.w, R1.w, R0.w;"
"ADD R1.xyz, R0, c[2];"
"TEX R1.w, R1, texture[2], SHADOW2D;"
"ADD R0.w, R1.w, R0.w;"
"ADD R1.xyz, R0, c[3];"
"TEX R1.w, R1, texture[2], SHADOW2D;"
"ADD R0.w, R1.w, R0.w;"
"RCP R1.w, c[4].x;"
"MUL R1.w, R0.w, R1.w;"
"TXP R0, fragment.texcoord[0], texture[0], 2D;"
"MUL R1, R0, R1.w;"
"TXP R0, fragment.texcoord[1], texture[1], 1D;"
"MUL R1, R1, R0;"
"MUL result.color.xyz, fragment.color.primary, R1;"
"MOV result.color.w, c[4].y;"
"END";

const char fp_decal0_source[] =
"!!ARBfp1.0"
"TEMP R0;"
"TEMP R1;"
"TXP R0, fragment.texcoord[0], texture[0], 2D;"
"TXP R1, fragment.texcoord[1], texture[1], 1D;"
"MUL R1, R0, R1;"
"MUL R1, fragment.color.primary, R1;"
"TEX R0, fragment.texcoord[3], texture[3], 2D;"
"MUL R0.xyz, R0, R0.w;"
"MUL result.color.xyz, R0, R1;"
"MOV result.color.w, 1.0;"
"END";

const char fp_decal1_source[] =
"!!ARBfp1.0"
"OPTION ARB_fragment_program_shadow;"
"OPTION ARB_precision_hint_fastest;"
"TEMP R0;"
"TEMP R1;"
"TEMP R3;"
"TEMP R4;"
"RCP R0.x, fragment.texcoord[2].w;"
"MUL R0.xyz, fragment.texcoord[2], R0.x;"
"TEX R1.w, R0, texture[2], SHADOW2D;"
"TXP R0, fragment.texcoord[0], texture[0], 2D;"
"MUL R1, R0, R1.w;"
"TXP R0, fragment.texcoord[1], texture[1], 1D;"
"MUL R1, R1, R0;"
"MUL R3.xyz, fragment.color.primary, R1;"
"TEX R4, fragment.texcoord[3], texture[3], 2D;"
"MUL R4.xyz, R4, R4.w;"
"MUL result.color.xyz, R3, R4;"
"MOV result.color.w, 1.0;"
"END";

const char fp_decal2_source[] =
"!!ARBfp1.0"
"OPTION ARB_fragment_program_shadow;"
"OPTION ARB_precision_hint_fastest;"
"PARAM c[5] = {"
"{0, -0.00390625},"
"{-0.00390625, 0},"
"{0.00390625, 0},"
"{0, 0.00390625},"
"{5, 1}};"
"TEMP R0;"
"TEMP R1;"
"TEMP R3;"
"TEMP R4;"
"RCP R0.x, fragment.texcoord[2].w;"
"MUL R0.xyz, fragment.texcoord[2], R0.x;"
"TEX R0.w, R0, texture[2], SHADOW2D;"
"ADD R1.xyz, R0, c[0];"
"TEX R1.w, R1, texture[2], SHADOW2D;"
"ADD R0.w, R1.w, R0.w;"
"ADD R1.xyz, R0, c[1];"
"TEX R1.w, R1, texture[2], SHADOW2D;"
"ADD R0.w, R1.w, R0.w;"
"ADD R1.xyz, R0, c[2];"
"TEX R1.w, R1, texture[2], SHADOW2D;"
"ADD R0.w, R1.w, R0.w;"
"ADD R1.xyz, R0, c[3];"
"TEX R1.w, R1, texture[2], SHADOW2D;"
"ADD R0.w, R1.w, R0.w;"
"RCP R1.w, c[4].x;"
"MUL R1.w, R0.w, R1.w;"
"TXP R0, fragment.texcoord[0], texture[0], 2D;"
"MUL R1, R0, R1.w;"
"TXP R0, fragment.texcoord[1], texture[1], 1D;"
"MUL R1, R1, R0;"
"MUL R3.xyz, fragment.color.primary, R1;"
"TEX R4, fragment.texcoord[3], texture[3], 2D;"
"MUL R4.xyz, R4, R4.w;"
"MUL result.color.xyz, R3, R4;"
"MOV result.color.w, c[4].y;"
"END";

const char fp_decal3_source[] =
"!!ARBfp1.0"
"TEMP R0;"
"TEMP R1;"
"TXP R0, fragment.texcoord[0], texture[0], CUBE;"
"TXP R1, fragment.texcoord[1], texture[1], 3D;"
"MUL R1, R0, R1;"
"MUL R1, fragment.color.primary, R1;"
"TEX R0, fragment.texcoord[3], texture[3], 2D;"
"MUL R0.xyz, R0, R0.w;"
"MUL result.color.xyz, R0, R1;"
"MOV result.color.w, 1.0;"
"END";

const char fp_liquid_source[ ] =
"!!ARBfp1.0\n"
"TEMP R0;\n"
"TEX R0, fragment.texcoord[1], texture[1], 3D;\n"
"MAD R0, R0, 2.0, fragment.texcoord[0];\n"
"TXP result.color.xyz, R0, texture[0], 2D;\n"
"DP3 R0.w, fragment.texcoord[2], fragment.texcoord[2];\n"
"RSQ R0.w, R0.w;\n"
"MUL R0.xyz, R0.w, fragment.texcoord[2];\n"
"DP3 R0.w, R0, fragment.texcoord[3];\n"
"ADD R0.w, R0.w, 1.0;\n"
"MAD result.color.w, R0.w, 0.8, 0.1;\n"
"END\n";

const char *shader1_v = "\
uniform vec3 p1;\
\
void main()\
{\
	vec4 v1=ftransform();\
	gl_Position=v1;\
	gl_TexCoord[0]=v1;\
	gl_TexCoord[1].xy=gl_MultiTexCoord0.xy;\
	gl_TexCoord[2].xyz=p1-gl_Vertex.xyz;\
}";

const char *shader1_f = "\
#extension GL_ARB_texture_rectangle : enable\n\
uniform vec3 p2;\
uniform vec4 p3;\
uniform sampler2D tx1;\
uniform sampler2D tx2;\
uniform sampler2DRect tx3;\
\
void main()\
{\
	vec3 v1=normalize(gl_TexCoord[2]).xyz;\
	vec3 v2=normalize(texture2D(tx2,gl_TexCoord[1].xy)*2.0-1.0).xyz;\
	vec2 v3=(gl_TexCoord[0].xy/gl_TexCoord[0].w).xy*vec2(-0.5,0.5)+0.5;\
	vec3 v4=texture2D(tx1,v3+v2.xy/p2.xy*p2.z).xyz;\
	vec3 v5=texture2DRect(tx3,gl_FragCoord.xy+v2.xy*p2.z).xyz*p3.xyz;\
	float v6=clamp(dot(v1,v2)+p3.w,0.0,1.0);\
	gl_FragColor=vec4(mix(v4,v5,v6),1.0);\
}";

const char *shader2_v = "\
void main()\
{\
	gl_TexCoord[0]=gl_MultiTexCoord0;\
	gl_Position=ftransform();\
}";

const char *shader2_f = "\
#extension GL_ARB_texture_rectangle : enable\n\
uniform float p1;\
uniform sampler2D tx1;\
uniform sampler2D tx2;\
uniform sampler2DRect tx3;\
\
void main()\
{\
	vec3 v1=texture2D(tx1,gl_TexCoord[0].xy).xyz;\
	vec3 v2=normalize(texture2D(tx2,gl_TexCoord[0].xy)*2.0-1.0).xyz;\
	vec3 v3=texture2DRect(tx3,gl_FragCoord.xy+v2.xy*p1).xyz;\
	gl_FragColor=vec4(v3*v1*2.0,1.0);\
}";

const char *shader3_4_v = "\
uniform vec3 p1;\
attribute vec2 a1;\
attribute vec2 a2;\
attribute vec3 a3;\
attribute vec3 a4;\
attribute vec3 a5;\
\
void main()\
{\
	vec3 v1=gl_Vertex.xyz-p1;\
	gl_TexCoord[0].xy=a1;\
	gl_TexCoord[1].xy=a2;\
	gl_TexCoord[2].x=-dot(v1,a3);\
	gl_TexCoord[2].y=-dot(v1,a4);\
	gl_TexCoord[2].z=dot(v1,a5);\
	gl_Position=ftransform();\
}";

const char *shader3_f = "\
uniform vec3 p2;\
uniform sampler2D tx1;\
uniform sampler2D tx2;\
uniform sampler2D tx3;\
uniform sampler2D tx4;\
\
void main()\
{\
	vec3 v1=normalize(gl_TexCoord[2].xyz);\
	vec2 v2=gl_TexCoord[0].xy+(texture2D(tx3,gl_TexCoord[0].xy).a*2.0-1.0)*v1.xy*p2.x;\
	vec3 v3=normalize(texture2D(tx3,v2).xyz*2.0-1.0);\
	vec3 v4=normalize(texture2D(tx2,gl_TexCoord[1].xy).xyz*2.0-1.0);\
	vec3 v5=texture2D(tx4,v2).xyz*pow(clamp(dot(reflect(-v1,v3.xzy),v4),0.0,1.0),p2.y);\
	v4[2]=0.5;float v6=dot(v3,v4);\
	gl_FragColor.xyz=texture2D(tx1,gl_TexCoord[1].xy).xyz*(max(v6,p2.z)*2.0+v5);\
}";

const char *shader4_f = "\
uniform float p2;\
uniform sampler2D tx1;\
uniform sampler2D tx2;\
uniform sampler2D tx3;\
\
void main()\
{\
	vec3 v1=normalize(gl_TexCoord[2].xyz);\
	vec2 v2=gl_TexCoord[0].xy+(texture2D(tx2,gl_TexCoord[0].xy).a*2.0-1.0)*v1.xy*p2;\
	gl_FragColor=texture2D(tx1,v2)*texture2D(tx3,v2)*2.0;\
}";

const char *shader5_v = "\
void main()\
{\
	vec4 v1=gl_ModelViewMatrix*gl_Vertex;\
	gl_TexCoord[0].x=dot(v1,gl_EyePlaneS[0]);\
	gl_TexCoord[0].y=dot(v1,gl_EyePlaneT[0]);\
	gl_TexCoord[0].z=dot(v1,gl_EyePlaneS[1]);\
	gl_FrontColor.xyz=gl_Color.xyz;\
	gl_Position=ftransform();\
}";

const char *shader5_f = "\
uniform sampler2D tx1;\
uniform sampler2D tx2;\
\
void main()\
{\
	gl_FragColor=vec4(gl_Color.xyz,texture2D(tx1,gl_TexCoord[0].xy).a*texture2D(tx2,vec2(gl_TexCoord[0].z,0.5)).a);\
}";

const char *shader6_v = "\
uniform vec4 p1;\
uniform vec4 p2;\
\
void main()\
{\
	vec3 v1=gl_Vertex.xyz-p1.xyz;\
	vec3 v2=gl_Vertex.xyz-p2.xyz;\
	gl_TexCoord[0]=gl_MultiTexCoord0;\
	gl_TexCoord[1]=gl_MultiTexCoord1;\
	gl_TexCoord[2].x=-dot(v1,gl_MultiTexCoord2.xyz);\
	gl_TexCoord[2].y=-dot(v1,gl_MultiTexCoord3.xyz);\
	gl_TexCoord[2].z=dot(v1,gl_Normal.xyz);\
	gl_TexCoord[3].x=-dot(v2,gl_MultiTexCoord2.xyz);\
	gl_TexCoord[3].y=-dot(v2,gl_MultiTexCoord3.xyz);\
	gl_TexCoord[3].z=dot(v2,gl_Normal.xyz);\
	gl_TexCoord[3].xyz = v2;\
	gl_Position=ftransform();\
}";

const char *shader6_f = "\
uniform vec4 p2;\
uniform vec3 p3;\
uniform sampler2D tx1;\
uniform sampler2D tx2;\
uniform sampler2D tx3;\
\
void main()\
{\
	float v1=length(gl_TexCoord[3].xyz)/p2.w;\
	float v2=1.0-v1*v1;\
	gl_FragColor=vec4(p3.xyz,v2);\
}";
#endif//R_PROGRAMS_H
