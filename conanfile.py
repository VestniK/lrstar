from conans import ConanFile, CMake

class Lrstar(ConanFile):
    name = "lrstar"
    version = "6.6.004"
    settings = "os", "compiler", "build_type", "arch"
    url="http://github.com/VestniK/lrstar"
    exports = "*.cpp", "*.h", "*CMakeLists.txt", "*.grm", "*.lgr", "*.skl", "*.cmake", "*.in"

    def build(self):
        cmake = CMake(self.settings)
        if self.settings.os == "Windows":
            self.run("IF not exist _build mkdir _build")
        else:
            self.run("[ -d _build ] || mkdir _build")
        self.run('cd _build && cmake %s %s -DCMAKE_INSTALL_PREFIX=%s' % (self.conanfile_directory, cmake.command_line, self.package_folder))
        self.run("cd _build && cmake --build . --target install %s" % cmake.build_config)

    def package(self):
        pass

    def package_info(self):
        pass
