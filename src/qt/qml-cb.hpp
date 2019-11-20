#ifndef QML_CB_HPP
#define QML_CB_HPP

#include <QObject>
#include <QVariant>
#include <qml-ui.hpp>


class QmlCb : public QObject {
	Q_OBJECT
	
public:
	
	static void init(QmlUi::Cb cb);
	
	
public:
	
	explicit QmlCb(QmlUi *owner);
	
	// Callable within QML code
	Q_INVOKABLE void eventEmit(const QByteArray &name,
		QVariant data = QVariant()
	) const;
	
private:
	
	// Which Ui does this instance belong to
	QmlUi *_owner;
	
	
private:
	
	// A single global callback
	static QmlUi::Cb __cb;
	
};


#endif // QML_CB_HPP
