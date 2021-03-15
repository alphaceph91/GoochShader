#version 330 core							

uniform vec3 lightPos;

in vec3 fragPos;
in vec3 normal;

out vec4 fragColor;

void main() {

	// light
	vec3 lightColor = vec3(1.0, 1.0, 1.0);

	// ambient
	float ambientFactor = 0.1;
	vec3 ambient = ambientFactor * lightColor;

	// diffuse
	vec3 nnormal = normalize(normal); // incoming normal vector normalized for the diffuse light, nnnormal will be our normalized normal
	vec3 lightDir = normalize(lightPos - fragPos); // normalizing the lightDir aswell
	float diffFactor = max(dot(nnormal, lightDir), 0.0);
	vec3 diffuse = diffFactor * lightColor;
	
	vec3 result = (ambient + diffuse) * vec3(1.0, 1.0, 1.0);

	float intensity = max(dot(nnormal, lightDir), 0.0); // intensity now uses proper normalized normal vector and lightDir
	vec3 toonColor;

	if (intensity > 0.95) // cosine greater than 0.95 will result in 
		toonColor = vec3(1.0, 1.0, 1.0);
	else if (intensity > 0.75) toonColor = vec3(0.8, 0.8, 0.8);
	else if (intensity > 0.50) toonColor = vec3(0.6, 0.6, 0.6);
	else if (intensity > 0.25) toonColor = vec3(0.4, 0.4, 0.4);
	else					   toonColor = vec3(0.2, 0.2, 0.2);

	vec3 resultColor = toonColor * vec3(1.0, 0.0, 0.0);

	fragColor = vec4(resultColor, 1.0);
}