# QmlUi binaries

This is a part of [Node3D](https://github.com/node-3d) project.


## Synopsis

Offscreen OpenGL Qt QML renderer with C-way API.
Contains a Qt project. Binaries are prebuilt and then used as dependencies.

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




`Window` is higher level js-wrapper around the above functions, which helps in managing window
instances. It basically has all the functionality where in GLFW Docs `window` parameter
is mentioned. E.g. `glfwSetWindowTitle(window, title)` -> `window.title = title`.

There are few simple rules for the above transformation to become intuitive:

* API is available if it has `window` parameter.
* All props start lowercase.
* Word "Window" is omitted.
* Whatever could have a `get/set` interface is made so.


Constructor:

* `Window({ title, width, height, display, vsync, mode, autoIconify, msaa })`
	* `string title $PWD` - window title, takes current directory as default.
	* `number width 800` - window initial width.
	* `number height 600` - window initial height.
	* `number display undefined` - display id to open window on a specific display.
	* `boolean vsync false` - if vsync should be used.
	* `string mode 'windowed'` - one of `'windowed', 'borderless', 'fullscreen'`.
	* `boolean autoIconify true` - if fullscreen windows should iconify automatically on focus loss.
	* `number msaa 2` - multisample antialiasing level.
	* `boolean decorated true` - if window has borders (use `false` for borderless fullscreen).


Properties:

* `get number handle` - window pointer.
* `get string version` - OpenGL vendor info.
* `get number platformWindow` - window HWND pointer.
* `get number platformContext` - OpenGL context handle.
* `get {width, height} framebufferSize` - the size of allocated framebuffer.
* `get number currentContext` - what GLFW window is now current.
* `get number samples` - number of msaa samples passed to the constructor.

* `get/set string mode` - one of `'windowed', 'borderless', 'fullscreen'`. Here
`'borderless'` emulates fullscreen by a frameless, screen-sized window.
This when this property is changed, a new window is created and the old is hidden.
* `get/set number width|w` - window width.
* `get/set number height|h` - window height.
* `get/set [width, height] wh` - window width and height.
* `get/set {width, height} size` - window width and height.
* `get/set string title` - window title.
* `get/set {width, height, Buffer data} icon` - window icon in RGBA format. Consider
using [this Image implementation](https://github.com/raub/node-image).
* `get/set boolean shouldClose` - if window is going to be closed.
* `get/set number x` - window position X-coordinate on the screen.
* `get/set number y` - window position Y-coordinate on the screen.
* `get/set {x, y} pos` - where window is on the screen.
* `get/set {x, y} cursorPos` - where mouse is relative to the window.

---

Methods:

* `getKey(number key)` - `glfw.getKey(window, key)`.
* `getMouseButton(number button)` - `glfw.getMouseButton(window, button)`.
* `getWindowAttrib(number attrib)` - `glfw.getWindowAttrib(window, attrib)`.
* `setInputMode(number mode)` - `glfw.setInputMode(window, mode)`.
* `swapBuffers()` - `glfw.swapBuffers(window)`.
* `makeCurrent()` - `glfw.makeContextCurrent(window)`.
* `destroy()` - `glfw.destroyWindow(window)`.
* `iconify()` - `glfw.iconifyWindow(window)`.
* `restore()` - `glfw.restoreWindow(window)`.
* `hide()` - `glfw.hideWindow(window)`.
* `show()` - `glfw.showWindow(window)`.
* `on(string type, function cb)` - listen for window (GLFW) events.
