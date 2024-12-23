project('src', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3',
                     'cpp_std=c++17',
		     'default_library=shared',
		    ]
)

incdirs = include_directories(
                ['../include',
                 '../include/IPP2017',
                 '../include/OceanOpticsSeabreeze'
                ]
          )


abslibdir = join_paths(meson.source_root(), '../lib/win64')

cc = meson.get_compiler('cpp')
libs = []
foreach lib : ['ippcoremt', 'ippimt', 'ippsmt', 'ippvmmt', 'dcamapi',
               'ids_peak_comfort_c', 'SC2_Cam', 'uEye_api_64', 'pvcam64','SeaBreeze',
               'atmcd64m', 'atutilitym', 'atcorem'
              ]
        libs += cc.find_library(lib, dirs:[abslibdir])
endforeach

library('SCCamera',
        'AndorCamera.cpp',
        'AndorSDK3Camera.cpp',
        'AndorSDK3CameraHandler.cpp',
        'BaseCameraClass.cpp',
        'CameraManager.cpp',
        'CameraPropertiesEncoding.cpp',
        'DummyCamera.cpp',
        'HamamatsuCamera.cpp',
        'HamamatsuCameraHandler.cpp',
        'IDSCamera.cpp',
        'IDSPeakCamera.cpp',
        'IDSPeakCameraHandler.cpp',
        'ImageProcessingUtils.cpp',
        'ImageRecycler.cpp',
        'OceanOpticsCamera.cpp',
        'PCOCamera.cpp',
        'PhotometricsCamera.cpp',
        'PhotometricsCameraHandler.cpp',
        'SCCameraDLL.cpp',
        'Utils.cpp',
	include_directories : incdirs,
        dependencies: libs,
        cpp_args : '-DNOMINMAX',
        install : true)
