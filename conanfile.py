from conan import ConanFile
from conan.tools.cmake import CMakeDeps, CMakeToolchain, cmake_layout

class SetUp(ConanFile):
    settings = "os", "arch", "compiler", "build_type"
    options = { "sanitizer": ["none", "asan", "tsan", "msan", "ubsan"] }
    default_options = { "sanitizer": "none" }

    def configure(self):
        import os
        project_root = os.getcwd()

        flags = [ f"-fprebuilt-module-path={project_root}/modules" ]

        # DEBUG
        if self.settings.build_type == "Debug":
            print("Configuring DEBUG build.")

            # ENABLE ALL WARNINGS
            warning_flags = [
                "-Wall", "-Wextra", "-Wpedantic", "-Wshadow", "-Wnon-virtual-dtor",
                "-Wcast-align", "-Wunused", "-Woverloaded-virtual","-Wdouble-promotion",
                "-Wconversion", "-Wmisleading-indentation",
            ]

            # SELECT SANITIZER
            sanitizer_flags = []
            if self.options.sanitizer == "asan":
                sanitizer_flags = ["-fsanitize=address", "-fsanitize=leak"]
            elif self.options.sanitizer == "tsan":
                sanitizer_flags = ["-fsanitize=thread"]
            elif self.options.sanitizer == "msan":
                sanitizer_flags = ["-fsanitize=memory"]
            elif self.options.sanitizer == "ubsan":
                sanitizer_flags = ["-fsanitize=undefined"]

            flags += warning_flags
            flags += sanitizer_flags
            flags.extend(["-O0", "-g", "-fno-omit-frame-pointer"])
            
            self.conf.define("tools.build:sharedlinkflags", sanitizer_flags)
            self.conf.define("tools.build:exelinkflags", sanitizer_flags)

        # RELEASE
        elif self.settings.build_type == "Release":
            print("Configuring RELEASE build.")
            flags += ["-O3", "-DNDEBUG", "-march=native", "-mtune=native", "-flto"]

        self.conf.define("tools.build:cxxflags", flags)
        self.conf.define("tools.build:cflags", flags)
    
    def generate(self):
        cmake = CMakeToolchain(self)
        cmake.generator = "Ninja"
        cmake.generate()

        deps = CMakeDeps(self)
        deps.generate()

    def layout(self):
        cmake_layout(self)