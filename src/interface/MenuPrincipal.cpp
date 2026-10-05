#include "interface/MenuPrincipal.h"
#include "interface/TelaMoradores.h"
#include "interface/TelaReservas.h"
#include "interface/TelaVisitas.h"
#include "interface/UtilQt.h"

// monta a janela: titulo, tamanho e as tres abas
MenuPrincipal::MenuPrincipal(QWidget *parent)
    : QMainWindow(parent) {
    
    setWindowTitle("Sistema de Gestão de Condomínio");
    resize(1000, 700);

    abas = new QTabWidget(this);

    // cada tela e um QWidget e vira uma aba
    auto* moradores = new TelaMoradores(this);
    auto* reservas = new TelaReservas(this);
    auto* visitas = new TelaVisitas(this);
    abas->addTab(moradores, "Moradores e Aptos");
    abas->addTab(reservas, "Areas e Reservas");
    abas->addTab(visitas, "Portaria e Visitas");

    // quando troca de aba, a tela daquela aba recarrega os dados do banco (signal e slot)
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

// vazio de proposito: os widgets filhos sao apagados pelo qt sozinho (o pai e o dono)
MenuPrincipal::~MenuPrincipal() {}
