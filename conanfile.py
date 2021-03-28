from conans import ConanFile, CMake

class Recipe(ConanFile):
    settings = {
        "os": None,
        "compiler": {
            "Visual Studio": {"version": ["16"]}
        },
        "build_type": None,
        "arch": None
    }
    requires = [
        "glew/2.2.0",
        "imgui/1.82",
        "sdl2/2.0.14@bincrafters/stable",
    ]
    generators = [
        "cmake"
    ]
