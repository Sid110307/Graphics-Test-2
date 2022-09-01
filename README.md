# Graphics Test 2

> OpenGL Test with camera movement

## Requirements

* A Graphics card with DirectX 11 (or higher) supported
* OpenGL v4.0 or higher
* FreeGLUT (GL Utility Toolkit)
* GLEW (GL Extension Wrangler) v4.0 or higher
* Assimp (Asset Import Library) v5.0 or higher
* SOIL (Simple OpenGL Image Library)
* A C++ compiler that supports C++20
    * [GCC](https://gcc.gnu.org/download/gcc.html)
    * [Clang](https://clang.llvm.org/download.html)
    * [Visual Studio](https://visualstudio.microsoft.com/downloads/)

## Installation

* Clone the repository to your local machine

```bash
$ git clone https://github.com/Sid110307/Graphics-Test-2.git GraphicsTest2
```

* Build the project

```bash
$ cd GraphicsTest2
$ cmake --build bin --target clean -j4
$ cmake --build bin --target graphicsTest2 -j4
```

* Compile the examples

```bash
cd GraphicsTest2
make examples -s
```

## Usage

* Run the examples

```bash
./bin/graphicsTest2
```

* Get help

```bash
./bin/graphicsTest2 --help
```

## License

> [MIT](https://opensource.org/licenses/MIT)
