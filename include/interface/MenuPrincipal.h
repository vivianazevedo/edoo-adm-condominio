#ifndef MENUPRINCIPAL_H
#define MENUPRINCIPAL_H

#include <QMainWindow>
#include <QTabWidget>
#include <QWidget>

// janela principal da interface grafica (qt): so tem as abas, cada aba e uma tela
class MenuPrincipal : public QMainWindow {
    // macro do qt, necessaria pra usar signals e slots
    Q_OBJECT

public:
    MenuPrincipal(QWidget *parent = nullptr);
    ~MenuPrincipal();

private:
    // o widget que guarda as abas
    QTabWidget *abas;
};

#endif // MENUPRINCIPAL_H