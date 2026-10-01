#include "interface/TelaMoradores.h"
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>

TelaMoradores::TelaMoradores(QWidget *parent) : QWidget(parent) {
    configurarLayout();
    connect(btnCadastrar, &QPushButton::clicked, this, &TelaMoradores::aoClicarCadastrar);
}

TelaMoradores::~TelaMoradores() {}

void TelaMoradores::configurarLayout() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *formLayout = new QHBoxLayout();

    txtNome = new QLineEdit(this); 
    txtNome->setPlaceholderText("Nome completo");
    
    txtCpf = new QLineEdit(this); 
    txtCpf->setPlaceholderText("CPF");
    
    txtTelefone = new QLineEdit(this); 
    txtTelefone->setPlaceholderText("Telefone");
    
    txtApartamento = new QLineEdit(this); 
    txtApartamento->setPlaceholderText("Nº Apto");
    
    btnCadastrar = new QPushButton("Cadastrar", this);

    formLayout->addWidget(txtNome);
    formLayout->addWidget(txtCpf);
    formLayout->addWidget(txtTelefone);
    formLayout->addWidget(txtApartamento);
    formLayout->addWidget(btnCadastrar);

    tabelaMoradores = new QTableWidget(this);
    tabelaMoradores->setColumnCount(4);
    tabelaMoradores->setHorizontalHeaderLabels({"Nome", "CPF", "Telefone", "Apartamento"});
    tabelaMoradores->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(tabelaMoradores);
    setLayout(mainLayout);
}

void TelaMoradores::aoClicarCadastrar() {
    try {
        if (txtNome->text().isEmpty() || txtCpf->text().isEmpty()) {
            QMessageBox::warning(this, "Validação", "Preencha os campos obrigatórios.");
            return;
        }
        
        QMessageBox::information(this, "Sucesso", "Morador cadastrado com sucesso!");
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Erro", e.what());
    }
}