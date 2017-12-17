#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.

in vec3 fragNormal;
in vec3 objNormal;
in vec3 fragPos; // fragVert

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{
    vec3 norm = (normalize(objNormal)+1.0f)/2.0f; // only for normal coloring
    fragColor = vec4(norm, 1.0f); // normal coloring

}








