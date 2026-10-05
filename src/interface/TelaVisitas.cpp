#include "interface/TelaVisitas.h"

#include "interface/UtilQt.h"
#include "modelo/Funcionario.h"
#include "modelo/Visitante.h"

#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

namespace {

// deixa a tabela no padrao: colunas, linha inteira selecionavel, uma por vez e sem editar a celula
void prepararTabela(QTableWidget* tabela, const QStringList& colunas) {
    tabela->setColumnCount(colunas.size());
    tabela->setHorizontalHeaderLabels(colunas);
    tabela->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tabela->setSelectionBehavior(QAbstractItemView::SelectRows);
    tabela->setSelectionMode(QAbstractItemView::SingleSelection);
    tabela->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tabela->setMinimumHeight(170);
    tabela->setAlternatingRowColors(true);
}

}  // namespace

// monta a tela inteira e liga os botoes aos services
TelaVisitas::TelaVisitas(QWidget* parent)
    : QWidget(parent), funcionarios_(repoPessoas_), visitas_(repoPessoas_, repoVisitas_) {
    auto* pagina = new QWidget(this);
    auto* layout = new QVBoxLayout(pagina);

    // parte dos funcionarios: formulario, botoes e tabela
    auto* grupoFuncionarios = new QGroupBox("Funcionarios", pagina);
    auto* layoutFuncionarios = new QVBoxLayout(grupoFuncionarios);
    auto* formularioFuncionario = new QFormLayout;
    nomeFuncionario_ = new QLineEdit(grupoFuncionarios);
    cpfFuncionario_ = new QLineEdit(grupoFuncionarios);
    telefoneFuncionario_ = new QLineEdit(grupoFuncionarios);
    cargo_ = new QComboBox(grupoFuncionarios);
    nomeFuncionario_->setObjectName("funcionarioNome");
    cpfFuncionario_->setObjectName("funcionarioCpf");
    telefoneFuncionario_->setObjectName("funcionarioTelefone");
    cargo_->setObjectName("funcionarioCargo");
    cargo_->addItems({"Porteiro", "Zelador", "Faxineiro", "Administrador"});
    turno_ = new QLineEdit(grupoFuncionarios);
    turno_->setObjectName("funcionarioTurno");
    admissao_ = new QDateEdit(QDate::currentDate(), grupoFuncionarios);
    admissao_->setCalendarPopup(true);
    admissao_->setDisplayFormat("dd/MM/yyyy");
    formularioFuncionario->addRow("Nome", nomeFuncionario_);
    formularioFuncionario->addRow("CPF", cpfFuncionario_);
    formularioFuncionario->addRow("Telefone", telefoneFuncionario_);
    formularioFuncionario->addRow("Cargo", cargo_);
    formularioFuncionario->addRow("Turno", turno_);
    formularioFuncionario->addRow("Admissao", admissao_);
    layoutFuncionarios->addLayout(formularioFuncionario);
    auto* botoesFuncionario = new QHBoxLayout;
    auto* cadastrarFuncionario = new QPushButton("Cadastrar funcionario", grupoFuncionarios);
    cadastrarFuncionario->setObjectName("funcionarioCadastrar");
    auto* editarFuncionario = new QPushButton("Editar selecionado", grupoFuncionarios);
    auto* removerFuncionario = new QPushButton("Remover selecionado", grupoFuncionarios);
    editarFuncionario->setObjectName("funcionarioEditar");
    removerFuncionario->setObjectName("funcionarioRemover");
    botoesFuncionario->addWidget(cadastrarFuncionario);
    botoesFuncionario->addWidget(editarFuncionario);
    botoesFuncionario->addWidget(removerFuncionario);
    layoutFuncionarios->addLayout(botoesFuncionario);
    tabelaFuncionarios_ = new QTableWidget(grupoFuncionarios);
    tabelaFuncionarios_->setObjectName("funcionariosTabela");
    prepararTabela(tabelaFuncionarios_, {"ID", "Nome", "CPF", "Telefone", "Cargo", "Turno"});
    layoutFuncionarios->addWidget(tabelaFuncionarios_);
    layout->addWidget(grupoFuncionarios);

    // parte dos visitantes: formulario, botao e tabela
    auto* grupoVisitantes = new QGroupBox("Visitantes", pagina);
    auto* layoutVisitantes = new QVBoxLayout(grupoVisitantes);
    auto* formularioVisitante = new QFormLayout;
    nomeVisitante_ = new QLineEdit(grupoVisitantes);
    cpfVisitante_ = new QLineEdit(grupoVisitantes);
    telefoneVisitante_ = new QLineEdit(grupoVisitantes);
    nomeVisitante_->setObjectName("visitanteNome");
    cpfVisitante_->setObjectName("visitanteCpf");
    telefoneVisitante_->setObjectName("visitanteTelefone");
    formularioVisitante->addRow("Nome", nomeVisitante_);
    formularioVisitante->addRow("CPF", cpfVisitante_);
    formularioVisitante->addRow("Telefone", telefoneVisitante_);
    layoutVisitantes->addLayout(formularioVisitante);
    auto* cadastrarVisitante = new QPushButton("Cadastrar visitante", grupoVisitantes);
    cadastrarVisitante->setObjectName("visitanteCadastrar");
    layoutVisitantes->addWidget(cadastrarVisitante);
    tabelaVisitantes_ = new QTableWidget(grupoVisitantes);
    prepararTabela(tabelaVisitantes_, {"ID", "Nome", "CPF", "Telefone"});
    layoutVisitantes->addWidget(tabelaVisitantes_);
    layout->addWidget(grupoVisitantes);

    // parte das visitas: escolhe visitante, apartamento e porteiro pra registrar entrada e saida
    auto* grupoVisitas = new QGroupBox("Entrada, saida e historico de visitas", pagina);
    auto* layoutVisitas = new QVBoxLayout(grupoVisitas);
    auto* formularioVisita = new QFormLayout;
    visitante_ = new QComboBox(grupoVisitas);
    apartamento_ = new QComboBox(grupoVisitas);
    porteiro_ = new QComboBox(grupoVisitas);
    visitante_->setObjectName("visitaVisitante");
    apartamento_->setObjectName("visitaApartamento");
    porteiro_->setObjectName("visitaPorteiro");
    formularioVisita->addRow("Visitante", visitante_);
    formularioVisita->addRow("Apartamento de destino", apartamento_);
    formularioVisita->addRow("Porteiro responsavel", porteiro_);
    layoutVisitas->addLayout(formularioVisita);
    auto* botoesVisita = new QHBoxLayout;
    auto* registrarEntrada = new QPushButton("Registrar entrada", grupoVisitas);
    auto* registrarSaida = new QPushButton("Registrar saida selecionada", grupoVisitas);
    registrarEntrada->setObjectName("visitaEntrada");
    registrarSaida->setObjectName("visitaSaida");
    botoesVisita->addWidget(registrarEntrada);
    botoesVisita->addWidget(registrarSaida);
    layoutVisitas->addLayout(botoesVisita);
    // combo pra filtrar o historico por apartamento
    filtroApartamento_ = new QComboBox(grupoVisitas);
    layoutVisitas->addWidget(filtroApartamento_);
    tabelaVisitas_ = new QTableWidget(grupoVisitas);
    tabelaVisitas_->setObjectName("visitasTabela");
    prepararTabela(tabelaVisitas_, {"ID", "Visitante", "Apartamento", "Porteiro", "Entrada", "Saida"});
    layoutVisitas->addWidget(tabelaVisitas_);
    layout->addWidget(grupoVisitas);

    // poe tudo dentro de uma area com barra de rolagem
    auto* rolagem = new QScrollArea(this);
    rolagem->setWidgetResizable(true);
    rolagem->setWidget(pagina);
    auto* externo = new QVBoxLayout(this);
    externo->addWidget(rolagem);

    // ligacao dos botoes (signal clicked) com as acoes
    // o executarNaTela mostra uma janela com o erro se o service jogar excecao
    connect(cadastrarFuncionario, &QPushButton::clicked, this, [this] {
        executarNaTela(this, [this] {
            funcionarios_.cadastrar(nomeFuncionario_->text().trimmed().toStdString(),
                                    cpfFuncionario_->text().trimmed().toStdString(),
                                    telefoneFuncionario_->text().trimmed().toStdString(),
                                    static_cast<Cargo>(cargo_->currentIndex()),
                                    turno_->text().trimmed().toStdString(),
                                    admissao_->date().toString("yyyy-MM-dd").toStdString());
            atualizar();
        });
    });
    connect(editarFuncionario, &QPushButton::clicked, this, [this] {
        executarNaTela(this, [this] {
            funcionarios_.editar(idSelecionado(tabelaFuncionarios_),
                                 nomeFuncionario_->text().trimmed().toStdString(),
                                 cpfFuncionario_->text().trimmed().toStdString(),
                                 telefoneFuncionario_->text().trimmed().toStdString(),
                                 static_cast<Cargo>(cargo_->currentIndex()),
                                 turno_->text().trimmed().toStdString(),
                                 admissao_->date().toString("yyyy-MM-dd").toStdString());
            atualizar();
        });
    });
    connect(removerFuncionario, &QPushButton::clicked, this, [this] {
        executarNaTela(this, [this] {
            funcionarios_.remover(idSelecionado(tabelaFuncionarios_));
            atualizar();
        });
    });
    // clicar numa linha busca o funcionario no banco e preenche o formulario
    connect(tabelaFuncionarios_, &QTableWidget::cellClicked, this, [this](int linha, int) {
        const int id = tabelaFuncionarios_->item(linha, 0)->text().toInt();
        auto pessoa = repoPessoas_.buscarPorId(id);
        auto* funcionario = dynamic_cast<Funcionario*>(pessoa.get());
        if (!funcionario) return;
        nomeFuncionario_->setText(QString::fromStdString(funcionario->nome()));
        cpfFuncionario_->setText(QString::fromStdString(funcionario->cpf()));
        telefoneFuncionario_->setText(QString::fromStdString(funcionario->telefone()));
        cargo_->setCurrentIndex(static_cast<int>(funcionario->cargo()));
        turno_->setText(QString::fromStdString(funcionario->turno()));
        admissao_->setDate(QDate::fromString(QString::fromStdString(funcionario->dataAdmissao()),
                                           "yyyy-MM-dd"));
    });

    // depois de cadastrar o visitante, ja deixa ele escolhido no combo da entrada
    connect(cadastrarVisitante, &QPushButton::clicked, this, [this] {
        executarNaTela(this, [this] {
            const int id = visitas_.cadastrarVisitante(
                nomeVisitante_->text().trimmed().toStdString(),
                cpfVisitante_->text().trimmed().toStdString(),
                telefoneVisitante_->text().trimmed().toStdString());
            atualizar();
            visitante_->setCurrentIndex(visitante_->findData(id));
        });
    });
    connect(registrarEntrada, &QPushButton::clicked, this, [this] {
        executarNaTela(this, [this] {
            visitas_.registrarEntrada(visitante_->currentData().toInt(),
                                      apartamento_->currentData().toInt(),
                                      porteiro_->currentData().toInt());
            atualizarVisitas();
        });
    });
    connect(registrarSaida, &QPushButton::clicked, this, [this] {
        executarNaTela(this, [this] {
            visitas_.registrarSaida(idSelecionado(tabelaVisitas_));
            atualizarVisitas();
        });
    });
    connect(filtroApartamento_, &QComboBox::currentIndexChanged, this,
            [this](int) { executarNaTela(this, [this] { atualizarVisitas(); }); });

    // carrega os dados na primeira vez que a tela abre
    atualizar();
}

