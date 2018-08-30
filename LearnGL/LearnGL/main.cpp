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
	//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
	0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
	0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
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
	//告诉glfw使用的opengl版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//告诉opengl使用什么profile(配置), 此处指定为核心模式, 即opengl的一个子集, 不需要考虑向后兼容
	//早期固定管线, 现在可编程管线
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	//glfw窗口创建
	GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//初始化glew
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed init GLEW" << std::endl;
		glfwTerminate();
		return -1;
	}
	//opengl渲染视窗大小, 如果设置的比glfw窗口小的话, 则嵌入在glfw窗口的一片区域中
	//OpenGL幕后使用glViewport中定义的位置和宽高进行2D坐标的转换，将OpenGL中的位置坐标转换为你的屏幕坐标。
	//例如，OpenGL中的坐标(-0.5, 0.5)有可能（最终）被映射为屏幕中的坐标(200,450)。
	//注意，处理过的OpenGL坐标范围只为-1到1，因此我们事实上将(-1到1)范围内的坐标映射到(0, 800)和(0, 600)。
	//pram0&pram1:控制窗口左下角的位置
	//pram1&pram2:控制渲染窗口的宽度和高度(像素)
	glViewport(0, 0, 800, 600);
	//设置glfw窗口大小改变的事件回调
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//开启背面剔除模式, 默认逆时针为正面 
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//可以设置正面的绕行方向为顺时针
	//glFrontFace(GL_CW);

	Shader shaderProgram = Shader(shaders);
	//构造VAO
	unsigned int VAO; //等同于 GLuint VAO, VAO的ID
	//unsigned int VAO[10]; //可以绑定多个
	//生成1个VAO
	glGenVertexArrays(1, &VAO);
	//绑定VAO到渲染管线
	glBindVertexArray(VAO);

	//生成一个VBO
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	//绑定VBO到glContext
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//cpu的顶点数据传输到gpu
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//生成一个EBO
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	//绑定EBO到context
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//cpu的顶点索引数据传输到gpu
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//生成一个texture buffer
	unsigned int TBO;
	glGenTextures(1, &TBO);
	glBindTexture(GL_TEXTURE_2D, TBO);
	//为纹理对象设置环绕,过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//opengl坐标和像素坐标不一致, opengl是以左下为原点, 图片以左上为原点, 即y轴需要翻转
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
	
	
	//从VBO取顶点数据填充到VAO指定的slot
	//param0: slot号,0-15, shader程序需要该号码从指定的slot取数据,eg, layout(location=6) in vec3 xxx;

	//param2: 顶点属性个数, 用来组装VAO的slot,如r,g,b,a,该值为4
	//param3: 顶点属性的数据类型
	//param4: 是否Normalization
	//param5 & param6 : 取值的步长和偏移, byte为单位
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
	//一定要enable对应的slot
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	shaderProgram.active();
	//绑定时默认会将TBO号码送给片段着色器第一个sampler2D, 不需要在手动uniform传过去
	//如果需要动态滑动槽位, 需要手动指定uniform
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
		//draw的绘制方式有GL_TRIANGLES, GL_TRIANGLES_FAN(三角形扇), GL_TRIANGLES_STRIP(三角形带)
		//drawelements采用EBO索引绘制
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glFlush();
		//双缓冲(Double Buffer)

		//应用程序使用单缓冲绘图时可能会存在图像闪烁的问题。 
		//这是因为生成的图像不是一下子被绘制出来的，而是按照从左到右，由上而下逐像素地绘制而成的。
		//最终图像不是在瞬间显示给用户，而是通过一步一步生成的，这会导致渲染的结果很不真实。
		//为了规避这些问题，我们应用双缓冲渲染窗口应用程序。前缓冲保存着最终输出的图像，它会在屏幕上显示；
		//而所有的的渲染指令都会在后缓冲上绘制。当所有的渲染指令执行完毕后，我们交换(Swap)前缓冲和后缓冲，这样图像就立即呈显出来，之前提到的不真实感就消除了。
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