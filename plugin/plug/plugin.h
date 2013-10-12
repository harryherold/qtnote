#ifndef PLUGIN_H
#define PLUGIN_H
#include <QString>
#include <QObject>
#include "../interfaces.h"
/*
 * unbedingt Reihenfolge bei vererbung einhalten
 * d.h. erst QObject dann die eigenen Klassen
 */
class Plugin :
				public QObject,
				public HelloInterface
				
{
	Q_OBJECT
	Q_INTERFACES(HelloInterface)
public:
	void sayHello();
	
};

#endif
