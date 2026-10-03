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

// CRUD de apartamentos e moradores, sempre por meio dos servicos.
class TelaMoradores : public QWidget {
    Q_OBJECT

public:
    explicit TelaMoradores(QWidget* parent = nullptr);
    void atualizar();

private:
    RepositorioApartamento repoApartamentos_;
    RepositorioPessoa repoPessoas_;
    ApartamentoService apartamentos_;
    MoradorService moradores_;

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
