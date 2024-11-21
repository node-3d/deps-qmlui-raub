#include <QJsonDocument>
#include <QList>
#include <QJSValue>

#include "qml-cb.hpp"


QmlUi::Cb QmlCb::__cb;


void QmlCb::init(QmlUi::Cb cb) {
	__cb = cb;
}


QmlCb::QmlCb(QmlUi *owner) {
	_owner = owner;
}


void QmlCb::eventEmit(const QByteArray &name, QVariant data) const {
	QList<QVariant> vlist;
	
	QVariant jsVariant = data.value<QJSValue>().toVariant();
	if (jsVariant.isValid()) {
		vlist.push_back(jsVariant);
	} else {
		vlist.push_back(data);
	}
	
	__cb(_owner, name, QJsonDocument::fromVariant(vlist).toJson());
}
