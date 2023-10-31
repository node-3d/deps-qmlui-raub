#ifndef QML_UI_HPP
#define QML_UI_HPP

#ifdef WIN32
	#if defined QMLUI_SHARED
		#define QMLUI_DLLSPEC __declspec(dllexport)
	#else
		#define QMLUI_DLLSPEC __declspec(dllimport)
	#endif
#else
	#define QMLUI_DLLSPEC
#endif

#include <string>

class QmlView;
class QmlCb;

/**
 * @brief QML Scene Container.
 * 
 * @details Instances represent separate QML scenes, each having a dedicated OpenGL framebuffer.
 * 
 * @note Make sure to always switch back to your own OpenGL context before drawing.
 */
class QMLUI_DLLSPEC QmlUi {
	
public:
	typedef void (*Cb) (QmlUi *target, const char *type, const char *json);
	
	/**
	 * @brief Initialize the QML renderer.
	 * 
	 * @details Should be called once, before any instance is created.
	 * Extra calls are ignored.
	 * 
	 * @param cwdOwn "current working directory" for QML.
	 * @param wnd platform window handle.
	 * @param ctx platform OpenGL context handle.
	 * @param device platform device handle. On Linux this is `Display *`, otherwise ignored.
	 * @param cb common callback for all events.
	 */
	static void init2(const char *cwdOwn, size_t wnd, size_t ctx, size_t device, QmlUi::Cb cb);
	
	/**
	 * @brief Add directory for `QmlEngine` to look for plugins.
	 * 
	 * @param path directory path
	 */
	static void plugins(const char *path);
	
	/**
	 * @brief Poll window events.
	 * 
	 * @details Required for async operations, such as signal/slot interaction.
	 * If this method is not called at all, the QML scene won't ever load. It is preferred
	 * to call it regularly, e.g. every frame.
	 */
	static void update();
	
	/**
	 * @brief Set the QML
	 * [style](https://doc.qt.io/qt-5/qtquickcontrols2-styles.html#using-styles-in-qt-quick-controls).
	 * 
	 * @param name name of the [style](https://doc.qt.io/qt-5/qquickstyle.html#setStyle)
	 * @param fallback name of the
	 * [fallback style](https://doc.qt.io/qt-5/qquickstyle.html#setFallbackStyle), or `nullptr`.
	 * 
	 * @note The style must be configured before loading QML that imports Qt Quick Controls.
	 */
	static void style(const char *name, const char *fallback);
	
	/**
	 * @brief Create a new QML container instance.
	 * 
	 * @param w initial (framebuffer) width.
	 * @param h initial (framebuffer) height.
	 */
	QmlUi(int w, int h);
	~QmlUi();
	
	/**
	 * @brief Change the scene size.
	 * 
	 * @details Re-creates the framebuffer and generates a new texture ID (async).
	 * 
	 * @param w new (framebuffer) width.
	 * @param h new (framebuffer) height.
	 */
	void resize(int w, int h);
	
	/**
	 * @brief Send a mouse event to the QML scene.
	 * 
	 * @param type event type: `MouseMove=0` `MouseButtonPress=1` `MouseButtonRelease=2`
	 * @param button button id: `LeftButton=1` `RightButton=2` `MiddleButton=4`
	 * @param buttons currently pressed buttons, bitwise "or" of 1, 2, 4.
	 * @param x horizontal mouse position.
	 * @param y vertical mouse position.
	 * 
	 * @note If unhandled by the scene, re-emitted back as '_mouse'.
	 * Re-emitted format is
	 * [web MouseEvent](https://developer.mozilla.org/en-US/docs/Web/API/MouseEvent)
	 */
	void mouse(int type, int button, int buttons, int x, int y);
	
	/**
	 * @brief Send a keyboard event to the QML scene.
	 * 
	 * @param type event type: `KeyRelease=0` `KeyPress=1`
	 * @param key logical key id/code.
	 * @param text input text character as [unsigned int](https://doc.qt.io/qt-5/qchar.html#QChar-4).
	 * 
	 * @note If unhandled by the scene, re-emitted back as '_key'.
	 * Re-emitted format is
	 * [web KeyboardEvent](https://developer.mozilla.org/en-US/docs/Web/API/KeyboardEvent).
	 */
	void keyboard(int type, int key, unsigned text);
	
