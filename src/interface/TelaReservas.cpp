#include "interface/TelaReservas.h"

#include "interface/UtilQt.h"
#include "modelo/Morador.h"

#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

namespace {

void prepararTabela(QTableWidget* tabela, const QStringList& colunas) {
    tabela->setColumnCount(colunas.size());
    tabela->setHorizontalHeaderLabels(colunas);
    tabela->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tabela->setSelectionBehavior(QAbstractItemView::SelectRows);
    tabela->setSelectionMode(QAbstractItemView::SingleSelection);
    tabela->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tabela->setMinimumHeight(175);
    tabela->setAlternatingRowColors(true);
}

}  // namespace

TelaReservas::TelaReservas(QWidget* parent)
    : QWidget(parent), repoAreas_(std::make_shared<RepositorioAreaComum>()),
      repoReservas_(std::make_shared<RepositorioReserva>()), areas_(repoAreas_),
      reservas_(repoReservas_, repoAreas_) {
    auto* pagina = new QWidget(this);
    auto* layout = new QVBoxLayout(pagina);

    auto* grupoAreas = new QGroupBox("Areas comuns", pagina);
    auto* layoutAreas = new QVBoxLayout(grupoAreas);
    auto* formularioArea = new QFormLayout;
    tipo_ = new QComboBox(grupoAreas);
    tipo_->setObjectName("areaTipo");
    tipo_->addItems({"SalaoFestas", "Piscina", "Churrasqueira"});
    nomeArea_ = new QLineEdit(grupoAreas);
    capacidade_ = new QSpinBox(grupoAreas);
    nomeArea_->setObjectName("areaNome");
    capacidade_->setObjectName("areaCapacidade");
    capacidade_->setRange(1, 10000);
    taxa_ = new QDoubleSpinBox(grupoAreas);
    taxa_->setObjectName("areaTaxa");
    taxa_->setRange(0, 1000000);
    taxa_->setDecimals(2);
    taxa_->setPrefix("R$ ");
    abertura_ = new QTimeEdit(QTime(8, 0), grupoAreas);
    fechamento_ = new QTimeEdit(QTime(23, 0), grupoAreas);
    abertura_->setDisplayFormat("HH:mm");
    fechamento_->setDisplayFormat("HH:mm");
    formularioArea->addRow("Tipo (so no cadastro)", tipo_);
    formularioArea->addRow("Nome", nomeArea_);
    formularioArea->addRow("Capacidade", capacidade_);
    formularioArea->addRow("Taxa base", taxa_);
    formularioArea->addRow("Abertura (so no cadastro)", abertura_);
    formularioArea->addRow("Fechamento (so no cadastro)", fechamento_);
    layoutAreas->addLayout(formularioArea);
    auto* botoesArea = new QHBoxLayout;
    auto* cadastrarArea = new QPushButton("Cadastrar area", grupoAreas);
    cadastrarArea->setObjectName("areaCadastrar");
    auto* editarArea = new QPushButton("Editar selecionada", grupoAreas);
    auto* removerArea = new QPushButton("Remover selecionada", grupoAreas);
    editarArea->setObjectName("areaEditar");
    removerArea->setObjectName("areaRemover");
    botoesArea->addWidget(cadastrarArea);
    botoesArea->addWidget(editarArea);
    botoesArea->addWidget(removerArea);
    layoutAreas->addLayout(botoesArea);
    tabelaAreas_ = new QTableWidget(grupoAreas);
    tabelaAreas_->setObjectName("areasTabela");
    prepararTabela(tabelaAreas_, {"ID", "Tipo", "Nome", "Capacidade", "Taxa", "Abertura", "Fechamento"});
    layoutAreas->addWidget(tabelaAreas_);
    layout->addWidget(grupoAreas);

    auto* grupoReservas = new QGroupBox("Reservas", pagina);
    auto* layoutReservas = new QVBoxLayout(grupoReservas);
    auto* formularioReserva = new QFormLayout;
    morador_ = new QComboBox(grupoReservas);
    area_ = new QComboBox(grupoReservas);
    data_ = new QDateEdit(QDate::currentDate().addDays(2), grupoReservas);
    morador_->setObjectName("reservaMorador");
    area_->setObjectName("reservaArea");
    data_->setObjectName("reservaData");
    data_->setCalendarPopup(true);
    data_->setDisplayFormat("dd/MM/yyyy");
    data_->setMinimumDate(QDate::currentDate());
    inicio_ = new QTimeEdit(QTime(10, 0), grupoReservas);
    fim_ = new QTimeEdit(QTime(11, 0), grupoReservas);
    inicio_->setObjectName("reservaInicio");
    fim_->setObjectName("reservaFim");
    inicio_->setDisplayFormat("HH:mm");
    fim_->setDisplayFormat("HH:mm");
    convidados_ = new QSpinBox(grupoReservas);
    convidados_->setObjectName("reservaConvidados");
    convidados_->setRange(0, 10000);
    formularioReserva->addRow("Morador", morador_);
    formularioReserva->addRow("Area", area_);
    formularioReserva->addRow("Data", data_);
    formularioReserva->addRow("Inicio", inicio_);
    formularioReserva->addRow("Fim", fim_);
    formularioReserva->addRow("Convidados", convidados_);
    layoutReservas->addLayout(formularioReserva);
    auto* botoesReserva = new QHBoxLayout;
    auto* criarReserva = new QPushButton("Criar reserva", grupoReservas);
    criarReserva->setObjectName("reservaCriar");
    auto* consultarReservas = new QPushButton("Atualizar consulta", grupoReservas);
    auto* cancelarReserva = new QPushButton("Cancelar selecionada", grupoReservas);
    cancelarReserva->setObjectName("reservaCancelar");
    botoesReserva->addWidget(criarReserva);
    botoesReserva->addWidget(consultarReservas);
    botoesReserva->addWidget(cancelarReserva);
    layoutReservas->addLayout(botoesReserva);
    tabelaReservas_ = new QTableWidget(grupoReservas);
    tabelaReservas_->setObjectName("reservasTabela");
    prepararTabela(tabelaReservas_, {"ID", "Morador", "Area", "Data", "Inicio", "Fim", "Status", "Valor"});
    layoutReservas->addWidget(tabelaReservas_);
    layout->addWidget(grupoReservas);

    auto* rolagem = new QScrollArea(this);
    rolagem->setWidgetResizable(true);
    rolagem->setWidget(pagina);
    auto* externo = new QVBoxLayout(this);
    externo->addWidget(rolagem);

    connect(cadastrarArea, &QPushButton::clicked, this, [this] {
        executarNaTela(this, [this] {
            areas_.cadastrar(tipo_->currentText().toStdString(),
                             nomeArea_->text().trimmed().toStdString(), capacidade_->value(),
                             taxa_->value(), abertura_->time().toString("HH:mm").toStdString(),
                             fechamento_->time().toString("HH:mm").toStdString());
            atualizar();
        });
    });
    connect(editarArea, &QPushButton::clicked, this, [this] {
        executarNaTela(this, [this] {
            areas_.editar(idSelecionado(tabelaAreas_), nomeArea_->text().trimmed().toStdString(),
                          capacidade_->value(), taxa_->value());
            atualizar();
        });
    });
    connect(removerArea, &QPushButton::clicked, this, [this] {
        executarNaTela(this, [this] {
            areas_.remover(idSelecionado(tabelaAreas_));
            atualizar();
        });
    });
    connect(tabelaAreas_, &QTableWidget::cellClicked, this, [this](int linha, int) {
        const int id = tabelaAreas_->item(linha, 0)->text().toInt();
        auto selecionada = repoAreas_->buscarPorId(id);
        if (!selecionada) return;
        tipo_->setCurrentIndex(tipo_->findText(QString::fromStdString(selecionada->getTipo())));
        nomeArea_->setText(QString::fromStdString(selecionada->getNome()));
        capacidade_->setValue(selecionada->getCapacidade());
        taxa_->setValue(selecionada->getTaxaBase());
        abertura_->setTime(QTime::fromString(QString::fromStdString(selecionada->getHoraAbertura()), "HH:mm"));
        fechamento_->setTime(QTime::fromString(QString::fromStdString(selecionada->getHoraFechamento()), "HH:mm"));
    });

    connect(criarReserva, &QPushButton::clicked, this, [this] {
        executarNaTela(this, [this] {
            reservas_.criar(morador_->currentData().toInt(), area_->currentData().toInt(),
                            data_->date().toString("yyyy-MM-dd").toStdString(),
                            inicio_->time().toString("HH:mm").toStdString(),
                            fim_->time().toString("HH:mm").toStdString(), convidados_->value());
            atualizar();
        });
    });
    connect(consultarReservas, &QPushButton::clicked, this,
            [this] { executarNaTela(this, [this] { atualizar(); }); });
    connect(cancelarReserva, &QPushButton::clicked, this, [this] {
        executarNaTela(this, [this] {
            reservas_.cancelar(idSelecionado(tabelaReservas_));
            atualizar();
        });
    });

    atualizar();
}

