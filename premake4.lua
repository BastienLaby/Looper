solution "TablaturnSolution"
   configurations { "Debug", "Release" }
   platforms {"native", "x64", "x32"}

   -- Tablaturn
   project "Tablaturn"
      kind "ConsoleApp"
      language "C++"
      files { "src/**" }
      includedirs { "src", "common", "lib/fmod/inc", "lib/opencv2/include", "include/**", "include"}
      libdirs { "lib/**" }

      configuration { "linux" }
         links {"X11","Xrandr", "rt", "pthread", "fmodex64", "SDL", "SDL_image", "SDL_ttf",  "opencv_calib3d",
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
