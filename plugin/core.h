#ifndef CORE_H
#define CORE_H
#include "interfaces.h"
#include <QString>
#include <QDir>
#include <QObject>
class Core : 
				public QObject
{
	
	Q_OBJECT
	
public:
	void sayHello();
	Core();
	void loadPlugins();
private:
	HelloInterface *inter;
	QDir pluginsDir;
};

#endif
