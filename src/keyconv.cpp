#include <QMap>
#include <QDebug>

#include "keyconv.hpp"

QMap<int, Qt::Key> keys;


Qt::Key keyconv(int from)
{

	if (keys.contains(from)) {
//		qDebug() << "CONV contains" << Qt::Key_0 << keys[from] << from;
		return keys[from];
	} else {
		qDebug() << "CONV absent" << static_cast<Qt::Key>(from) << from;
		return static_cast<Qt::Key>(from);
	}
}


void keyfill()
{
	keys[259] = Qt::Key_Backspace;
	keys[257] = Qt::Key_Enter;
}
