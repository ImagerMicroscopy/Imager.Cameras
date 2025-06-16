project('src', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3',
                     'cpp_std=c++20',
		             'default_library=shared',
		    ]
)

isWindows = build_machine.system() == 'windows'

if (isWindows)
        incdirs = include_directories(
                        ['../include',
                        '../include/IPP2017',
                        '../include/OceanOpticsSeabreeze'
                        ]
                )

        abslibdir = join_paths(meson.source_root(), '../lib/win64')

        libNames = ['ippcoremt', 'ippimt', 'ippsmt', 'ippvmmt',
                    'ids_peak_comfort_c', 'uEye_api_64','SeaBreeze',
                    'atmcd64m']
else
        incdirs = include_directories([
                        '../include',
                        '/opt/intel/oneapi/ipp/latest/include/ipp'
                        ])

        abslibdir = '/opt/intel/oneapi/ipp/latest/lib'

        libNames = ['ippcore', 'ippi', 'ipps']
endif

cc = meson.get_compiler('cpp')
libs = []
foreach lib : libNames
        libs += cc.find_library(lib, dirs: [abslibdir], static: true)
endforeach

nlohmann_json_dep = dependency('nlohmann_json')

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
        'SCPrinter.cpp',
        'Utils.cpp',
	include_directories : incdirs,
        dependencies: [libs, nlohmann_json_dep],
        cpp_args : '-DNOMINMAX',
        install : true)
