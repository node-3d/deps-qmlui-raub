#ifndef QMLUI_HPP
#define QMLUI_HPP

#if defined QMLUI_SHARED
 #define QMLUI_DLLSPEC Q_DECL_EXPORT
#else
 #define QMLUI_DLLSPEC __declspec(dllimport)
#endif


// Callback type
typedef void (*EventCb) (const char *data);

// C-API:

extern "C" QMLUI_DLLSPEC void qmlui_init(const char *cwdOwn, size_t wnd, size_t ctx, int w, int h, EventCb cb);
extern "C" QMLUI_DLLSPEC void qmlui_resize(int i, int w, int h);
extern "C" QMLUI_DLLSPEC void qmlui_mouse(int type, int button, int buttons, int x, int y);
extern "C" QMLUI_DLLSPEC void qmlui_keyboard(int type, int key, char text);
extern "C" QMLUI_DLLSPEC void qmlui_use(const char *str, bool isFile);
extern "C" QMLUI_DLLSPEC void qmlui_set(const char *obj, const char *prop, const char *json);
extern "C" QMLUI_DLLSPEC void qmlui_get(const char *obj, const char *prop);
extern "C" QMLUI_DLLSPEC void qmlui_invoke(const char *obj, const char *method, const char *json);
extern "C" QMLUI_DLLSPEC void qmlui_libs(const char *libs);
extern "C" QMLUI_DLLSPEC void qmlui_plugins(const char *plugins);

#endif // QMLUI_HPP
