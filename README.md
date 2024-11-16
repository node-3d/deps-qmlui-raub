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
Uses [QQuickRenderControl](https://doc.qt.io/qt-6/qquickrendercontrol.html) to render QML
scenes to textures. The QML renderer has a separate OpenGL context, so
[resource sharing](https://www.glfw.org/docs/3.4/context_guide.html#context_sharing) is used.

Binaries are prebuilt and then used as dependency package.

* Platforms (x64): Windows, Linux, Linux ARM, MacOS ARM.
* Library: QmlUi.
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
  dlopen("libicui18n.so.73", RTLD_NOW | RTLD_GLOBAL);
  dlopen("libicuuc.so.73", RTLD_NOW | RTLD_GLOBAL);
  dlopen("libicudata.so.73", RTLD_NOW | RTLD_GLOBAL);
  dlopen("libicuio.so.73", RTLD_NOW | RTLD_GLOBAL);
  dlopen("libicule.so.73", RTLD_NOW | RTLD_GLOBAL);
  dlopen("libicutu.so.73", RTLD_NOW | RTLD_GLOBAL);
  dlopen("libQt6Core.so.6", RTLD_NOW | RTLD_GLOBAL);
  dlopen("libQt6Network.so.6", RTLD_NOW | RTLD_GLOBAL);
  dlopen("libQt6DBus.so.6", RTLD_NOW | RTLD_GLOBAL);
  dlopen("libQt6Gui.so.6", RTLD_NOW | RTLD_GLOBAL);
  dlopen("libQt6OpenGL.so.6", RTLD_NOW | RTLD_GLOBAL);
  dlopen("libQt6Widgets.so.6", RTLD_NOW | RTLD_GLOBAL);
  dlopen("libQt6XcbQpa.so.6", RTLD_NOW | RTLD_GLOBAL);
  dlopen("libQt6Qml.so.6", RTLD_NOW | RTLD_GLOBAL);
  dlopen("libQt6Quick.so.6", RTLD_NOW | RTLD_GLOBAL);
  dlopen("libQt6QuickControls2.so.6", RTLD_NOW | RTLD_GLOBAL);
  dlopen("libQt6QuickTemplates2.so.6", RTLD_NOW | RTLD_GLOBAL);
  dlopen("libQt6QuickWidgets.so.6", RTLD_NOW | RTLD_GLOBAL);
  dlopen("libqmlui.so", RTLD_NOW | RTLD_GLOBAL);
  #endif
```

</details>


## Legal notice

This software uses the [Qt library](https://www.qt.io/).
Qt is legally used under the LGPLv3 (GNU Lesser General Public License) version.
It is [explicitly stated](https://www.qt.io/licensing/open-source-lgpl-obligations)
that Qt Libraries can be used in a commercial closed-source app (if you wish):

> In case of dynamic linking, it is possible, but not mandatory,
to keep application source code proprietary as long as it is
“work that uses the library” – typically achieved
via dynamic linking of the library.

These **terms and conditions** allow using (unmodified) Qt as a
shared library (DLL), in a closed-source project.

Qt licensing information (a COPY) is given in a [separate file](/QT_LGPL),
which also can be found on
[Qt's official web-site](http://doc.qt.io/qt-6/lgpl.html).

---

The rest of this package is MIT licensed.
