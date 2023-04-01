#include <GL/glew.h>
#include <cxxopts.hpp>
#include <iostream>

#include <freetype2/ft2build.h>

#include "../tests/environments/randomModels/world.h"
#include "./include/model.h"

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

	int width = 800, height = 600;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);

	GLFWwindow* window = glfwCreateWindow(width, height, "Graphics Test 2", NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (result.count("fullscreen")) glfwMaximizeWindow(window);
	if (result.count("res"))
	{
		std::string res = result["res"].as<std::string>();
		auto pos = (GLint)res.find('x');

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

	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cerr << "Failed to initialize FreeType library." << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	FT_Face face;
	if (FT_New_Face(ft, "tests/environments/randomModels/Ubuntu-R.ttf", 0, &face))
	{
		std::cerr << "Failed to load font file." << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	FT_Set_Pixel_Size(face, 0);

	auto* world = new World({
		new BuiltinModels::Snowman(20.0f, 0.0f, 0.0f),
		new BuiltinModels::Building(-20.0f, 0.0f, 0.0f),
		new BuiltinModels::Tree(0.0f, 0.0f, 20.0f),
		new BuiltinModels::Water(0.0f, 0.0f, -20.0f),
		new BuiltinModels::Snow(20.0f, 0.0f, 20.0f),
		});

	int fbWidth, fbHeight;
	glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
	glViewport(0, 0, fbWidth, fbHeight);
	Camera::changeSize(window, fbWidth, fbHeight);

	glfwSetFramebufferSizeCallback(window, Camera::changeSize);
	glfwSetKeyCallback(window, Camera::processKeys);
	glfwSetMouseButtonCallback(window, Camera::processMouse);

	// FIXME: Model loading is broken
	// auto model = Model(
	// 	"/home/user/Documents/Graphics Test 2/tests/models/nanosuit/nanosuit.obj",
	// 	-20.0f, 0.0f, 20.0f);
	// model.showModelName(true);
	// world->addObject(&model);

	world->setGridRange(50.0f);
	world->setGrid(true);
	world->setAxes(true);
	world->setPerspectiveIcon(true);
	world->create();

	while (!glfwWindowShouldClose(window))
	{
		world->display();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	delete world;
	glfwTerminate();

	return EXIT_SUCCESS;
}
