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

// aba da portaria (qt): cadastra funcionarios e visitantes e controla entrada e saida das visitas
// nunca mexe no sql, so chama os services
class TelaVisitas : public QWidget {
    Q_OBJECT

public:
    explicit TelaVisitas(QWidget* parent = nullptr);
    // recarrega tudo com o que esta no banco
    void atualizar();

private:
    // recarrega so a tabela de visitas (respeitando o filtro de apartamento)
    void atualizarVisitas();

    // repositorios e services da tela (o service recebe o repositorio por referencia)
    RepositorioPessoa repoPessoas_;
    RepositorioVisita repoVisitas_;
    RepositorioApartamento repoApartamentos_;
    FuncionarioService funcionarios_;
    VisitaService visitas_;

    // campos dos formularios e tabelas, sao ponteiros e quem apaga eles e o qt
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
