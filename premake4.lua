solution "TablaturnSolution"
   configurations { "Debug", "Release" }
   platforms {"native", "x64", "x32"}

   -- Tablaturn
   project "Looper"
      kind "ConsoleApp"
      language "C++"
      files { "src/**" }
      includedirs { "src", "common", "include", "lib/", "lib/opencv2/include", "lib/glfw/include"}
      libdirs { "lib/opencv2/lib/" }
      links {"glfw", "glew"}
      defines { "GLEW_STATIC" }

      configuration { "linux" }
         links {"X11","Xrandr", "rt", "GL", "GLU", "pthread", "fmodex64", "SDL", "opencv_calib3d",
                                                                                 "opencv_contrib",
                                                                                 "opencv_core",
                                                                                 "opencv_features2d",
                                                                                 "opencv_flann",
                                                                                 "opencv_gpu",
                                                                                 "opencv_highgui",
                                                                                 "opencv_imgproc",
                                                                                 "opencv_legacy",
                                                                                 "opencv_ml",
                                                                                 "opencv_nonfree",
                                                                                 "opencv_objdetect",
                                                                                 "opencv_ocl",
                                                                                 "opencv_photo",
                                                                                 "opencv_stitching",
                                                                                 "opencv_superres",
                                                                                 "opencv_video",
                                                                                 "opencv_videostab"}

      configuration "Debug"
         defines { "DEBUG" }
         flags {"ExtraWarnings", "Symbols" }
         targetsuffix "d"

      configuration "Release"
         defines { "NDEBUG" }
         flags { "Optimize"}    
         targetsuffix "r"

   -- GLFW Library
   project "glfw"
      kind "StaticLib"
      language "C"
      files { "lib/glfw/lib/*.h", "lib/glfw/lib/*.c", "lib/glfw/include/GL/glfw.h" }
      includedirs { "lib/glfw/lib", "lib/glfw/include"}

      configuration {"linux"}
         files { "lib/glfw/lib/x11/*.c", "lib/glfw/x11/*.h" }
         includedirs { "lib/glfw/lib/x11" }
         defines { "_GLFW_USE_LINUX_JOYSTICKS", "_GLFW_HAS_XRANDR", "_GLFW_HAS_PTHREAD" ,"_GLFW_HAS_SCHED_YIELD", "_GLFW_HAS_GLXGETPROCADDRESS" }
         buildoptions { "-pthread" }

      configuration "Debug"
         defines { "DEBUG" }
         flags { "Symbols" }
         targetdir "bin/debug"

      configuration "Release"
         defines { "NDEBUG" }
         flags { "Optimize" }    
         targetdir "bin/release"

   -- GLEW Library         
   project "glew"
      kind "StaticLib"
      language "C"
      files {"lib/glew/*.c", "lib/glew/*.h"}
      defines { "GLEW_STATIC" }

      configuration "Debug"
         defines { "DEBUG" }
         flags { "Symbols" }
         targetdir "bin/debug"

      configuration "Release"
         defines { "NDEBUG" }
         flags { "Optimize" }    
         targetdir "bin/release"
