#include <iostream>
#define GLEW_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include "Shader.h"

void framebuffer_size_callback(GLFWwindow *, int, int);
void processInput(GLFWwindow *);

float vertices[] = {
	//     ---- λ�� ----       ---- ��ɫ ----     - �������� -
	0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // ����
	0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // ����
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ����
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // ����
};

unsigned int indices[] = {
	0, 3, 1,
	3, 2, 1
};

ShaderParam shaders[] = {
	{ GL_VERTEX_SHADER, "./shader.vs" },
	{ GL_FRAGMENT_SHADER, "./shader.fs" },
	{ GL_NONE, NULL }
};

int main() {

	glfwInit();
	//����glfwʹ�õ�opengl�汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//����openglʹ��ʲôprofile(����), �˴�ָ��Ϊ����ģʽ, ��opengl��һ���Ӽ�, ����Ҫ����������
	//���ڹ̶�����, ���ڿɱ�̹���
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	//glfw���ڴ���
	GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//��ʼ��glew
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed init GLEW" << std::endl;
		glfwTerminate();
		return -1;
	}
	//opengl��Ⱦ�Ӵ���С, ������õı�glfw����С�Ļ�, ��Ƕ����glfw���ڵ�һƬ������
	//OpenGLĻ��ʹ��glViewport�ж����λ�úͿ�߽���2D�����ת������OpenGL�е�λ������ת��Ϊ�����Ļ���ꡣ
	//���磬OpenGL�е�����(-0.5, 0.5)�п��ܣ����գ���ӳ��Ϊ��Ļ�е�����(200,450)��
	//ע�⣬�������OpenGL���귶ΧֻΪ-1��1�����������ʵ�Ͻ�(-1��1)��Χ�ڵ�����ӳ�䵽(0, 800)��(0, 600)��
	//pram0&pram1:���ƴ������½ǵ�λ��
	//pram1&pram2:������Ⱦ���ڵĿ�Ⱥ͸߶�(����)
	glViewport(0, 0, 800, 600);
	//����glfw���ڴ�С�ı���¼��ص�
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//���������޳�ģʽ, Ĭ����ʱ��Ϊ���� 
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//����������������з���Ϊ˳ʱ��
	//glFrontFace(GL_CW);

	Shader shaderProgram = Shader(shaders);
	//����VAO
	unsigned int VAO; //��ͬ�� GLuint VAO, VAO��ID
	//unsigned int VAO[10]; //���԰󶨶��
	//����1��VAO
	glGenVertexArrays(1, &VAO);
	//��VAO����Ⱦ����
	glBindVertexArray(VAO);

	//����һ��VBO
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	//��VBO��glContext
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//cpu�Ķ������ݴ��䵽gpu
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//����һ��EBO
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	//��EBO��context
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//cpu�Ķ����������ݴ��䵽gpu
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//����һ��texture buffer
	unsigned int TBO;
	glGenTextures(1, &TBO);
	glBindTexture(GL_TEXTURE_2D, TBO);
	//Ϊ����������û���,���˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//opengl������������겻һ��, opengl��������Ϊԭ��, ͼƬ������Ϊԭ��, ��y����Ҫ��ת
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	unsigned char *data = stbi_load("./container.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		printf("error occured");
	}
	stbi_image_free(data);

	unsigned int TBO2;
	glGenTextures(1, &TBO2);
	glBindTexture(GL_TEXTURE_2D, TBO2);
	data = stbi_load("./awesomeface.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		printf("error occured");
	}
	stbi_image_free(data);
	
	
	//��VBOȡ����������䵽VAOָ����slot
	//param0: slot��,0-15, shader������Ҫ�ú����ָ����slotȡ����,eg, layout(location=6) in vec3 xxx;

	//param2: �������Ը���, ������װVAO��slot,��r,g,b,a,��ֵΪ4
	//param3: �������Ե���������
	//param4: �Ƿ�Normalization
	//param5 & param6 : ȡֵ�Ĳ�����ƫ��, byteΪ��λ
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
	//һ��Ҫenable��Ӧ��slot
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	shaderProgram.active();
	//��ʱĬ�ϻὫTBO�����͸�Ƭ����ɫ����һ��sampler2D, ����Ҫ���ֶ�uniform����ȥ
	//�����Ҫ��̬������λ, ��Ҫ�ֶ�ָ��uniform
	glUniform1i(glGetUniformLocation(shaderProgram.ID, "ourTexture"), 0);
	glUniform1i(glGetUniformLocation(shaderProgram.ID, "faceTexture"), 1);


	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		//display();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TBO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, TBO2);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//draw�Ļ��Ʒ�ʽ��GL_TRIANGLES, GL_TRIANGLES_FAN(��������), GL_TRIANGLES_STRIP(�����δ�)
		//drawelements����EBO��������
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glFlush();
		//˫����(Double Buffer)

		//Ӧ�ó���ʹ�õ������ͼʱ���ܻ����ͼ����˸�����⡣ 
		//������Ϊ���ɵ�ͼ����һ���ӱ����Ƴ����ģ����ǰ��մ����ң����϶��������صػ��ƶ��ɵġ�
		//����ͼ������˲����ʾ���û�������ͨ��һ��һ�����ɵģ���ᵼ����Ⱦ�Ľ���ܲ���ʵ��
		//Ϊ�˹����Щ���⣬����Ӧ��˫������Ⱦ����Ӧ�ó���ǰ���屣�������������ͼ����������Ļ����ʾ��
		//�����еĵ���Ⱦָ����ں󻺳��ϻ��ơ������е���Ⱦָ��ִ����Ϻ����ǽ���(Swap)ǰ����ͺ󻺳壬����ͼ����������Գ�����֮ǰ�ᵽ�Ĳ���ʵ�о������ˡ�
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		std::cout << "escape key pressed" << std::endl;
		glfwSetWindowShouldClose(window, true);
	}
}