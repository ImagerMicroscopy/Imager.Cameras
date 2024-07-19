project('src', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3',
                     'cpp_std=c++17',
		     'default_library=shared',
		    ]
)

incdirs = include_directories(
                ['../include',
                 '../include/IPP2017'
                ]
           )


abslibdir = join_paths(meson.source_root(), '../lib/win64')

cc = meson.get_compiler('cpp')
libippcoremt = cc.find_library('ippcoremt', dirs:[abslibdir])
libippimt = cc.find_library('ippimt', dirs:[abslibdir])
libippsmt = cc.find_library('ippsmt', dirs:[abslibdir])
libippippvmmt = cc.find_library('ippvmmt', dirs:[abslibdir])

library('SCCamera',
        'AndorCamera.cpp',
        'BaseCameraClass.cpp',
        'CameraManager.cpp',
        'CameraPropertiesEncoding.cpp',
        'DummyCamera.cpp',
        'HamamatsuCamera.cpp',
        'HamamatsuCameraHandler.cpp',
        'IDSCamera.cpp',
        'ImageProcessingUtils.cpp',
        'ImageRecycler.cpp',
        'OceanOpticsCamera.cpp',
        'PCOCamera.cpp',
        'PhotometricsCamera.cpp',
        'PhotometricsCameraHandler.cpp',
        'SCCameraDLL.cpp',
	include_directories : incdirs,
        dependencies: [libippcoremt, libippimt, libippsmt, libippippvmmt],
        cpp_args : '-DNOMINMAX',
        install : true)
