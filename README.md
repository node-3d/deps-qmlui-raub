# QmlUi binaries

This is a part of [Node3D](https://github.com/node-3d) project.

[![NPM](https://badge.fury.io/js/deps-qmlui-raub.svg)](https://badge.fury.io/js/deps-qmlui-raub)
[![ESLint](https://github.com/node-3d/deps-qmlui-raub/actions/workflows/eslint.yml/badge.svg)](https://github.com/node-3d/deps-qmlui-raub/actions/workflows/eslint.yml)
[![Test](https://github.com/node-3d/deps-qmlui-raub/actions/workflows/test.yml/badge.svg)](https://github.com/node-3d/deps-qmlui-raub/actions/workflows/test.yml)
[![Cpplint](https://github.com/node-3d/deps-qmlui-raub/actions/workflows/cpplint.yml/badge.svg)](https://github.com/node-3d/deps-qmlui-raub/actions/workflows/cpplint.yml)
[![Build](https://github.com/node-3d/deps-qmlui-raub/actions/workflows/build.yml/badge.svg)](https://github.com/node-3d/deps-qmlui-raub/actions/workflows/build.yml)

```console
npm i -s deps-qmlui-raub
```

**OpenGL QML offscreen** renderer with **non-Qt C++ API**.
Uses [QQuickRenderControl](https://doc.qt.io/qt-5/qquickrendercontrol.html) to render QML
scenes to textures. The QML renderer has a separate OpenGL context, so
[resource sharing](https://www.glfw.org/docs/3.3/context_guide.html#context_sharing) is used.

Binaries are prebuilt and then used as dependency package.

* Platforms (x64): Windows, Linux, OSX.
* Libraries: QmlUi.
* Linking: static dll-type.

## Common workflow

1. Create a window and initialize OpenGL context.
1. Call `QmlUi::init2(...)` to create the QML rendering surface (and its shared OpenGL context).
1. Instantiate `QmlUi` and load a QML scene.
1. Receive an OpenGL texture ID and use it somehow.
1. Send/receive input and other events.

See [QmlUi header](/include/qml-ui.hpp) for more details.


## Building addons

Requiring this module on Windows - `require('deps-qmlui-raub')` - adds Qt's DLL
location to ENV PATH. On Unix this does nothing, as library directories are not in ENV PATH.
The paths for Unix have to be compiled into the node-addon with `rpath` option.

<details>

<summary><b>binding.gyp</b></summary>

```javascript
  'variables': {
    'qt_core_bin': '<!(node -p "require(\'deps-qmlui-raub\').core.bin")',
    'qt_gui_bin': '<!(node -p "require(\'deps-qmlui-raub\').gui.bin")',
    'qt_qml_bin': '<!(node -p "require(\'deps-qmlui-raub\').qml.bin")',
    'qmlui_include': '<!(node -p "require(\'deps-qmlui-raub\').include")',
    'qmlui_bin': '<!(node -p "require(\'deps-qmlui-raub\').bin")',
  },
  ...
  'targets': [
    {
      'target_name': '...',
      
      'include_dirs': [
        '<(qmlui_include)',
      ],
      
      'library_dirs': [ '<(qmlui_bin)' ],
      'libraries'    : [ '-lqmlui' ],
      
      'conditions': [
        
        ['OS=="linux"', {
          'libraries': [
            "-Wl,-rpath,'$$ORIGIN'",
            "-Wl,-rpath,'$$ORIGIN/../node_modules/deps-qt-core-raub/bin-linux'",
            "-Wl,-rpath,'$$ORIGIN/../node_modules/deps-qt-gui-raub/bin-linux'",
            "-Wl,-rpath,'$$ORIGIN/../node_modules/deps-qt-qml-raub/bin-linux'",
            "-Wl,-rpath,'$$ORIGIN/../node_modules/deps-qmlui-raub/bin-linux'",
            "-Wl,-rpath,'$$ORIGIN/../../deps-qt-core-raub/bin-linux'",
            "-Wl,-rpath,'$$ORIGIN/../../deps-qt-gui-raub/bin-linux'",
            "-Wl,-rpath,'$$ORIGIN/../../deps-qt-qml-raub/bin-linux'",
            "-Wl,-rpath,'$$ORIGIN/../../deps-qmlui-raub/bin-linux'",
          ],
        }],
        
        ['OS=="mac"', {
          'libraries': [
            '<(qmlui_bin)/libqmlui.dylib',
            '-Wl,-rpath,@loader_path',
            '-Wl,-rpath,@loader_path/../node_modules/deps-qt-core-raub/bin-osx',
            '-Wl,-rpath,@loader_path/../node_modules/deps-qt-gui-raub/bin-osx',
            '-Wl,-rpath,@loader_path/../node_modules/deps-qt-qml-raub/bin-osx',
            '-Wl,-rpath,@loader_path/../node_modules/deps-qmlui-raub/bin-osx',
            '-Wl,-rpath,@loader_path/../../deps-qt-core-raub/bin-osx',
            '-Wl,-rpath,@loader_path/../../deps-qt-gui-raub/bin-osx',
            '-Wl,-rpath,@loader_path/../../deps-qt-qml-raub/bin-osx',
            '-Wl,-rpath,@loader_path/../../deps-qmlui-raub/bin-osx',
          ],
        }],
        
        ['OS=="win"', {
          'libraries': [ '-lqmlui' ],
        }],
        
      ],
    },
```

</details>

<details>

<summary><b>Preload libraries</b></summary>

```cpp
#ifdef __linux__
  #include <dlfcn.h>
#endif

  // ... inside some kind of init() function
  #ifdef __linux__
  dlopen("libicui18n.so.56", RTLD_NOW | RTLD_GLOBAL);
  dlopen("libicuuc.so.56", RTLD_NOW | RTLD_GLOBAL);
  dlopen("libicudata.so.56", RTLD_NOW | RTLD_GLOBAL);
  dlopen("libicuio.so.56", RTLD_NOW | RTLD_GLOBAL);
  dlopen("libicule.so.56", RTLD_NOW | RTLD_GLOBAL);
  dlopen("libicutu.so.56", RTLD_NOW | RTLD_GLOBAL);
  dlopen("libQt5Core.so.5", RTLD_NOW | RTLD_GLOBAL);
  dlopen("libQt5Network.so.5", RTLD_NOW | RTLD_GLOBAL);
  dlopen("libQt5DBus.so.5", RTLD_NOW | RTLD_GLOBAL);
  dlopen("libQt5Gui.so.5", RTLD_NOW | RTLD_GLOBAL);
  dlopen("libQt5OpenGL.so.5", RTLD_NOW | RTLD_GLOBAL);
  dlopen("libQt5Widgets.so.5", RTLD_NOW | RTLD_GLOBAL);
  dlopen("libQt5XcbQpa.so.5", RTLD_NOW | RTLD_GLOBAL);
  dlopen("libQt5Qml.so.5", RTLD_NOW | RTLD_GLOBAL);
  dlopen("libQt5Quick.so.5", RTLD_NOW | RTLD_GLOBAL);
  dlopen("libQt5QuickControls2.so.5", RTLD_NOW | RTLD_GLOBAL);
  dlopen("libQt5QuickTemplates2.so.5", RTLD_NOW | RTLD_GLOBAL);
  dlopen("libQt5QuickWidgets.so.5", RTLD_NOW | RTLD_GLOBAL);
  dlopen("libqmlui.so", RTLD_NOW | RTLD_GLOBAL);
  #endif
```

</details>
