#include <QJsonDocument>
#include <QList>

#include "qml-cb.hpp"


QmlUi::Cb QmlCb::__cb;


void QmlCb::init(QmlUi::Cb cb) {
	
	__cb = cb;
	
}


QmlCb::QmlCb(QmlUi *owner) {
	
	_owner = owner;
	
}


void QmlCb::call(const QString &type, QVariant props) const {
	
	QList<QVariant> vlist;
	vlist.push_back(props);
	__cb(_owner, type.toLatin1(), QJsonDocument::fromVariant(vlist).toJson());
	
}
