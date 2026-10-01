#include "interface/TelaVisitas.h"
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>

TelaVisitas::TelaVisitas(QWidget *parent) : QWidget(parent) {
    configurarLayout();
    connect(btnEntrada, &QPushButton::clicked, this, &TelaVisitas::aoClicarRegistrarEntrada);
}

TelaVisitas::~TelaVisitas() {}

void TelaVisitas::configurarLayout() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *formLayout = new QHBoxLayout();

    txtVisitante = new QLineEdit(this); 
    txtVisitante->setPlaceholderText("Nome do Visitante");
    
    txtDocumento = new QLineEdit(this); 
    txtDocumento->setPlaceholderText("Documento/RG");
    
    txtApto = new QLineEdit(this); 
    txtApto->setPlaceholderText("Apto Destino");
    
    btnEntrada = new QPushButton("Registrar Entrada", this);

    formLayout->addWidget(txtVisitante);
    formLayout->addWidget(txtDocumento);
    formLayout->addWidget(txtApto);
    formLayout->addWidget(btnEntrada);

    tabelaVisitas = new QTableWidget(this);
    tabelaVisitas->setColumnCount(4);
    tabelaVisitas->setHorizontalHeaderLabels({"Visitante", "Documento", "Apto", "Entrada"});
    tabelaVisitas->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(tabelaVisitas);
    setLayout(mainLayout);
}

void TelaVisitas::aoClicarRegistrarEntrada() {
    try {
        QMessageBox::information(this, "Portaria", "Entrada de visitante registrada!");
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Erro na Portaria", e.what());
    }
}