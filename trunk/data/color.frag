#extension GL_ARB_texture_rectangle : enable
uniform sampler2DRect tex;
varying vec4 color1, color2, color3;
varying float edge1, edge2, edge3;
varying vec4 bary;
varying float R;
varying float faceOffset;
varying float lightIntensity;

vec4 C(float i, float j){


	if(abs(i) <= .001 && abs(j)<=.001){
		return(color3);
	}

	else if(abs(i-R)<=.001 && abs(j)<=.001){
		return(color1);
	}
	else if(abs(i)<=.001 && abs(j-R)<=.001){
		return(color2);
	}



	else if(abs(i)<=.001){
		
		float e1 = edge1 + j;
		float row1 = floor(e1/4096.0);
		float col1 = mod(e1, 4096.0);
		return(texture2DRect(tex, vec2(col1, row1)));
	}

	else if(abs(j)<=.001){
		
		float e2 = edge2 + i;
		float row2 = floor(e2/4096.0);
		float col2 = mod(e2, 4096.0);
		return(texture2DRect(tex, vec2(col2, row2)));
	}
	else if(abs(i+j-(R))<=.001)
	{

		float e3 = edge3 + j;
		float row3 = floor(e3/4096.0);
		float col3 = mod(e3, 4096.0);
		return(texture2DRect(tex, vec2(col3, row3)));
	}


	else {

		float x = float(i-1.0);
		float y = float(j-1.0);
		float offset = float(x + y*(R - 2.0) + ((y-y*y)/2.0) + faceOffset);
		float row3 = float(floor(offset/4096.0));
		float col3 = float(mod(offset, 4096.0));
		return(texture2DRect(tex, vec2(col3, row3)));
	}

}



void main()
{
	

	/*float faceStart = faceOffset;
	float row3 = floor(faceStart/4096.0);
	float col3 = mod(faceStart, 4096.0);
	gl_FragColor = texture2DRect(tex, vec2(col3, row3));*/


	vec4 B = floor(R * bary);
	float i = B.x;
	float j = B.y;
	vec4 w = R * bary - B;

	if(abs(w.x) <= .001 && abs(w.y) <= .001 && abs(w.y) <= .001){
		gl_FragColor = lightIntensity * C(i,j);
	}

	else if(abs(w.x + w.y + w.z - 1.0) <=.001){
		gl_FragColor = lightIntensity * (C(i+1.0, j)*w.x+ C(i, j+1.0)*w.y + C(i, j)*w.z);
	}

	else{
		vec4 wprime = vec4(1.0,1.0,1.0,1.0)-w;
		gl_FragColor = lightIntensity * (C(i, j+1.0)*wprime.x + 
			       			 C(i+1.0, j)*wprime.y + 
			       			 C(i+1.0, j+1.0)*wprime.z);
	}


}