#ifndef QMLUI_HPP
#define QMLUI_HPP

#if defined QMLUI_SHARED
 #define QMLUI_DLLSPEC Q_DECL_EXPORT
#else
 #define QMLUI_DLLSPEC __declspec(dllimport)
#endif


// Callback type
typedef void (*EventCb) (int i, const char *data);

// C-API:

extern "C" QMLUI_DLLSPEC void qmlui_init(const char *cwdOwn, size_t wnd, size_t ctx, EventCb cb);
extern "C" QMLUI_DLLSPEC void qmlui_view(volatile int *i, int w, int h);
extern "C" QMLUI_DLLSPEC void qmlui_close(int i);
extern "C" QMLUI_DLLSPEC void qmlui_exit();
extern "C" QMLUI_DLLSPEC void qmlui_resize(int i, int w, int h);
extern "C" QMLUI_DLLSPEC void qmlui_mouse(int i, int type, int button, int buttons, int x, int y);
extern "C" QMLUI_DLLSPEC void qmlui_keyboard(int i, int type, int key, char text);
extern "C" QMLUI_DLLSPEC void qmlui_load(int i, const char *str, bool isFile);
extern "C" QMLUI_DLLSPEC void qmlui_set(int i, const char *obj, const char *prop, const char *json);
extern "C" QMLUI_DLLSPEC void qmlui_get(int i, const char *obj, const char *prop);
extern "C" QMLUI_DLLSPEC void qmlui_invoke(int i, const char *obj, const char *method, const char *json);
extern "C" QMLUI_DLLSPEC void qmlui_libs(int i, const char *libs);
extern "C" QMLUI_DLLSPEC void qmlui_plugins(const char *plugins);

#endif // QMLUI_HPP
