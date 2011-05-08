varying vec3 N;
varying vec3 v;
varying vec3 myColor;
void main(void)
{
   v = vec3(gl_ModelViewMatrix * gl_Vertex);
   N = normalize(gl_NormalMatrix * gl_Normal);

   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	
   myColor = gl_MultiTexCoord0.xyz;
}