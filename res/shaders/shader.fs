#version 330 core
in vec3 vertextColor;
in vec2 texCoord;
uniform sampler2D ourTexture;
uniform sampler2D faceTexture;
out vec4 FragColor;
void main() {
	//FragColor = vec4(vertextColor, 1.0f);
	//区别于向量乘法, 就是简单一一相乘,结果仍然是一个4维向量
	//FragColor = texture(ourTexture, texCoord) * vec4(vertextColor, 1.0f);
	FragColor = mix(texture(ourTexture, texCoord) , texture(faceTexture, texCoord), 0.2);
}