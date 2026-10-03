#include "interface/TelaMoradores.h"

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
    tabela->setMinimumHeight(170);
}

}  // namespace

TelaMoradores::TelaMoradores(QWidget* parent)
    : QWidget(parent), apartamentos_(repoApartamentos_),
      moradores_(repoPessoas_, repoApartamentos_) {
    auto* pagina = new QWidget(this);
    auto* layout = new QVBoxLayout(pagina);

    auto* grupoApartamentos = new QGroupBox("Apartamentos", pagina);
    auto* layoutApartamentos = new QVBoxLayout(grupoApartamentos);
    auto* formularioApartamento = new QFormLayout;
    bloco_ = new QLineEdit(grupoApartamentos);
    numero_ = new QLineEdit(grupoApartamentos);
    andar_ = new QSpinBox(grupoApartamentos);
    bloco_->setObjectName("aptoBloco");
    numero_->setObjectName("aptoNumero");
    andar_->setObjectName("aptoAndar");
    andar_->setRange(0, 200);
    formularioApartamento->addRow("Bloco", bloco_);
    formularioApartamento->addRow("Numero", numero_);
    formularioApartamento->addRow("Andar", andar_);
    layoutApartamentos->addLayout(formularioApartamento);
    auto* botoesApartamento = new QHBoxLayout;
    auto* cadastrarApartamento = new QPushButton("Cadastrar apartamento", grupoApartamentos);
    cadastrarApartamento->setObjectName("aptoCadastrar");
    auto* editarApartamento = new QPushButton("Editar selecionado", grupoApartamentos);
    auto* removerApartamento = new QPushButton("Remover selecionado", grupoApartamentos);
    editarApartamento->setObjectName("aptoEditar");
    removerApartamento->setObjectName("aptoRemover");
    botoesApartamento->addWidget(cadastrarApartamento);
    botoesApartamento->addWidget(editarApartamento);
    botoesApartamento->addWidget(removerApartamento);
    layoutApartamentos->addLayout(botoesApartamento);
    tabelaApartamentos_ = new QTableWidget(grupoApartamentos);
    tabelaApartamentos_->setObjectName("aptosTabela");
    prepararTabela(tabelaApartamentos_, {"ID", "Bloco", "Numero", "Andar"});
    layoutApartamentos->addWidget(tabelaApartamentos_);
    layout->addWidget(grupoApartamentos);

    auto* grupoMoradores = new QGroupBox("Moradores", pagina);
    auto* layoutMoradores = new QVBoxLayout(grupoMoradores);
    auto* formularioMorador = new QFormLayout;
    nome_ = new QLineEdit(grupoMoradores);
    cpf_ = new QLineEdit(grupoMoradores);
    telefone_ = new QLineEdit(grupoMoradores);
    apartamento_ = new QComboBox(grupoMoradores);
    nome_->setObjectName("moradorNome");
    cpf_->setObjectName("moradorCpf");
    telefone_->setObjectName("moradorTelefone");
    apartamento_->setObjectName("moradorApartamento");
    ocupacao_ = new QComboBox(grupoMoradores);
    ocupacao_->addItems({"Proprietario", "Inquilino", "Dependente"});
    entrada_ = new QDateEdit(QDate::currentDate(), grupoMoradores);
    entrada_->setCalendarPopup(true);
    entrada_->setDisplayFormat("dd/MM/yyyy");
    formularioMorador->addRow("Nome", nome_);
    formularioMorador->addRow("CPF", cpf_);
    formularioMorador->addRow("Telefone", telefone_);
    formularioMorador->addRow("Apartamento", apartamento_);
    formularioMorador->addRow("Ocupacao", ocupacao_);
    formularioMorador->addRow("Data de entrada", entrada_);
    layoutMoradores->addLayout(formularioMorador);
    auto* botoesMorador = new QHBoxLayout;
    auto* cadastrarMorador = new QPushButton("Cadastrar morador", grupoMoradores);
    cadastrarMorador->setObjectName("moradorCadastrar");
    auto* editarMorador = new QPushButton("Editar selecionado", grupoMoradores);
    auto* removerMorador = new QPushButton("Remover selecionado", grupoMoradores);
    editarMorador->setObjectName("moradorEditar");
    removerMorador->setObjectName("moradorRemover");
    botoesMorador->addWidget(cadastrarMorador);
    botoesMorador->addWidget(editarMorador);
    botoesMorador->addWidget(removerMorador);
    layoutMoradores->addLayout(botoesMorador);
    tabelaMoradores_ = new QTableWidget(grupoMoradores);
    tabelaMoradores_->setObjectName("moradoresTabela");
    prepararTabela(tabelaMoradores_, {"ID", "Nome", "CPF", "Telefone", "Apartamento", "Ocupacao"});
    layoutMoradores->addWidget(tabelaMoradores_);
    layout->addWidget(grupoMoradores);

    auto* rolagem = new QScrollArea(this);
    rolagem->setWidgetResizable(true);
    rolagem->setWidget(pagina);
    auto* externo = new QVBoxLayout(this);
    externo->addWidget(rolagem);

    connect(cadastrarApartamento, &QPushButton::clicked, this, [this] {
        executarNaTela(this, [this] {
            apartamentos_.cadastrar(bloco_->text().trimmed().toStdString(),
                                    numero_->text().trimmed().toStdString(), andar_->value());
            atualizar();
        });
    });
    connect(editarApartamento, &QPushButton::clicked, this, [this] {
        executarNaTela(this, [this] {
            const int id = idSelecionado(tabelaApartamentos_);
            apartamentos_.editar(id, bloco_->text().trimmed().toStdString(),
                                 numero_->text().trimmed().toStdString(), andar_->value());
            atualizar();
        });
    });
    connect(removerApartamento, &QPushButton::clicked, this, [this] {
        executarNaTela(this, [this] {
            apartamentos_.remover(idSelecionado(tabelaApartamentos_));
            atualizar();
        });
    });
    connect(tabelaApartamentos_, &QTableWidget::cellClicked, this, [this](int linha, int) {
        bloco_->setText(tabelaApartamentos_->item(linha, 1)->text());
        numero_->setText(tabelaApartamentos_->item(linha, 2)->text());
        andar_->setValue(tabelaApartamentos_->item(linha, 3)->text().toInt());
    });

    connect(cadastrarMorador, &QPushButton::clicked, this, [this] {
        executarNaTela(this, [this] {
            moradores_.cadastrar(nome_->text().trimmed().toStdString(),
                                 cpf_->text().trimmed().toStdString(),
                                 telefone_->text().trimmed().toStdString(),
                                 apartamento_->currentData().toInt(),
                                 static_cast<TipoOcupacao>(ocupacao_->currentIndex()),
                                 entrada_->date().toString("yyyy-MM-dd").toStdString());
            atualizar();
        });
    });
    connect(editarMorador, &QPushButton::clicked, this, [this] {
        executarNaTela(this, [this] {
            moradores_.editar(idSelecionado(tabelaMoradores_),
                             nome_->text().trimmed().toStdString(),
                             cpf_->text().trimmed().toStdString(),
                             telefone_->text().trimmed().toStdString(),
                             apartamento_->currentData().toInt(),
                             static_cast<TipoOcupacao>(ocupacao_->currentIndex()),
                             entrada_->date().toString("yyyy-MM-dd").toStdString());
            atualizar();
        });
    });
    connect(removerMorador, &QPushButton::clicked, this, [this] {
        executarNaTela(this, [this] {
            moradores_.remover(idSelecionado(tabelaMoradores_));
            atualizar();
        });
    });
    connect(tabelaMoradores_, &QTableWidget::cellClicked, this, [this](int linha, int) {
        const int id = tabelaMoradores_->item(linha, 0)->text().toInt();
        auto pessoa = repoPessoas_.buscarPorId(id);
        auto* morador = dynamic_cast<Morador*>(pessoa.get());
        if (!morador) return;
        nome_->setText(QString::fromStdString(morador->nome()));
        cpf_->setText(QString::fromStdString(morador->cpf()));
        telefone_->setText(QString::fromStdString(morador->telefone()));
        apartamento_->setCurrentIndex(apartamento_->findData(morador->apartamentoId()));
        ocupacao_->setCurrentIndex(static_cast<int>(morador->tipoOcupacao()));
        entrada_->setDate(QDate::fromString(QString::fromStdString(morador->dataEntrada()),
                                          "yyyy-MM-dd"));
    });

    atualizar();
}

