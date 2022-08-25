#include <GL/glew.h>
#include <cxxopts.hpp>
#include <iostream>

#include "../tests/environments/randomModels/world.h"
#include "./include/model.h"

void initialize(int* argc, char** argv)
{
	try
	{
		cxxopts::Options options(argv[0]);

		options.add_options()
				("f, fullscreen", "Start in fullscreen mode")
				("res", "Set the resolution to \"arg\" (e.g. --res 640x480)",
				 cxxopts::value<std::string>()->default_value("800x600"))
				("h, help", "Show this help message and exit");

		auto result = options.parse(*argc, argv);

		if (result.count("help"))
		{
			std::cout << options.help()
					  << "\nCamera controls:\n"
					  << "  Arrow keys    - pan (hold shift to rotate)\n"
					  << "  Mouse wheel   - move (hold shift to move faster)\n"
					  << "  Space         - reset (hold shift to only reset rotation)\n"
					  << "  Escape        - exit\n"
					  << std::endl;

			std::cin.ignore();
			exit(EXIT_SUCCESS);
		}

		int width = 800, height = 600;

		glutInit(argc, argv);
		glutInitContextVersion(3, 3);
		glutInitContextProfile(GLUT_CORE_PROFILE);
		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

		glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - width) >> 1,
							   (glutGet(GLUT_SCREEN_HEIGHT) - height) >> 1);
		glutInitWindowSize(width, height);
		glutCreateWindow("Graphics Test 2");

		if (result.count("fullscreen")) glutFullScreen();
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

			glutReshapeWindow(width, height);
		}
	}
	catch (const cxxopts::OptionException &e)
	{
		std::cerr << "Error parsing options: " << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}

	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
		exit(EXIT_FAILURE);
	}
}

auto main(int argc, char** argv) -> int
{
	initialize(&argc, argv);
	auto* world = new World({
									new BuiltinModels::Snowman(20.0f, 0.0f,
															   0.0f),
									new BuiltinModels::Building(-20.0f, 0.0f,
																0.0f),
									new BuiltinModels::Tree(0.0f, 0.0f, 20.0f),
									new BuiltinModels::Water(0.0f, 0.0f,
															 -20.0f),
									new BuiltinModels::Snow(20.0f, 0.0f, 20.0f),
							});

	glutDisplayFunc(World::display);
	glutIdleFunc(World::display);

	glutReshapeFunc(Camera::changeSize);
	glutKeyboardFunc(Camera::processNormalKeys);
	glutSpecialFunc(Camera::processSpecialKeys);
	glutMouseFunc(Camera::processMouse);

	auto model = Model(
			"/home/sid/Desktop/Graphics Test 2/tests/models/nanosuit/nanosuit.obj",
			-20.0f, 0.0f, 20.0f);
	model.showModelName(true);
	world->addObject(&model);

	world->setGridRange(50.0f);
	world->setGrid(true);
	world->setAxes(true);
	world->setPerspectiveIcon(true);
	world->create();

	glutMainLoop();
	return EXIT_SUCCESS;
}
