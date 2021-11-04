from conans import ConanFile, CMake


class Recipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = [
        'glfw/3.3.4',
        'glad/0.1.34',
        'glm/0.9.9.8',
        'fmt/7.1.3',
        'assimp/5.0.1',
        'catch2/2.13.6',
        'imgui/1.85'
    ]
    generators = 'cmake', 'cmake_find_package'

    def configure(self):
        self.options["glad"].gl_version = "4.6"

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
