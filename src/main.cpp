#include <QApplication>
#include "interface/MenuPrincipal.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MenuPrincipal menu;
    menu.show();
    return app.exec();
}