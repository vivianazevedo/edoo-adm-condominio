#pragma once

#include "repositorio/RepositorioApartamento.h"
#include "repositorio/RepositorioPessoa.h"
#include "repositorio/RepositorioVisita.h"
#include "servico/FuncionarioService.h"
#include "servico/VisitaService.h"

#include <QComboBox>
#include <QDateEdit>
#include <QLineEdit>
#include <QTableWidget>
#include <QWidget>

// Cadastro de funcionarios/visitantes e controle de entrada e saida.
class TelaVisitas : public QWidget {
    Q_OBJECT

public:
    explicit TelaVisitas(QWidget* parent = nullptr);
    void atualizar();

private:
    void atualizarVisitas();

    RepositorioPessoa repoPessoas_;
    RepositorioVisita repoVisitas_;
    RepositorioApartamento repoApartamentos_;
    FuncionarioService funcionarios_;
    VisitaService visitas_;

    QLineEdit* nomeFuncionario_ = nullptr;
    QLineEdit* cpfFuncionario_ = nullptr;
    QLineEdit* telefoneFuncionario_ = nullptr;
    QComboBox* cargo_ = nullptr;
    QLineEdit* turno_ = nullptr;
    QDateEdit* admissao_ = nullptr;
    QTableWidget* tabelaFuncionarios_ = nullptr;

    QLineEdit* nomeVisitante_ = nullptr;
    QLineEdit* cpfVisitante_ = nullptr;
    QLineEdit* telefoneVisitante_ = nullptr;
    QTableWidget* tabelaVisitantes_ = nullptr;

    QComboBox* visitante_ = nullptr;
    QComboBox* apartamento_ = nullptr;
    QComboBox* porteiro_ = nullptr;
    QComboBox* filtroApartamento_ = nullptr;
    QTableWidget* tabelaVisitas_ = nullptr;
};
