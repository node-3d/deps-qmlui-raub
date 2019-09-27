#ifndef _QML_CB_HPP_
#define _QML_CB_HPP_

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


#endif // _QML_CB_HPP_
