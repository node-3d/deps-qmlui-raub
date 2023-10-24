# QmlUi binaries

This is a part of [Node3D](https://github.com/node-3d) project.

[![NPM](https://badge.fury.io/js/deps-qmlui-raub.svg)](https://badge.fury.io/js/deps-qmlui-raub)
[![ESLint](https://github.com/node-3d/deps-qmlui-raub/actions/workflows/eslint.yml/badge.svg)](https://github.com/node-3d/deps-qmlui-raub/actions/workflows/eslint.yml)
[![Test](https://github.com/node-3d/deps-qmlui-raub/actions/workflows/test.yml/badge.svg)](https://github.com/node-3d/deps-qmlui-raub/actions/workflows/test.yml)
[![Cpplint](https://github.com/node-3d/deps-qmlui-raub/actions/workflows/cpplint.yml/badge.svg)](https://github.com/node-3d/deps-qmlui-raub/actions/workflows/cpplint.yml)

```console
npm i -s deps-qmlui-raub
```

**OpenGL QML offscreen** renderer with **non-Qt C++ API**.
Contains a Qt **.pro** file and all the sources.
Binaries are prebuilt and then used as dependency package.

* Platforms (x64): Windows, Linux, OSX.
* Libraries: QmlUi.
* Linking: static dll-type.


Before any import of Qt-dependent module, there should be `require('deps-qt-qml-raub')`.
On Windows it adds Qt's DLL location to ENV PATH.
On Unix, **special** runtime library directories are not in ENV PATH. The paths
to such directories have to be compiled into the node-addon with `rpath` option.

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
            '<(qmlui_bin)/libqmlui.so',
            "-Wl,-rpath,'$$ORIGIN'",
            "-Wl,-rpath,'$$ORIGIN/../node_modules/deps-qt-core-raub/<(bin)'",
            "-Wl,-rpath,'$$ORIGIN/../node_modules/deps-qt-gui-raub/<(bin)'",
            "-Wl,-rpath,'$$ORIGIN/../node_modules/deps-qt-qml-raub/<(bin)'",
            "-Wl,-rpath,'$$ORIGIN/../../deps-qt-core-raub/<(bin)'",
            "-Wl,-rpath,'$$ORIGIN/../../deps-qt-gui-raub/<(bin)'",
            "-Wl,-rpath,'$$ORIGIN/../../deps-qt-qml-raub/<(bin)'",
            '<(qt_core_bin)/libicui18n.so.56',
            '<(qt_core_bin)/libicuuc.so.56',
            '<(qt_core_bin)/libicudata.so.56',
            '<(qt_core_bin)/libicuio.so.56',
            '<(qt_core_bin)/libicule.so.56',
            '<(qt_core_bin)/libicutu.so.56',
            '<(qt_core_bin)/libQt5Core.so.5',
            '<(qt_core_bin)/libQt5Network.so.5',
            '<(qt_core_bin)/libQt5DBus.so.5',
            '<(qt_gui_bin)/libQt5Gui.so.5',
            '<(qt_gui_bin)/libQt5OpenGL.so.5',
            '<(qt_gui_bin)/libQt5Widgets.so.5',
            '<(qt_gui_bin)/libQt5XcbQpa.so.5',
            '<(qt_qml_bin)/libQt5Qml.so.5',
            '<(qt_qml_bin)/libQt5Quick.so.5',
            '<(qt_qml_bin)/libQt5QuickControls2.so.5',
            '<(qt_qml_bin)/libQt5QuickTemplates2.so.5',
            '<(qt_qml_bin)/libQt5QuickWidgets.so.5',
          ],
        }],
        
        ['OS=="mac"', {
          'libraries': [
            '<(qmlui_bin)/libqmlui.dylib',
            '-Wl,-rpath,@loader_path',
            '-Wl,-rpath,@loader_path/../node_modules/deps-qt-core-raub/<(bin)',
            '-Wl,-rpath,@loader_path/../node_modules/deps-qt-gui-raub/<(bin)',
            '-Wl,-rpath,@loader_path/../node_modules/deps-qt-qml-raub/<(bin)',
            '-Wl,-rpath,@loader_path/../../deps-qt-core-raub/<(bin)',
            '-Wl,-rpath,@loader_path/../../deps-qt-gui-raub/<(bin)',
            '-Wl,-rpath,@loader_path/../../deps-qt-qml-raub/<(bin)',
          ],
        }],
        
        ['OS=="win"', {
          'libraries'     : [ '-lqmlui' ],
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
	dlopen("libicui18n.so.56", RTLD_LAZY);
	dlopen("libicuuc.so.56", RTLD_LAZY);
	dlopen("libicudata.so.56", RTLD_LAZY);
	dlopen("libicuio.so.56", RTLD_LAZY);
	dlopen("libicule.so.56", RTLD_LAZY);
	dlopen("libicutu.so.56", RTLD_LAZY);
	dlopen("libQt5Core.so.5", RTLD_LAZY);
	dlopen("libQt5Network.so.5", RTLD_LAZY);
	dlopen("libQt5DBus.so.5", RTLD_LAZY);
	dlopen("libQt5Gui.so.5", RTLD_LAZY);
	dlopen("libQt5OpenGL.so.5", RTLD_LAZY);
	dlopen("libQt5Widgets.so.5", RTLD_LAZY);
	dlopen("libQt5XcbQpa.so.5", RTLD_LAZY);
	dlopen("libQt5Qml.so.5", RTLD_LAZY);
	dlopen("libQt5Quick.so.5", RTLD_LAZY);
	dlopen("libQt5QuickControls2.so.5", RTLD_LAZY);
	dlopen("libQt5QuickTemplates2.so.5", RTLD_LAZY);
	dlopen("libQt5QuickWidgets.so.5", RTLD_LAZY);
	#endif
```

</details>

Static`QmlUi::init(const char *cwdOwn, size_t wnd, size_t ctx, QmlUi::Cb cb)`
must be called before any other method.
Then instances of `QmlUi` can be created and operated on. Each such
an instance represents a hidden `QWindow` being
[rendered to texture](http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/).


## Class QmlUi

`QmlUi` is a facade class for all operations. Instances represent separate QML scenes,
each having a dedicated OpenGL framebuffer.

### Constructor

* `QmlUi(w, h)`
	* `int w` - initial width.
	* `int h` - initial height.


### Static Methods:

* `void init(cwdOwn, wnd, ctx, cb)`
	
	Initialize the renderer. Should be called once, before any instance is created.
	Extra calls are ignored.
	* `const char *cwdOwn` - "current working directory" for QML.
	* `size_t wnd` - platform window handle.
	* `size_t ctx` - platform OpenGL context handle.
	* `QmlUi::Cb cb` - callback for all events.
	`QmlUi::Cb` = `void (*) (QmlUi *target, const char *type, const char *json)`.
	This callback will later receive all the asynchronous events, for all the instances.


* `void plugins(path)`
	
	Add more directories for `QmlEngine` to look for plugins.
	* `const char *path` - directory path.


* `void update()`
	
	Poll window events. Required for async operations, including signal/slot interaction.
	If this method is not called at all, the QML scene won't ever load. It is preferred
	to call it regularly.
	> NOTE: It is at this point, that the QML render may happen, causing GL context
	switch. Make sure to return your original GL context (if any), after calling this.


* `void style(name, fallback)`
	
	Set the QML [style](https://doc.qt.io/qt-5/qquickstyle.html#setStyle).
	Optionally set a fallback style.
	* `const char *name` - the name of the style to be [used](https://doc.qt.io/qt-5/qtquickcontrols2-styles.html#using-styles-in-qt-quick-controls).
	* `const char *fallback` - the name of the fallback style, or `nullptr`.


### Methods:

* `void resize(w, h)`
	
	Change the scene size. It induces async recreation of the framebuffer.
	* `int w` - new width.
	* `int h` - new height.


* `void mouse(type, button, buttons, x, y)`
	
	Propagate a mouse event to the QML scene. If the event can't be handled
	by the scene, it gets re-emitted through the callback, named '\_mouse'.
	Re-emitted format is
	[web MouseEvent](https://developer.mozilla.org/en-US/docs/Web/API/MouseEvent).
	* `int type` - event type:
		* `MouseMove` = 0
		* `MouseButtonPress` = 1
		* `MouseButtonRelease` = 2
	* `int button` - button id:
		* `LeftButton` = 1
		* `RightButton` = 2
		* `MiddleButton` = 4
	* `int buttons` - currently pressed buttons, in bitwise "or".
	* `int x` - horizontal mouse position.
	* `int y` - vertical mouse position.


* `void keyboard(type, key, text)`
	
	Propagate a keyboard event to the QML scene. If the event can't be handled
	by the scene, it gets re-emitted through the callback, named '\_key'.
	Re-emitted format is
	[web KeyboardEvent](https://developer.mozilla.org/en-US/docs/Web/API/KeyboardEvent).
	* `int type` - event type:
		* `KeyRelease` = 0
		* `KeyPress` = 1
	* `int key` - key id.
	* `char text` - input text.


* `void load(str, isFile)`
	
	Set content of the QML scene with a file or a string.
	* `const char *str` - if `isFile == true`, `str` is taken as a path to the
	**.qml** file to be loaded. Otherwise, `str` itself is interpreted as QML
	source.
	* `bool isFile` - tells if `src` is a path to **.qml** file.
	
	This operation is asynchronous. An event of type `'_qml_load'` will be emited, when
	loading a QML scene is finished. Until then, QML methods and properties will
	return `[null]` JSON.


* `std::string get(obj, prop)`
	
	Get a property of some object in the QML scene.
	* `const char *obj` - name of the object, as in `Item { objectName: "my-name" }`.
	* `const char *prop` - property key, as in `Item { property var someProp: 10 }`.
	
	The property will be returned as a string containing a JSON array. The only
	element in this array is the value of the property. If anything went wrong
	the JSON will be `[null]`.
	
	Example: `Item { objectName: "my-name", property var x: 10 }`, to get `x`,
	call `std:string json = ui.get("my-name", "x"); // "[10]"`.
	
	> NOTE: properties can't be fetched (returns `[null]` JSON)
	until `'_qml_load'` event is fired.


* `void set(obj, prop, json)`
	
	Set a property of some object in the QML scene.
	* `const char *obj` - name of the object, as in `Item { objectName: "my-name" }`.
	* `const char *prop` - property key, as in `Item { property var someProp: 10 }`.
	* `const char *json` - the value to be set. Must be a JSON array, i.e. enclosed in "[]".
	
	Example: `Item { objectName: "my-name", property var x: 10 }`, to make `x`
	become `11`, call `ui.set("my-name", "x", "[11]")`.
	
	> NOTE: properties can't be set (does nothing) until `'_qml_load'` event is fired.


* `std::string invoke(obj, method, json)`
	
	Invoke a method of some object in the QML scene.
	* `const char *obj` - name of the object, as in `Item { objectName: "my-name" }`.
	* `const char *method` - method key, as in `Item { function f() { return 10; } }`.
	* `const char *json` - arguments to be passed. Must be a JSON array, up to 10 elements.
	
	The result will be returned as a string containing a JSON array. The only
	element in this array is the return value of the function. If anything went wrong
	the JSON will be `[null]`.
	
	Example: `Item { objectName: "my-name", function f() { return 10; } }`, to call `f`,
	call `std:string json = ui.invoke("my-name", "f", "[]"); // "[10]"`.
	
	> NOTE: `invoke()` will look for a function receiving the same number of arguments
	you pass within the JSON array. So these must match, otherwise the call will fail.
	
	> NOTE: methods can't be invoked (returns `[null]` JSON, does nothing)
	until `'_qml_load'` event is fired.


* `void libs(path)`
	
	Register a directory where additional QML search should be performed when
	importing a component. Better do it before the `load()`
	* `const char *path` - the directory path.
