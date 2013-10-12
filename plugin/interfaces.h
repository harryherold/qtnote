#ifndef INTERFACES_H
#define INTERFACES_H
#include <QtPlugin>
class HelloInterface{
public:	
	virtual void sayHello() = 0;
	virtual ~HelloInterface(){}
};

Q_DECLARE_INTERFACE(HelloInterface,"Hello.World.HelloInterface/1.0");
 #endif
