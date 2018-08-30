#version 330 core
in vec3 vertextColor;
in vec2 texCoord;
uniform sampler2D ourTexture;
out vec4 FragColor;
void main() {
	//FragColor = vec4(vertextColor, 1.0f);
	//�����������˷�, ���Ǽ�һһ���,�����Ȼ��һ��4ά����
	FragColor = texture(ourTexture, texCoord) * vec4(vertextColor, 1.0f);
}