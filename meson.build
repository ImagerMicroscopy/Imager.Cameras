project('SCCameraPlugin', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++20', 'b_vscrt=static_from_buildtype']
)

# --- USER SETTINGS ---
# Change this variable to customize your plugin's name
plugin_name = 'SCCamera'
# ---------------------

# Enforce 64-bit build
if host_machine.cpu_family() != 'x86_64'
  error('This plugin must be built for 64-bit architecture. Please configure your build system for x64.')
endif

# Ensure the equipment name macro is available to all subprojects (especially ImagerPluginCore)
add_global_arguments('-DIMAGER_EQUIPMENT_NAME="' + plugin_name + '"', language : 'cpp')

# Fetch the core Imager plugin support files
imager_core_dep = dependency('imagerplugincore')

# Fetch dependencies
json_dep = dependency('nlohmann_json')
queue_dep = dependency('concurrentqueue')

sdk_inc = include_directories('./include')

# Define the shared library (DLL) target for the plugin
shared_library(plugin_name,

  # Add your plugin source files here
  'src/AndorSDK3Camera.cpp',
  'src/AndorSDK3CameraHandler.cpp',
  'src/CameraManager.cpp',
  'src/DummyCamera.cpp',
  'src/HamamatsuCamera.cpp',
  'src/HamamatsuCameraHandler.cpp',
  'src/IDSPeakCamera.cpp',
  'src/IDSPeakCameraHandler.cpp',
  'src/OceanOpticsCamera.cpp',
  'src/PCOCamera.cpp',
  'src/PhotometricsCamera.cpp',
  'src/PhotometricsCameraHandler.cpp',
  'src/SCCameraPlugin.cpp',
  'src/SCPrinter.cpp',
  'src/SCConfigurationFile.cpp',

  include_directories : [sdk_inc, include_directories('src')],
  dependencies : [json_dep, queue_dep, imager_core_dep],
  name_suffix : 'imagerplugin', # Change the output file extension
  cpp_args : ['-DNOMINMAX'],
  )
