#pragma once

#include "repositorio/RepositorioApartamento.h"
#include "repositorio/RepositorioPessoa.h"
#include "servico/ApartamentoService.h"
#include "servico/MoradorService.h"

#include <QComboBox>
#include <QDateEdit>
#include <QLineEdit>
#include <QSpinBox>
#include <QTableWidget>
#include <QWidget>

// aba de apartamentos e moradores (qt): formularios, botoes e tabelas
// nunca mexe no sql, so chama os services
class TelaMoradores : public QWidget {
    Q_OBJECT

public:
    explicit TelaMoradores(QWidget* parent = nullptr);
    // recarrega as tabelas e as listas com o que esta no banco
    void atualizar();

private:
    // repositorios e services da tela (o service recebe o repositorio por referencia)
    RepositorioApartamento repoApartamentos_;
    RepositorioPessoa repoPessoas_;
    ApartamentoService apartamentos_;
    MoradorService moradores_;

    // campos dos formularios e tabelas, sao ponteiros e quem apaga eles e o qt
    QLineEdit* bloco_ = nullptr;
    QLineEdit* numero_ = nullptr;
    QSpinBox* andar_ = nullptr;
    QTableWidget* tabelaApartamentos_ = nullptr;

    QLineEdit* nome_ = nullptr;
    QLineEdit* cpf_ = nullptr;
    QLineEdit* telefone_ = nullptr;
    QComboBox* apartamento_ = nullptr;
    QComboBox* ocupacao_ = nullptr;
    QDateEdit* entrada_ = nullptr;
    QTableWidget* tabelaMoradores_ = nullptr;
};
