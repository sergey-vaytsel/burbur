from conans import ConanFile, CMake


class Recipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = [
        'glfw/3.3.4',
        'glad/0.1.34',
        'glm/0.9.9.8',
        'fmt/7.1.3',
    ]
    generators = 'cmake', 'cmake_find_package'

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
