from conans import ConanFile, CMake, tools, __version__ as conan_version
from conans.model.version import Version
import subprocess
import os, getpass, string, unicodedata

class PictofitStudioConan(ConanFile):
    name = "example"
    settings = "os", "compiler", "build_type", "arch"
    exports_sources = "*"
    options = {"shared"                            : [True, False],
               "fPIC"                              : [True, False]
              }
    default_options = {"shared" : False,
                       "fPIC"   : True
                       }
    generators = "qt", "cmake", "cmake_find_package_multi", "qmake"

    def package_id(self):
        if self.settings.compiler == "Visual Studio":
            if "MD" in self.settings.compiler.runtime:
                self.info.settings.compiler.runtime = "MD/MDd"
            else:
                self.info.settings.compiler.runtime = "MT/MTd"

    def requirements(self):
        self.requires.add('qt/6.2.4')
        self.options["qt"].shared = True
        self.options["qt"].with_vulkan = True
        self.options["qt"].qtsvg = True
        self.options["qt"].qt5compat = True
        self.options["qt"].with_libjpeg = "libjpeg"

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

