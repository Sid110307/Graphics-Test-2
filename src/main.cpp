#include <iostream>
#include <cxxopts.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../tests/environments/randomModels/world.h"
#include "./include/model.h"

void initFont()
{
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) std::cerr << "Could not initialize FreeType." << std::endl;

	FT_Face face;
	if (FT_New_Face(ft, "fonts/ubuntu.ttf", 0, &face)) std::cerr << "Could not load font." << std::endl;

	FT_Set_Pixel_Sizes(face, 0, 48);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (GLubyte c = 0; c < 128; c++)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cerr << "Could not load glyph." << std::endl;
			continue;
		}

		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, (GLint) face->glyph->bitmap.width, (GLint) face->glyph->bitmap.rows, 0,
					 GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Characters.insert(std::pair<GLchar, Character>(c, {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				(GLuint) face->glyph->advance.x
		}));
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

World* initWorld(GLFWwindow* window)
{
	Shader shader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	auto* world = new World(
			{
					new BuiltinModels::Snowman(shader, 20.0f, 0.0f, 0.0f),
					new BuiltinModels::Building(shader, -20.0f, 0.0f, 0.0f),
					new BuiltinModels::Tree(shader, 0.0f, 0.0f, 20.0f),
					new BuiltinModels::Water(shader, 0.0f, 0.0f, -20.0f),
					new BuiltinModels::Snow(shader, 20.0f, 0.0f, 20.0f),
			});

	int fbWidth, fbHeight;
	glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
	glViewport(0, 0, fbWidth, fbHeight);
	Camera::changeSize(window, fbWidth, fbHeight);

	glfwSetFramebufferSizeCallback(window, Camera::changeSize);
	glfwSetKeyCallback(window, Camera::processKeys);
	glfwSetScrollCallback(window, Camera::processMouse);

	// FIXME: Model loading is broken
	//	auto model = Model("./tests/models/nanosuit/nanosuit.obj", -20.0f, 0.0f, 20.0f);
	//	model.isModelNameShown = true;
	//	world->addObject(&model);

	World::grid = true;
	World::axes = true;
	World::perspectiveIcon = true;
	world->setGridRange(50.0f);

	return world;
}

int main(int argc, char** argv)
{
	cxxopts::Options options(argv[0]);

	options.add_options()
			("f, fullscreen", "Start in fullscreen mode")
			("res", "Set the resolution to \"arg\" (e.g. --res 640x480)",
			 cxxopts::value<std::string>()->default_value("800x600"))
			("h, help", "Show this help message and exit");

	auto result = options.parse(argc, argv);
	if (result.count("help"))
	{
		std::cout << options.help()
				  << "\nCamera controls:\n"
				  << "  Arrow keys    - pan (hold shift to rotate)\n"
				  << "  Mouse wheel   - move (hold shift to move faster)\n"
				  << "  Space         - reset (hold shift to only reset rotation)\n"
				  << "  Escape        - exit\n"
				  << std::endl;
		exit(EXIT_SUCCESS);
	}

	GLint width = 800, height = 600;
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW." << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(width, height, "Graphics Test 2", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cerr << "Failed to create GLFW window." << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (result.count("fullscreen")) glfwMaximizeWindow(window);
	if (result.count("res"))
	{
		std::string res = result["res"].as<std::string>();
		auto pos = (GLint) res.find('x');

		if (pos == -1)
		{
			std::cerr << "Invalid resolution '" << res << "'." << std::endl;
			exit(EXIT_FAILURE);
		}

		width = std::stoi(res.substr(0, pos));
		height = std::stoi(res.substr(pos + 1));

		glfwSetWindowSize(window, width, height);
	}

	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	initFont();
	auto* world = initWorld(window);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		world->create();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return EXIT_SUCCESS;
}
