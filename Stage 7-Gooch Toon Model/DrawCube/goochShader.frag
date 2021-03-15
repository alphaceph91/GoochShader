#version 330 core									

in vec3 fragPos;
in vec3 normal;

//uniforms variables passed by the application
uniform vec3 SurfaceColor;
uniform vec3 WarmColor;
uniform vec3 CoolColor;
uniform float DiffuseWarm;
uniform float DiffuseCool;

in float NdotL; //the weighting factor is NdotL, the lighting value

//variables passed by the vertex shader
in vec3 ReflectVec;
in vec3 ViewVec;

out vec4 fragColor;		

void main() {	
	//combination of CoolColor, WarmColor, and surface color
	vec3 kcool = min(CoolColor + DiffuseCool * SurfaceColor, 1.0);
	vec3 kwarm = min(WarmColor + DiffuseWarm * SurfaceColor, 1.0);

	//interpolation by dot product of normal and light vector
	vec3 kfinal = mix(kcool, kwarm, NdotL);

	//specular(spec) highlighted by reflected lightVec and viewVec
	vec3 nreflect = normalize(ReflectVec); //nreflect is normalised ReflectVec
	vec3 nview = normalize(ViewVec); //nview is normalised ViewVec

	float spec = max(dot(nreflect, nview), 0.0); //spec is specular
	//spec = pow(spec, 32.0);

	fragColor = vec4(min(kfinal + spec, 1.0), 1.0);
}