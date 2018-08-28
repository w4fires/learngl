#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow *, int, int);
void processInput(GLFWwindow *);

float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f,  0.5f, 0.0f
};

const char *vertextShaderSource = "#version 330 core\nlayout(location = 0) in vec3 aPos;\nvoid main()\n {\n gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n}";
const char *fragmentShaderSource = "#version 330 core\nout vec4 FragColor;\n void main()\n {\n FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n}";


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

	//shader����
	unsigned int vertextShader;
	vertextShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertextShader, 1, &vertextShaderSource, NULL);
	glCompileShader(vertextShader);

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertextShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

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

	//��VBOȡ����������䵽VAOָ����slot
	//param0: slot��,0-15, shader������Ҫ�ú����ָ����slotȡ����,eg, layout(location=6) in vec3 xxx;
	//param2: �������Ը���, ������װVAO��slot,��r,g,b,a,��ֵΪ4
	//param3: �������Ե���������
	//param4: �Ƿ�Normalization
	//param5 & param6 : ȡֵ�Ĳ�����ƫ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	//һ��Ҫenable��Ӧ��slot
	glEnableVertexAttribArray(0);


	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		//display();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindVertexArray(VAO);
		glUseProgram(shaderProgram);
		glDrawArrays(GL_TRIANGLES, 0, 3);
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