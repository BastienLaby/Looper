#if defined(VERTEX)

in vec3 VertexPosition;
in vec3 VertexNormal;
in vec2 VertexTexCoord;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Object = mat4(1.0f);

out vData
{
	vec2 uv;
    vec3 normal;
    vec3 position;
}vertex;

void main(void)
{	

	vertex.uv = VertexTexCoord;
	vertex.normal = VertexNormal;
	vec4 position = Object * vec4(VertexPosition, 1.0);
	vertex.position = position.xyz;
	gl_Position = Projection * View * position;
}

#endif

#if defined(GEOMETRY)

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vData
{
	vec2 uv;
    vec3 normal;
    vec3 position;

}vertices[];

out fData
{
	vec2 uv;
    vec3 normal;
    vec3 position;
}frag;    

void main()
{
	int i;
	for (i = 0; i < gl_in.length(); ++i)
	{
		gl_Position = gl_in[i].gl_Position;
		frag.normal = vertices[i].normal;
		frag.position = vertices[i].position;
		frag.uv = vertices[i].uv;
		EmitVertex();
	}
	EndPrimitive();
}

#endif

#if defined(FRAGMENT)

in fData
{
	vec2 uv;
    vec3 normal;
    vec3 position;
}frag;

out vec4 Color;

void main(void)
{
	Color = vec4(1.0f, 0.0f, 1.0f, 1.0f);
}
#endif