void TelaReservas::atualizar() {
    const auto pessoas = repoPessoas_.listar();
    morador_->clear();
    morador_->addItem("Selecione...", -1);
    for (const auto& pessoa : pessoas) {
        if (dynamic_cast<const Morador*>(pessoa.get()) != nullptr) {
            morador_->addItem(QString::fromStdString(pessoa->nome()), pessoa->id());
        }
    }

    const auto areas = areas_.listar();
    area_->clear();
    area_->addItem("Selecione...", -1);
    tabelaAreas_->setRowCount(0);
    for (const auto& item : areas) {
        const int linha = tabelaAreas_->rowCount();
        tabelaAreas_->insertRow(linha);
        tabelaAreas_->setItem(linha, 0, new QTableWidgetItem(QString::number(item->getId())));
        tabelaAreas_->setItem(linha, 1, new QTableWidgetItem(QString::fromStdString(item->getTipo())));
        tabelaAreas_->setItem(linha, 2, new QTableWidgetItem(QString::fromStdString(item->getNome())));
        tabelaAreas_->setItem(linha, 3, new QTableWidgetItem(QString::number(item->getCapacidade())));
        tabelaAreas_->setItem(linha, 4, new QTableWidgetItem(QString::number(item->getTaxaBase(), 'f', 2)));
        tabelaAreas_->setItem(linha, 5, new QTableWidgetItem(QString::fromStdString(item->getHoraAbertura())));
        tabelaAreas_->setItem(linha, 6, new QTableWidgetItem(QString::fromStdString(item->getHoraFechamento())));
        area_->addItem(QString::fromStdString(item->getNome()), item->getId());
    }

    const auto reservas = repoReservas_->listar();
    tabelaReservas_->setRowCount(0);
    for (const auto& item : reservas) {
        const int linha = tabelaReservas_->rowCount();
        tabelaReservas_->insertRow(linha);
        tabelaReservas_->setItem(linha, 0, new QTableWidgetItem(QString::number(item->getId())));
        tabelaReservas_->setItem(linha, 1, new QTableWidgetItem(QString::number(item->getMoradorId())));
        tabelaReservas_->setItem(linha, 2, new QTableWidgetItem(QString::number(item->getAreaId())));
        tabelaReservas_->setItem(linha, 3, new QTableWidgetItem(QString::fromStdString(item->getData())));
        tabelaReservas_->setItem(linha, 4, new QTableWidgetItem(QString::fromStdString(item->getHoraInicio())));
        tabelaReservas_->setItem(linha, 5, new QTableWidgetItem(QString::fromStdString(item->getHoraFim())));
        tabelaReservas_->setItem(linha, 6, new QTableWidgetItem(item->getStatus() == StatusReserva::ATIVA ? "Ativa" : "Cancelada"));
        tabelaReservas_->setItem(linha, 7, new QTableWidgetItem(QString::number(item->getValor(), 'f', 2)));
    }
}
