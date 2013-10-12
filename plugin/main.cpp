#include "core.h"
#include <QtPlugin>
#include <QApplication>

Q_IMPORT_PLUGIN(pnp_plugin)

 int main(void)
 {
     Core *c = new Core();
     c->loadPlugins();
     c->sayHello(); 
     return 0;
}
