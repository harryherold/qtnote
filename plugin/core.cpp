#include "core.h"
#include <QApplication>
#include "interfaces.h"
#include <QPluginLoader>
#include <QDebug>
Core::Core(){
	qDebug("huhuh");
}
void Core::loadPlugins(){
	foreach(QObject *plugin, QPluginLoader::staticInstances())
		inter = qobject_cast<HelloInterface *>(plugin);
		
}
void Core::sayHello(){
	qDebug("Core say Hello");
	inter->sayHello();
}
