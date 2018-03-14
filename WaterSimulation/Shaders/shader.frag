#version 330 core

//in vec3 sampleExtraOutput;
//in vec3 P;
in vec3 N;
in vec3 C;
//in vec2 lights;

out vec4 color;

void directional();

void main()
{
	//color = vec4(C, 1.0f);
	directional();
	//color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	//color = vec4(N, 1.0f);
}

void directional()
{
	vec3 norm = normalize(N);

	//vec3 L1 = normalize(vec3(1.0f, 1.0f, 1.0f));
	vec3 L1 = normalize(vec3(-1.0f, 1.0f, 1.0f));
	//vec3 lightColor1 = vec3(0.0f, 0.0f, 1.0f);
	vec3 lightColor1 = vec3(1.0f, 1.0f, 1.0f);

	vec3 L2 = normalize(vec3(-1.0f, 1.0f, 1.0f));
	vec3 lightColor2 = vec3(0.0f, 1.0f, 0.0f);
	//vec3 lightColor2 = vec3(1.0f, 1.0f, 1.0f);
	//vec3 lightColor2 = vec3(0.8f, 0.8f, 0.8f);

	// diffuse
	float k_d = 0.6f;
	float inProduct1 = clamp( dot( L1, norm), 0, 1 );
	vec3 diff1 = k_d * inProduct1 * lightColor1;

	float inProduct2 = clamp( dot( L2, norm), 0, 1 );
	vec3 diff2 = k_d * inProduct2 * lightColor2;

	// ambient
	vec3 ambi1 = vec3(0.5f, 0.5f, 0.5f);

	// specular

	vec3 total = vec3(0.0f, 0.0f, 0.0f);
	total += diff1;
	//total += diff2;

	vec3 result = (total + ambi1) * C;
	color = vec4(result, 1.0f);
}