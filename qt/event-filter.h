#ifndef _EVENTFILTER_H_
#define _EVENTFILTER_H_

#include <QObject>


class EventFilter : public QObject {
	Q_OBJECT
	
public:
	explicit EventFilter(QObject *parent = nullptr) : QObject(parent) {}

protected:
	bool eventFilter(QObject *obj, QEvent *event);
	
};


#endif // _EVENTFILTER_H_
