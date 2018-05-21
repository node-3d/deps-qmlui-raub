# QmlUi binaries

This is a part of [Node3D](https://github.com/node-3d) project.


## Synopsis

**OpenGL QML offscreen** renderer with **non-Qt C++ API**.
Contains a Qt **.pro** file and all the sources.
Binaries are prebuilt and then used as dependency package.

* Platforms: win x32/x64, linux x32/x64, mac x64.
* Library: QmlUi.
* Linking: static dll-type.


## Install

`npm i -s deps-qmlui-raub`


## Usage

**binding.gyp**

```javascript
	'variables': {
		'qmlui_include' : '<!(node -e "require(\'deps-qmlui-raub\').include()")',
		'qmlui_bin'     : '<!(node -e "require(\'deps-qmlui-raub\').bin()")',
	},
	...
	'targets': [
		{
			'target_name': '...',
			
			'include_dirs': [
				'<(qmlui_include)',
				...
			],
			
			'library_dirs': [ '<(qmlui_bin)' ],
			'libraries'    : [ '-lqmlui' ],
			
			'conditions': [
				
				['OS=="linux" or OS=="mac"', {
					'libraries': [
						'-Wl,-rpath,<(qmlui_bin)',
					],
				}],
				
			],
		},
```


**addon.cpp**

```cpp
#include <qml-ui.hpp>
```


## Class QmlUi

`QmlUi` is a facade class for all operations. Instances represent separate QML scenes,
each having a dedicated OpenGL framebuffer.

### Constructor:

* `QmlUi(w, h)`
	* `int w` - initial width.
	* `int h` - initial height.


### Types:

`QmlUi::Cb` - `void (*) (QmlUi *target, const char *type, const char *json)`


### Static Methods:

* `void init(cwdOwn, wnd, ctx, cb)`
	Initialize the renderer. Should be called once, before any instance is created.
	Extra calls are ignored.
	* `const char *cwdOwn` - "current working directory" for QML.
	* `size_t wnd` - platform window handle.
	* `size_t ctx` - platform OpenGL context handle.
	* `QmlUi::Cb cb` - callback for all events.


* `void plugins(path)`
	Add more directories for `QmlEngine` to look for plugins.
	* `const char *path` - directory path.


* `void update()`
	Poll window events. Required for async operations, including signal/slot interaction.


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


* `void set(obj, prop, json)`
	Set a property of some object in the QML scene.
	* `const char *obj` - name of the object, as in `Item { objectName: "my-name" }`.
	* `const char *prop` - property key, as in `Item { property var someProp: 10 }`.
	* `const char *json` - the value to be set. Note: values "`10`" and "`'10'`"
	are different here. See output of `JSON.stringify(val)`.


* `void get(obj, prop)`
	Get a property of some object in the QML scene.
	* `const char *obj` - name of the object, as in `Item { objectName: "my-name" }`.
	* `const char *prop` - property key, as in `Item { property var someProp: 10 }`.


* `void invoke(obj, method, json)`
	Invoke a method of some object in the QML scene.
	* `const char *obj` - name of the object, as in `Item { objectName: "my-name" }`.
	* `const char *method` - method key, as in `Item { function f() { return 10; } }`.
	* `const char *json` - the argument to be passed. Only **1** argument is supported,
	but it can be an array or an object with any number of fileds.


* `void libs(path)`
	Register a directory where additional QML search should be performed when
	importing a component.
	* `const char *path` - the directory path.
