#ifndef _QML_UI_HPP_
#define _QML_UI_HPP_

#ifdef WIN32
	#if defined QMLUI_SHARED
		#define QMLUI_DLLSPEC __declspec(dllexport)
	#else
		#define QMLUI_DLLSPEC __declspec(dllimport)
	#endif
#else
	#define QMLUI_DLLSPEC
#endif

class QmlView;
class QmlCb;


class QMLUI_DLLSPEC QmlUi {
	
public:
	typedef void (*Cb) (QmlUi *target, const char *type, const char *json);
	
	static void init(const char *cwdOwn, size_t wnd, size_t ctx, QmlUi::Cb cb);
	static void plugins(const char *path);
	static void update();
	
	QmlUi(int w, int h);
	~QmlUi();
	
	void resize(int w, int h);
	void mouse(int type, int button, int buttons, int x, int y);
	void keyboard(int type, int key, unsigned text);
	void load(const char *str, bool isFile);
	void set(const char *obj, const char *prop, const char *json);
	void get(const char *obj, const char *prop);
	void invoke(const char *obj, const char *method, const char *json);
	void libs(const char *path);
	
private:
	// Instance-specific callback
	QmlCb *_qmlCb;
	// Aggregation hides implementation and disregards Qt headers
	QmlView *_view;
	
};


#endif // _QML_UI_HPP_