void TelaMoradores::atualizar() {
    const auto apartamentos = apartamentos_.listar();
    tabelaApartamentos_->setRowCount(0);
    apartamento_->clear();
    apartamento_->addItem("Selecione...", -1);
    for (const auto& apto : apartamentos) {
        const int linha = tabelaApartamentos_->rowCount();
        tabelaApartamentos_->insertRow(linha);
        tabelaApartamentos_->setItem(linha, 0, new QTableWidgetItem(QString::number(apto->getId())));
        tabelaApartamentos_->setItem(linha, 1, new QTableWidgetItem(QString::fromStdString(apto->getBloco())));
        tabelaApartamentos_->setItem(linha, 2, new QTableWidgetItem(QString::fromStdString(apto->getNumero())));
        tabelaApartamentos_->setItem(linha, 3, new QTableWidgetItem(QString::number(apto->getAndar())));
        apartamento_->addItem(QString::fromStdString(apto->descricao()), apto->getId());
    }

    const auto moradores = moradores_.listar();
    tabelaMoradores_->setRowCount(0);
    for (const auto& pessoa : moradores) {
        const auto* morador = dynamic_cast<const Morador*>(pessoa.get());
        if (!morador) continue;
        const int linha = tabelaMoradores_->rowCount();
        tabelaMoradores_->insertRow(linha);
        tabelaMoradores_->setItem(linha, 0, new QTableWidgetItem(QString::number(morador->id())));
        tabelaMoradores_->setItem(linha, 1, new QTableWidgetItem(QString::fromStdString(morador->nome())));
        tabelaMoradores_->setItem(linha, 2, new QTableWidgetItem(QString::fromStdString(morador->cpf())));
        tabelaMoradores_->setItem(linha, 3, new QTableWidgetItem(QString::fromStdString(morador->telefone())));
        tabelaMoradores_->setItem(linha, 4, new QTableWidgetItem(QString::number(morador->apartamentoId())));
        tabelaMoradores_->setItem(linha, 5, new QTableWidgetItem(ocupacao_->itemText(static_cast<int>(morador->tipoOcupacao()))));
    }
}
