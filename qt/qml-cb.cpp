#include <QDebug>
#include <QJsonDocument>

#include "qml-cb.hpp"


QmlCb::QmlCb(QmlUi *owner, QmlUi::Cb cb) {
	
	_owner = owner;
	_cb    = cb;
	
}


void QmlCb::call(const QString &type, QVariantMap props) const {
	
	QByteArray ba("{\"type\":\"%1\",\"data\":%2}");
	
	ba.replace("%1", type.toLatin1());
	ba.replace("%2", QJsonDocument::fromVariant(props).toJson());
	
	_cb( _owner, ba.constData() );
	
}
