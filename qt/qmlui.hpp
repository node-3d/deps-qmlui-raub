#ifndef _QMLUI_HPP_
#define _QMLUI_HPP_


#if defined QMLUI_SHARED
 #define QMLUI_DLLSPEC Q_DECL_EXPORT
#else
 #define QMLUI_DLLSPEC __declspec(dllimport)
#endif


class QmlWindow;
class QmlCb;


class QMLUI_SHARED QmlUi {
	
public:
	
	typedef void (*Cb) (QmlUi *target, const char *data);
	
	
public:
	
	static void init(const char *cwdOwn, size_t wnd, size_t ctx, QmlUi::Cb cb);
	static void plugins(const char *path);
	static void exit();
	
	
public:
	
	QmlUi(int w, int h);
	~QmlUi();
	
	void resize(int w, int h);
	void mouse(int type, int button, int buttons, int x, int y);
	void keyboard(int type, int key, char text);
	void load(const char *str, bool isFile);
	void set(const char *obj, const char *prop, const char *json);
	void get(const char *obj, const char *prop);
	void invoke(const char *obj, const char *method, const char *json);
	void libs(const char *path);
	
	
private:
	
	QmlCb *_callback;
	QmlWindow *_view;
	
	
private:
	
	static QmlCb *_callback;
	QmlWindow *_view;
	
};


#endif // _QMLUI_HPP_
