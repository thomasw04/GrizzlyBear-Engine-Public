from conan.tools.files import copy
from conan.tools.cmake import CMakeDeps
from conan.tools.cmake import CMakeToolchain
from conan import ConanFile
from os.path import join

class Pkg(ConanFile):
    settings = "os", "compiler", "build_type", "arch"

    def requirements(self):
        self.requires("fmt/10.1.1")
        self.requires("glew/2.2.0")
        self.requires("glfw/3.3.8")
        self.requires("imgui/cci.20230105+1.89.2.docking")
        self.requires("boost/1.83.0")
        self.requires("glm/cci.20230113")
        self.requires("nlohmann_json/3.11.2")
        self.requires("spdlog/1.12.0")
        self.requires("libsodium/cci.20220430")
        self.requires("luajit/2.1.0-beta3")
        self.requires("stb/cci.20220909")

    def build_requirements(self):
        self.tool_requires("cmake/3.22.6")

    def generate(self):
        for dep in self.dependencies.values():
            copy(self, "imgui_impl_glfw.cpp", join(dep.package_folder, "res/bindings"), join(self.build_folder, "../GrizzlyCore/bindings"))
            copy(self, "imgui_impl_opengl3.cpp", join(dep.package_folder, "res/bindings"), join(self.build_folder, "../GrizzlyCore/bindings"))
            copy(self, "imgui_impl_glfw.h", join(dep.package_folder, "res/bindings"), join(self.build_folder, "../GrizzlyCore/bindings"))
            copy(self, "imgui_impl_opengl3.h", join(dep.package_folder, "res/bindings"), join(self.build_folder, "../GrizzlyCore/bindings"))
            copy(self, "imgui_impl_opengl3_loader.h", join(dep.package_folder, "res/bindings"), join(self.build_folder, "../GrizzlyCore/bindings"))
            copy(self, "stb_image.c", join(dep.package_folder, "include/"), join(self.build_folder, "../GrizzlyCore/bindings"))
        
        cmake = CMakeDeps(self)
        ct = CMakeToolchain(self)
        cmake.generate()
        ct.generate()