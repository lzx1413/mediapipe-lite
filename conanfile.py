from conan import ConanFile
from conan.errors import ConanInvalidConfiguration
from conan.tools.build import check_min_cppstd
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout, CMakeDeps
from conan.tools.files import rmdir
from conan.tools.microsoft import is_msvc
from conan.tools.scm import Version
import os


class MediapieliteRecipe(ConanFile):
    name = "mediapipelite"
    version = "0.1.0"

    # Optional metadata
    license = "Apache-2.0"
    author = "lzx1413@live.cn"
    url = ""
    description = "cmake version of mediapipe graph"
    topics = ("compute graph", "cpp", "pipeline")
    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "enable_profiler": [True, False],
        "enable_rtti": [True, False],
        'export_package': [True, False]
    }
    default_options = {
        "shared": False,
        "fPIC": True,
        "enable_profiler": False,
        "enable_rtti": True,
        'export_package': False
    }
    exports_sources = "CMakeLists.txt", "mediapipe/*", "cmake/*"

    @property
    def _compilers_minimum_version(self):
        return {
            "gcc": "6",
            "clang": "5",
            "apple-clang": "10",
            "Visual Studio": "15",
            "msvc": "191",
        }

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC
        if self.settings.os == "Linux" and not self.options.export_package:
           self.options["opencv/*"].with_gtk = False

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    @property
    def _min_cppstd(self):
        return "14"

    def validate(self):
        if self.settings.compiler.cppstd:
            check_min_cppstd(self, self._min_cppstd)
        minimum_version = self._compilers_minimum_version.get(
            str(self.settings.compiler), False
        )
        if (
            minimum_version
            and Version(self.settings.compiler.version) < minimum_version
        ):
            raise ConanInvalidConfiguration(
                f"{self.ref} requires C++{self._min_cppstd}, which your compiler does not support."
            )

        if self.options.shared and is_msvc(self):
            # upstream tries its best to export symbols, but it's broken for the moment
            raise ConanInvalidConfiguration(
                f"{self.ref} shared not availabe for Visual Studio (yet)"
            )

    def layout(self):
        cmake_layout(self)
        self.folders.generators = f"build_{self.settings.os}"

    def requirements(self):
        self.requires("abseil/20230125.1", visible=True)
        self.requires("protobuf/3.21.12", visible=True)
        self.requires("glog/0.5.0", visible=True)

    def build_requirements(self):
        if not self.options.export_package:
            self.test_requires("gtest/1.13.0")
            self.test_requires("zlib/1.2.13")
            self.test_requires(
                "opencv/4.5.5",
                options={
                    "with_vulkan": False,
                    "with_ffmpeg": False,
                    "gapi": False,
                    "objdetect": False,
                    "photo": False,
                    "dnn": False,
                    "optflow": True,
                    "ximgproc": True,
                },
            )
            self.test_requires("tensorflow-lite/2.10.0")
            self.test_requires("cpuinfo/cci.20220228")
            self.test_requires("pybind11/2.10.1")
            self.test_requires("stb/cci.20220909")
            self.test_requires("xz_utils/5.4.2")
            self.test_requires("benchmark/1.7.1")
            self.test_requires("libjpeg/9e")
            self.test_requires("eigen/3.4.0")

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["BUILD_TESTS"] = False
        tc.variables["BUILD_PYTHON"] = False
        tc.variables["BUILD_EXAMPLES"] = False
        tc.variables["BUILD_GRAPH_ONLY"] = True
        if self.settings.os == "Android":
            tc.variables["BUILD_PROTO_FILES"] = False
        if self.options.enable_rtti:
            tc.variables["ENABLE_RTTI"] = True
        else:
            tc.variables["ENABLE_RTTI"] = False
        if self.options.enable_profiler:
            tc.variables["ENABLE_PROFILER"] = True
        else:
            tc.variables["ENABLE_PROFILER"] = False
        tc.generate()
        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()
        rmdir(self, os.path.join(self.package_folder, "lib", "pkgconfig"))
        rmdir(self, os.path.join(self.package_folder, "share"))

    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", "mediapipelite")
        self.cpp_info.components["graph"].set_property(
            "cmake_target_name", "mediapipelite::graph"
        )
        self.cpp_info.components["graph"].set_property(
            "cmake_target_aliases", ["mediapipelite::graph"]
        )
        self.cpp_info.components["graph"].set_property(
            "pkg_config_name", "libmediapipelite_graph"
        )
        self.cpp_info.components["graph"].libs = ["graph"]
        self.cpp_info.components["stream_handler"].set_property(
            "cmake_target_name", "mediapipelite::stream_handler"
        )
        self.cpp_info.components["stream_handler"].set_property(
            "cmake_target_aliases", ["mediapipelite::stream_handler"]
        )
        self.cpp_info.components["stream_handler"].set_property(
            "pkg_config_name", "libmediapipelite_stream_handler"
        )
        self.cpp_info.components["stream_handler"].libs = ["stream_handler"]
        self.cpp_info.names["cmake_find_package"] = "mediapipelite"
        self.cpp_info.names["cmake_find_package_multi"] = "mediapipelite"
