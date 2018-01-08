# qmlui

Offscreen OpenGL Qt QML renderer with C-way API.
Contains a Qt project. Binaries are prebuilt and then used as dependency.


## Install

`npm i -s node-deps-qmlui-raub`


## Use

**binding.gyp**

```javascript
	'variables': {
		'qmlui_include' : '<!(node -e "console.log(require(\'node-deps-qmlui-raub\').include)")',
		'qmlui_bin'     : '<!(node -e "console.log(require(\'node-deps-qmlui-raub\').bin)")',
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
				
				['OS=="linux"', {
					'libraries': [
						'-Wl,-rpath,<(qmlui_bin)',
					],
				}],
				
				['OS=="mac"', {
					'libraries': [
						'-Wl,-rpath,<(qmlui_bin)',
				}],
				
				['OS=="win"', {
					
				}],
				
			],
		},
```


**addon.cpp**

```cpp
#include <qmlui.hpp>
```
