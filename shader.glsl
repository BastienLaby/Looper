#if defined(VERTEX)
uniform mat4 Projection;
uniform mat4 View;

in vec3 VertexPosition;
in vec3 VertexNormal;
in vec2 VertexTexCoord;

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
	vertex.position = VertexPosition;
	gl_Position = Projection * View * vec4(VertexPosition, 1.0f);
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

uniform sampler2D DiffuseTexture;

out vec4 Color;

void main(void)
{
	Color = texture(DiffuseTexture, frag.uv);
	//Color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
#endif
