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

// Cadastro de areas e operacoes de reserva conectadas ao SQLite.
class TelaReservas : public QWidget {
    Q_OBJECT

public:
    explicit TelaReservas(QWidget* parent = nullptr);
    void atualizar();

private:
    std::shared_ptr<RepositorioAreaComum> repoAreas_;
    std::shared_ptr<RepositorioReserva> repoReservas_;
    RepositorioPessoa repoPessoas_;
    AreaComumService areas_;
    ReservaService reservas_;

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