	/**
	 * @brief Set content of the QML scene with a file or a string.
	 * 
	 * @details This operation is asynchronous. An event of type `'_qml_load'` will be emited, when
	 * loading a QML scene is finished.
	 * 
	 * @param str if `isFile == true`, this is a path to QML file.
	 * Otherwise, `str` is interpreted as QML source.
	 * @param isFile tells if `src` is a path to QML file.
	 * 
	 * @note Until scene is loaded, QML methods and properties are not available and will
	 * return `[null]` JSON.
	 */
	void load(const char *str, bool isFile);
	
	/**
	 * @brief Set a property of some object in the QML scene.
	 * 
	 * @param obj name of the object, as in `Item { objectName: "my-name" }`.
	 * @param prop property key, as in `Item { property var someProp: 10 }`.
	 * @param json the value to be set. Must be a JSON array, i.e. enclosed in `[ ]`.
	 * 
	 * @example `Item { objectName: "my-name", property var x: 10 }`, to make `x`
	 * become `11`, call `ui->set("my-name", "x", "[11]")`.
	 */
	void set(const char *obj, const char *prop, const char *json);
	
	/**
	 * @brief Get a property of some object in the QML scene.
	 * 
	 * @details The property will be returned as a string containing a JSON array. The only
	 * element in this array is the value of the property. If anything went wrong
	 * the JSON will be `[null]`.
	 * 
	 * @param obj name of the object, as in `Item { objectName: "my-name" }`
	 * @param prop property key, as in `Item { property var someProp: 10 }`
	 * @return A stringified JSON value, wrapped in `[ ]`.
	 * 
	 * @note Properties can't be fetched (returns `[null]` JSON)
	 * until `'_qml_load'` event is fired.
	 * 
	 * @example For QML `Item { objectName: "my-name", property var x: 10 }`, to get `x`,
	 * call `std:string json = ui->get("my-name", "x"); // "[10]"`.
	 */
	std::string get(const char *obj, const char *prop);
	
	/**
	 * @brief Invoke a method of some object in the QML scene.
	 * 
	 * The result will be returned as a string containing a JSON array. The only
	 * element in this array is the return value of the function. If anything went wrong
	 * the JSON will be `[null]`.
	 * 
	 * @param obj name of the object, as in `Item { objectName: "my-name" }`.
	 * @param method method key, as in `Item { function f() { return 10; } }`.
	 * @param json arguments to be passed. Must be a JSON array, up to 10 elements.
	 * @return A stringified JSON result of the call, wrapped in `[ ]`.
	 * 
	 * @note Will look for a function receiving the same number of arguments
	 * you pass within the JSON array. So these must match, otherwise the call will fail.
	 * 
	 * @note Methods can't be invoked (returns `[null]` JSON, does nothing)
	 * until `'_qml_load'` event is fired.
	 * 
	 * @example `Item { objectName: "my-name", function f() { return 10; } }`, to call `f`,
	 * call `std:string json = ui.invoke("my-name", "f", "[]"); // "[10]"`
	 */
	std::string invoke(const char *obj, const char *method, const char *json);
	
	/**
	 * @brief Register a QML lib directory.
	 * 
	 * @details Additional QML search is performed in QML lib directories when
	 * importing a component.
	 * 
	 * @param path the directory path.
	 */
	void libs(const char *path);
	
	/**
	 * @brief Identical to `init2(cwdOwn, wnd, ctx, 0, cb)`.
	 *
	 * @details Due to the `device` value being always `0`, may fail to work on Linux.
	 *
	 * @deprecated Use `init2` directly.
	 * @see init2
	 */
	static void init(const char *cwdOwn, size_t wnd, size_t ctx, QmlUi::Cb cb);
	
private:
	QmlUi() {}
	void operator =(const QmlUi &v) { (void)v; }
	
	// Instance-specific callback
	QmlCb *_qmlCb;
	// Aggregation hides implementation and disregards Qt headers
	QmlView *_view;
	
};


#endif // QML_UI_HPP
