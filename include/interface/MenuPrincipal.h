#ifndef MENUPRINCIPAL_H
#define MENUPRINCIPAL_H

#include <QMainWindow>
#include <QTabWidget>
#include <QWidget>

class MenuPrincipal : public QMainWindow {
    Q_OBJECT

public:
    MenuPrincipal(QWidget *parent = nullptr);
    ~MenuPrincipal();

private:
    QTabWidget *abas;
};

#endif // MENUPRINCIPAL_H