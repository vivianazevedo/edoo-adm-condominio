#include "interface/MenuPrincipal.h"

MenuPrincipal::MenuPrincipal(QWidget *parent)
    : QMainWindow(parent) {
    
    setWindowTitle("Sistema de Condomínio");
    resize(800, 600);

    abas = new QTabWidget(this);

    QWidget *abaDashboard = new QWidget();
    QWidget *abaMoradores = new QWidget();
    QWidget *abaReservas = new QWidget();
    QWidget *abaFinanceiro = new QWidget();

    abas->addTab(abaDashboard, "Dashboard");
    abas->addTab(abaMoradores, "Moradores");
    abas->addTab(abaReservas, "Reservas");
    abas->addTab(abaFinanceiro, "Financeiro");

    setCentralWidget(abas);
}

MenuPrincipal::~MenuPrincipal() {}