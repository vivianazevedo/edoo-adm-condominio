#include "interface/MenuPrincipal.h"
#include "interface/TelaMoradores.h"
#include "interface/TelaReservas.h"
#include "interface/TelaVisitas.h"
#include "interface/UtilQt.h"

MenuPrincipal::MenuPrincipal(QWidget *parent)
    : QMainWindow(parent) {
    
    setWindowTitle("Sistema de Gestão de Condomínio");
    resize(1000, 700);

    abas = new QTabWidget(this);

    auto* moradores = new TelaMoradores(this);
    auto* reservas = new TelaReservas(this);
    auto* visitas = new TelaVisitas(this);
    abas->addTab(moradores, "Moradores e Aptos");
    abas->addTab(reservas, "Areas e Reservas");
    abas->addTab(visitas, "Portaria e Visitas");

    connect(abas, &QTabWidget::currentChanged, this,
            [this, moradores, reservas, visitas](int indice) {
        executarNaTela(this, [=] {
            if (indice == 0) moradores->atualizar();
            if (indice == 1) reservas->atualizar();
            if (indice == 2) visitas->atualizar();
        });
    });

    setCentralWidget(abas);
}

MenuPrincipal::~MenuPrincipal() {}
