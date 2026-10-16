from conan import ConanFile
from conan.tools.cmake import cmake_layout

class OpenCVProjectConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires("opencv/4.14.0")

    # Ця секція була зроблена через АI щоб полегшити білд під Windows
    def configure(self):
        # Оптимізація та відключення важких / непотрібних модулів OpenCV
        # Це рятує від збирання зайвих залежностей (protobuf, abseil тощо)
        opencv_options = self.options["opencv"]

        opencv_options.gapi = False
        opencv_options.shared = False              # Статична лінківка (або True, якщо потрібні DLL)
        opencv_options.dnn = False                 # Вимикаємо модуль нейромереж ( DNN затягує Protobuf )
        opencv_options.calib3d = True              # Залишаємо калібрування (часто потрібно)
        opencv_options.features2d = True           # Особливості / детектори (ORB, SIFT тощо)
        opencv_options.highgui = True              # Робота з вікнами (imshow)
        opencv_options.imgcodecs = True            # Читання/запись зображень (PNG, JPG)
        opencv_options.imgproc = True              # Обробка зображень (фільтри, маски)
        opencv_options.videoio = False             # Вимикаємо важкий відеозахват/кодеки (FFmpeg, GStreamer), якщо потрібні лише картинки
        
        # Вимикаємо непотрібні сторонні залежності та прикладні модулі
        opencv_options.with_cuda = False           # CUDA не потрібна для звичайного процесора
        opencv_options.with_eigen = True           # Легкі матричні операції
        opencv_options.with_tiff = False
        opencv_options.with_webp = False
        opencv_options.with_openexr = False
        opencv_options.with_quirc = False          # Вимикає детекцію QR-кодів (пришвидшує збірку)

    def layout(self):
        cmake_layout(self)