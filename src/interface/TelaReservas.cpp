#include "interface/TelaReservas.h"
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>

TelaReservas::TelaReservas(QWidget *parent) : QWidget(parent) {
    configurarLayout();
    connect(btnReservar, &QPushButton::clicked, this, &TelaReservas::aoClicarCriarReserva);
}

TelaReservas::~TelaReservas() {}

void TelaReservas::configurarLayout() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *formLayout = new QHBoxLayout();

    comboMoradores = new QComboBox(this);
    comboMoradores->addItem("Selecione o Morador...");

    comboAreasComuns = new QComboBox(this);
    comboAreasComuns->addItem("Salão de Festas");
    comboAreasComuns->addItem("Churrasqueira");

    dateTimeInicio = new QDateTimeEdit(QDateTime::currentDateTime(), this);
    dateTimeFim = new QDateTimeEdit(QDateTime::currentDateTime().addSecs(3600), this);
    btnReservar = new QPushButton("Reservar", this);

    formLayout->addWidget(comboMoradores);
    formLayout->addWidget(comboAreasComuns);
    formLayout->addWidget(dateTimeInicio);
    formLayout->addWidget(dateTimeFim);
    formLayout->addWidget(btnReservar);

    tabelaReservas = new QTableWidget(this);
    tabelaReservas->setColumnCount(4);
    tabelaReservas->setHorizontalHeaderLabels({"Morador", "Área", "Início", "Fim"});
    tabelaReservas->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(tabelaReservas);
    setLayout(mainLayout);
}

void TelaReservas::aoClicarCriarReserva() {
    try {
        QMessageBox::information(this, "Sucesso", "Reserva realizada com sucesso!");
    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Erro de Reserva", e.what());
    }
}