from conan import ConanFile


class BackendRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    def requirements(self):
        self.requires("crowcpp-crow/1.0+5")
        self.requires("sqlite3/3.42.0")
        self.requires("catch2/3.4.0")
        self.requires("libsodium/1.0.18")

    def build_requirements(self):
        self.tool_requires("cmake/3.22.6")
