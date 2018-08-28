#version 330 core
in vec3 vertextColor;
out vec4 FragColor;
void main() {
	FragColor = vec4(vertextColor, 1.0f);
}