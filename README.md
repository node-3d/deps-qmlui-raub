# Bullet binaries

* Platforms: win x32/x64, linux x32/x64, mac x64.
* Library: QmlUi.
* Linking: static dll-type.

Offscreen OpenGL Qt QML renderer with C-way API.
Contains a Qt project. Binaries are prebuilt and then used as dependencies.


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
#include <qmlui.hpp>
```
