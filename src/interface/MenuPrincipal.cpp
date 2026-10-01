#include "interface/MenuPrincipal.h"
#include "interface/TelaMoradores.h"
#include "interface/TelaReservas.h"
#include "interface/TelaVisitas.h"

MenuPrincipal::MenuPrincipal(QWidget *parent)
    : QMainWindow(parent) {
    
    setWindowTitle("Sistema de Gestão de Condomínio");
    resize(1000, 700);

    abas = new QTabWidget(this);

    abas->addTab(new TelaMoradores(this), "Moradores e Aptos");
    abas->addTab(new TelaReservas(this), "Áreas e Reservas");
    abas->addTab(new TelaVisitas(this), "Portaria e Visitas");

    setCentralWidget(abas);
}

MenuPrincipal::~MenuPrincipal() {}