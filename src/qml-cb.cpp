#include "qml-cb.hpp"
#include <QDebug>
#include <QJsonDocument>


QmlCb::QmlCb(QObject *parent, int i, EventCb cb) : QObject(parent) {
	
	_index   = i;
	_eventCb = cb;
	
}


void QmlCb::call(const QString &type, QVariantMap props) {
	
	QByteArray ba("{ \"type\": \"%1\", \"data\": %2}");
	
	ba.replace("%1", type.toLatin1());
	ba.replace("%2", QJsonDocument::fromVariant(props).toJson());
	
	_eventCb( _index, ba.constData() );
	
}
