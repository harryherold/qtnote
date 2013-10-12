#include <QDebug>
#include <QtGui>
#include <QObject>
#include "plugin.h"
void Plugin::sayHello(){
	qDebug("Plugin say Hello");	
}
Q_EXPORT_PLUGIN2(pnp_plugin, Plugin)
