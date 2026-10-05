#pragma once

#include "repositorio/RepositorioAreaComum.h"
#include "repositorio/RepositorioPessoa.h"
#include "repositorio/RepositorioReserva.h"
#include "servico/AreaComumService.h"
#include "servico/ReservaService.h"

#include <QComboBox>
#include <QDateEdit>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QTableWidget>
#include <QTimeEdit>
#include <QWidget>

#include <memory>

// aba de areas comuns e reservas (qt): cadastra areas e cria, consulta e cancela reservas
// nunca mexe no sql, so chama os services
class TelaReservas : public QWidget {
    Q_OBJECT

public:
    explicit TelaReservas(QWidget* parent = nullptr);
    // recarrega as tabelas e as listas com o que esta no banco
    void atualizar();

private:
    // os services de areas e reservas recebem shared_ptr, por isso esses dois repositorios sao shared_ptr
    std::shared_ptr<RepositorioAreaComum> repoAreas_;
    std::shared_ptr<RepositorioReserva> repoReservas_;
    RepositorioPessoa repoPessoas_;
    AreaComumService areas_;
    ReservaService reservas_;

    // campos dos formularios e tabelas, sao ponteiros e quem apaga eles e o qt
    QComboBox* tipo_ = nullptr;
    QLineEdit* nomeArea_ = nullptr;
    QSpinBox* capacidade_ = nullptr;
    QDoubleSpinBox* taxa_ = nullptr;
    QTimeEdit* abertura_ = nullptr;
    QTimeEdit* fechamento_ = nullptr;
    QTableWidget* tabelaAreas_ = nullptr;

    QComboBox* morador_ = nullptr;
    QComboBox* area_ = nullptr;
    QDateEdit* data_ = nullptr;
    QTimeEdit* inicio_ = nullptr;
    QTimeEdit* fim_ = nullptr;
    QSpinBox* convidados_ = nullptr;
    QTableWidget* tabelaReservas_ = nullptr;
};
