#extension GL_ARB_texture_rectangle : enable
uniform sampler2DRect tex;
uniform float numrows;
varying vec4 color1, color2, color3;
varying float edge1, edge2, edge3;
varying vec3 bary;
varying float R;
varying float faceOffset;
varying float lightIntensity;
void main()
{	
	gl_Position = ftransform();
        normalize(gl_Normal);
        vec4 lightoff = gl_LightSource[0].position - gl_ModelViewMatrix*gl_Vertex ;
        lightIntensity = dot(normalize(lightoff),gl_Normal) + 0.5;

	R = gl_MultiTexCoord2.x;
	faceOffset = gl_MultiTexCoord2.y;
	bary = gl_MultiTexCoord1.xyz;
	
	edge1 = gl_MultiTexCoord3.x;
	edge2 = gl_MultiTexCoord3.y;
	edge3 = gl_MultiTexCoord3.z;

	//vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);


	float row1 = float(floor(gl_MultiTexCoord0.x/4096.0));
	float col1 = float(mod(gl_MultiTexCoord0.x, 4096.0));
	color1 = texture2DRect(tex, vec2(col1, row1));
	
	float row2 = float(floor(gl_MultiTexCoord0.y/4096.0));
	float col2 = float(mod(gl_MultiTexCoord0.y, 4096.0));
	color2 = texture2DRect(tex, vec2(col2, row2));

	float row3 = float(floor(gl_MultiTexCoord0.z/4096.0));
	float col3 = float(mod(gl_MultiTexCoord0.z, 4096.0));
	color3 = texture2DRect(tex, vec2(col3, row3));
}

