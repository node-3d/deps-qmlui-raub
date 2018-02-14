#ifndef _QMLCB_HPP_
#define _QMLCB_HPP_

#include <QObject>
#include <QVariantMap>

#include "qmlui.hpp"


class QmlCb : public QObject {
	Q_OBJECT
	
public:
	
	QmlCb(QmlUi *owner, QmlUi::Cb cb);
	
	Q_INVOKABLE void call(const QString &type, QVariantMap props = QVariantMap()) const;
	
	
private:
	
	QmlUi *_owner;
	QmlUi::Cb _cb;
	
};


#endif // _QMLCB_HPP_
