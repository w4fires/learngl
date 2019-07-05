#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;
out vec3 vertextColor;
out vec2 texCoord;
uniform mat4 trans;
void main() {
	gl_Position = trans * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	vertextColor = aColor;
	texCoord = aTexCoord;
}