from conans import ConanFile, CMake, tools, __version__ as conan_version
from conans.model.version import Version
import subprocess
import os, getpass, string, unicodedata

url = "git@gitlab.com:reactivereality/contentpipeline/pictofitstudio.git"

def cleanName(name):
    validUsernameChars = "_{}{}".format(string.ascii_letters, string.digits)
    replaceChars = " \t"
    for c in replaceChars:
        username = name.replace(c,'_')

    # keep only valid ascii chars
    cleanedUsername = unicodedata.normalize('NFKD', username).encode('ASCII', 'ignore').decode()

    # keep only whitelisted chars
    cleanedUsername = ''.join(c for c in cleanedUsername if c in validUsernameChars)
    return cleanedUsername

def get_user():
    username = cleanName(getpass.getuser())
    return username if username else "user"

class PictofitStudioConan(ConanFile):
    name = "pictofitstudio"
    default_user    = get_user()
    default_channel = "testing"
    url             = url
    keep_imports = True
    topics = ("gui", "image_processing", "reactive_reality")
    settings = "os", "compiler", "build_type", "arch"
    exports_sources = "*", "!pictofitstudio-gui-tests/*", "pictofitstudio/libraries/*"
    options = {"shared"                            : [True, False],
               "fPIC"                              : [True, False]
              }
    default_options = {"shared" : False,
                       "fPIC"   : True
                       }
    generators = "qt", "cmake", "cmake_find_package_multi", "qmake"

    def set_version(self):
        git = tools.Git(folder=self.recipe_folder)
        self.version = git.run("describe --always --tags --abbrev=9")

    def package_id(self):
        if self.settings.compiler == "Visual Studio":
            if "MD" in self.settings.compiler.runtime:
                self.info.settings.compiler.runtime = "MD/MDd"
            else:
                self.info.settings.compiler.runtime = "MT/MTd"

    def requirements(self):
        if conan_version < Version("1.28"):
            raise ConanInvalidConfiguration("update conan client to version >= 1.28")

        self.requires.add('qt/6.1.2')
        self.options["qt"].shared = True
        self.options["qt"].with_vulkan = True
        self.options["qt"].qtsvg = True
        self.options["qt"].qt5compat = True
        self.options["qt"].with_libjpeg = "libjpeg"

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