// recarrega funcionarios, visitantes, os combos e a tabela de visitas
void TelaVisitas::atualizar() {
    const auto pessoas = repoPessoas_.listar();
    tabelaFuncionarios_->setRowCount(0);
    tabelaVisitantes_->setRowCount(0);
    visitante_->clear();
    porteiro_->clear();
    visitante_->addItem("Selecione...", -1);
    porteiro_->addItem("Selecione...", -1);
    for (const auto& pessoa : pessoas) {
        if (const auto* funcionario = dynamic_cast<const Funcionario*>(pessoa.get())) {
            const int linha = tabelaFuncionarios_->rowCount();
            tabelaFuncionarios_->insertRow(linha);
            tabelaFuncionarios_->setItem(linha, 0, new QTableWidgetItem(QString::number(funcionario->id())));
            tabelaFuncionarios_->setItem(linha, 1, new QTableWidgetItem(QString::fromStdString(funcionario->nome())));
            tabelaFuncionarios_->setItem(linha, 2, new QTableWidgetItem(QString::fromStdString(funcionario->cpf())));
            tabelaFuncionarios_->setItem(linha, 3, new QTableWidgetItem(QString::fromStdString(funcionario->telefone())));
            tabelaFuncionarios_->setItem(linha, 4, new QTableWidgetItem(cargo_->itemText(static_cast<int>(funcionario->cargo()))));
            tabelaFuncionarios_->setItem(linha, 5, new QTableWidgetItem(QString::fromStdString(funcionario->turno())));
            // so porteiro entra na lista de porteiros (a regra de verdade esta no VisitaService)
            if (funcionario->ehPorteiro()) {
                porteiro_->addItem(QString::fromStdString(funcionario->nome()), funcionario->id());
            }
        } else if (dynamic_cast<const Visitante*>(pessoa.get()) != nullptr) {
            const int linha = tabelaVisitantes_->rowCount();
            tabelaVisitantes_->insertRow(linha);
            tabelaVisitantes_->setItem(linha, 0, new QTableWidgetItem(QString::number(pessoa->id())));
            tabelaVisitantes_->setItem(linha, 1, new QTableWidgetItem(QString::fromStdString(pessoa->nome())));
            tabelaVisitantes_->setItem(linha, 2, new QTableWidgetItem(QString::fromStdString(pessoa->cpf())));
            tabelaVisitantes_->setItem(linha, 3, new QTableWidgetItem(QString::fromStdString(pessoa->telefone())));
            visitante_->addItem(QString::fromStdString(pessoa->nome()), pessoa->id());
        }
    }

    // guarda o filtro que estava escolhido pra nao perder quando recarregar o combo
    const int filtroAnterior = filtroApartamento_->currentIndex() >= 0
        ? filtroApartamento_->currentData().toInt() : -1;
    apartamento_->clear();
    // bloqueia os sinais enquanto recarrega, senao o combo chamaria atualizarVisitas varias vezes
    filtroApartamento_->blockSignals(true);
    filtroApartamento_->clear();
    apartamento_->addItem("Selecione...", -1);
    filtroApartamento_->addItem("Historico: todos os apartamentos", -1);
    for (const auto& apto : repoApartamentos_.listar()) {
        const QString descricao = QString::fromStdString(apto->descricao());
        apartamento_->addItem(descricao, apto->getId());
        filtroApartamento_->addItem(descricao, apto->getId());
    }
    filtroApartamento_->setCurrentIndex(filtroApartamento_->findData(filtroAnterior));
    filtroApartamento_->blockSignals(false);
    atualizarVisitas();
}

// mostra o historico do apartamento filtrado, ou todas as visitas se nao tiver filtro
void TelaVisitas::atualizarVisitas() {
    const int apartamentoId = filtroApartamento_->currentData().toInt();
    auto eventos = apartamentoId > 0
        ? visitas_.historicoPorApartamento(apartamentoId)
        : repoVisitas_.listar();
    tabelaVisitas_->setRowCount(0);
    for (const auto& visita : eventos) {
        const int linha = tabelaVisitas_->rowCount();
        tabelaVisitas_->insertRow(linha);
        tabelaVisitas_->setItem(linha, 0, new QTableWidgetItem(QString::number(visita->getId())));
        tabelaVisitas_->setItem(linha, 1, new QTableWidgetItem(QString::number(visita->getVisitanteId())));
        tabelaVisitas_->setItem(linha, 2, new QTableWidgetItem(QString::number(visita->getApartamentoId())));
        tabelaVisitas_->setItem(linha, 3, new QTableWidgetItem(QString::number(visita->getRegistradoPor())));
        tabelaVisitas_->setItem(linha, 4, new QTableWidgetItem(QString::fromStdString(visita->getEntrada())));
        tabelaVisitas_->setItem(linha, 5, new QTableWidgetItem(visita->estaAberta()
            ? "Em aberto" : QString::fromStdString(visita->getSaida())));
    }
}
