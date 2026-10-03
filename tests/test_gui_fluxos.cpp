#include "infra/Database.h"
#include "interface/MenuPrincipal.h"
#include "interface/TelaMoradores.h"
#include "interface/TelaReservas.h"
#include "interface/TelaVisitas.h"
#include "modelo/Morador.h"
#include "modelo/Funcionario.h"
#include "modelo/Visitante.h"
#include "repositorio/RepositorioApartamento.h"
#include "repositorio/RepositorioAreaComum.h"
#include "repositorio/RepositorioPessoa.h"
#include "repositorio/RepositorioReserva.h"
#include "repositorio/RepositorioVisita.h"

#include <QApplication>
#include <QComboBox>
#include <QDateEdit>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QTableWidget>
#include <QTimer>

#include <iostream>
#include <stdexcept>

namespace {

void exigir(bool condicao, const char* mensagem) {
    if (!condicao) throw std::runtime_error(mensagem);
}

template <typename T>
T* achar(QWidget* raiz, const char* nome) {
    auto* encontrado = raiz->findChild<T*>(nome);
    exigir(encontrado != nullptr, nome);
    return encontrado;
}

void selecionar(QComboBox* combo, int id) {
    const int indice = combo->findData(id);
    exigir(indice >= 0, "ID nao apareceu no formulario");
    combo->setCurrentIndex(indice);
}

}  // namespace

int main(int argc, char* argv[]) {
    if (argc != 2) throw std::invalid_argument("Informe sql/schema.sql");
    QApplication app(argc, argv);
    Database::instancia(":memory:", argv[1]);

    int dialogosErro = 0;
    QTimer vigilante;
    QObject::connect(&vigilante, &QTimer::timeout, [&] {
        for (QWidget* janela : QApplication::topLevelWidgets()) {
            if (auto* mensagem = qobject_cast<QMessageBox*>(janela)) {
                ++dialogosErro;
                mensagem->accept();
            }
        }
    });
    vigilante.start(50);

    MenuPrincipal janela;
    auto* telaPessoas = achar<TelaMoradores>(&janela, "");
    auto* telaReservas = achar<TelaReservas>(&janela, "");
    auto* telaVisitas = achar<TelaVisitas>(&janela, "");
    RepositorioApartamento apartamentos;
    RepositorioPessoa pessoas;
    RepositorioAreaComum areas;
    RepositorioReserva reservas;
    RepositorioVisita visitas;

    achar<QLineEdit>(telaPessoas, "aptoBloco")->setText("A");
    achar<QLineEdit>(telaPessoas, "aptoNumero")->setText("101");
    achar<QSpinBox>(telaPessoas, "aptoAndar")->setValue(1);
    achar<QPushButton>(telaPessoas, "aptoCadastrar")->click();
    const auto aptos = apartamentos.listar();
    exigir(aptos.size() == 1, "GUI nao cadastrou apartamento");
    const int aptoId = aptos.front()->getId();
    achar<QTableWidget>(telaPessoas, "aptosTabela")->setCurrentCell(0, 0);
    achar<QLineEdit>(telaPessoas, "aptoNumero")->setText("102");
    achar<QPushButton>(telaPessoas, "aptoEditar")->click();
    exigir(apartamentos.buscarPorId(aptoId)->getNumero() == "102",
           "GUI nao editou apartamento");
    achar<QLineEdit>(telaPessoas, "aptoBloco")->setText("B");
    achar<QLineEdit>(telaPessoas, "aptoNumero")->setText("201");
    achar<QPushButton>(telaPessoas, "aptoCadastrar")->click();
    achar<QTableWidget>(telaPessoas, "aptosTabela")->setCurrentCell(1, 0);
    achar<QPushButton>(telaPessoas, "aptoRemover")->click();
    exigir(apartamentos.listar().size() == 1, "GUI nao removeu apartamento livre");

    achar<QLineEdit>(telaPessoas, "moradorNome")->setText("Ana Souza");
    achar<QLineEdit>(telaPessoas, "moradorCpf")->setText("52998224725");
    achar<QLineEdit>(telaPessoas, "moradorTelefone")->setText("81999999999");
    selecionar(achar<QComboBox>(telaPessoas, "moradorApartamento"), aptoId);
    achar<QPushButton>(telaPessoas, "moradorCadastrar")->click();
    int moradorId = 0;
    for (const auto& pessoa : pessoas.listar()) {
        if (dynamic_cast<Morador*>(pessoa.get())) moradorId = pessoa->id();
    }
    exigir(moradorId > 0, "GUI nao cadastrou morador");
    achar<QTableWidget>(telaPessoas, "moradoresTabela")->setCurrentCell(0, 0);
    achar<QLineEdit>(telaPessoas, "moradorTelefone")->setText("81911111111");
    selecionar(achar<QComboBox>(telaPessoas, "moradorApartamento"), aptoId);
    achar<QPushButton>(telaPessoas, "moradorEditar")->click();
    exigir(pessoas.buscarPorId(moradorId)->telefone() == "81911111111",
           "GUI nao editou morador");
    achar<QLineEdit>(telaPessoas, "moradorNome")->setText("Joao Teste");
    achar<QLineEdit>(telaPessoas, "moradorCpf")->setText("93541134780");
    selecionar(achar<QComboBox>(telaPessoas, "moradorApartamento"), aptoId);
    achar<QPushButton>(telaPessoas, "moradorCadastrar")->click();
    achar<QTableWidget>(telaPessoas, "moradoresTabela")->setCurrentCell(1, 0);
    achar<QPushButton>(telaPessoas, "moradorRemover")->click();
    exigir(pessoas.listar().size() == 1, "GUI nao removeu morador");
    achar<QTableWidget>(telaPessoas, "aptosTabela")->setCurrentCell(0, 0);
    achar<QPushButton>(telaPessoas, "aptoRemover")->click();
    exigir(dialogosErro == 1 && apartamentos.listar().size() == 1,
           "GUI nao mostrou erro ao tentar remover apartamento ocupado");
    dialogosErro = 0;

    achar<QLineEdit>(telaReservas, "areaNome")->setText("Salao Azul");
    achar<QSpinBox>(telaReservas, "areaCapacidade")->setValue(80);
    achar<QDoubleSpinBox>(telaReservas, "areaTaxa")->setValue(250);
    achar<QPushButton>(telaReservas, "areaCadastrar")->click();
    const auto listaAreas = areas.listar();
    exigir(listaAreas.size() == 1, "GUI nao cadastrou area");
    const int areaId = listaAreas.front()->getId();
    achar<QTableWidget>(telaReservas, "areasTabela")->setCurrentCell(0, 0);
    achar<QLineEdit>(telaReservas, "areaNome")->setText("Salao Renovado");
    achar<QPushButton>(telaReservas, "areaEditar")->click();
    exigir(areas.buscarPorId(areaId)->getNome() == "Salao Renovado",
           "GUI nao editou area");
    achar<QComboBox>(telaReservas, "areaTipo")->setCurrentIndex(1);
    achar<QLineEdit>(telaReservas, "areaNome")->setText("Piscina");
    achar<QSpinBox>(telaReservas, "areaCapacidade")->setValue(30);
    achar<QDoubleSpinBox>(telaReservas, "areaTaxa")->setValue(0);
    achar<QPushButton>(telaReservas, "areaCadastrar")->click();
    achar<QTableWidget>(telaReservas, "areasTabela")->setCurrentCell(1, 0);
    achar<QPushButton>(telaReservas, "areaRemover")->click();
    exigir(areas.listar().size() == 1, "GUI nao removeu area sem reservas");

    telaReservas->atualizar();
    selecionar(achar<QComboBox>(telaReservas, "reservaMorador"), moradorId);
    selecionar(achar<QComboBox>(telaReservas, "reservaArea"), areaId);
    achar<QDateEdit>(telaReservas, "reservaData")->setDate(QDate::currentDate().addDays(3));
    achar<QSpinBox>(telaReservas, "reservaConvidados")->setValue(4);
    achar<QPushButton>(telaReservas, "reservaCriar")->click();
    exigir(reservas.listar().size() == 1, "GUI nao criou reserva");
    selecionar(achar<QComboBox>(telaReservas, "reservaMorador"), moradorId);
    selecionar(achar<QComboBox>(telaReservas, "reservaArea"), areaId);
    achar<QPushButton>(telaReservas, "reservaCriar")->click();
    exigir(dialogosErro == 1 && reservas.listar().size() == 1,
           "GUI nao informou conflito de reserva");
    dialogosErro = 0;
    achar<QTableWidget>(telaReservas, "reservasTabela")->setCurrentCell(0, 0);
    achar<QPushButton>(telaReservas, "reservaCancelar")->click();
    exigir(reservas.listar().front()->getStatus() == StatusReserva::CANCELADA,
           "GUI nao cancelou reserva");
    selecionar(achar<QComboBox>(telaReservas, "reservaMorador"), moradorId);
    selecionar(achar<QComboBox>(telaReservas, "reservaArea"), areaId);
    achar<QPushButton>(telaReservas, "reservaCriar")->click();
    exigir(reservas.listar().size() == 2, "GUI nao liberou horario cancelado");

    achar<QLineEdit>(telaVisitas, "funcionarioNome")->setText("Carla Dias");
    achar<QLineEdit>(telaVisitas, "funcionarioCpf")->setText("12345678909");
    achar<QLineEdit>(telaVisitas, "funcionarioTelefone")->setText("81977777777");
    achar<QLineEdit>(telaVisitas, "funcionarioTurno")->setText("manha");
    achar<QLineEdit>(telaVisitas, "funcionarioCpf")->setText("52998224725");
    achar<QPushButton>(telaVisitas, "funcionarioCadastrar")->click();
    exigir(dialogosErro == 1 && pessoas.listar().size() == 1,
           "GUI nao informou CPF duplicado");
    dialogosErro = 0;
    achar<QLineEdit>(telaVisitas, "funcionarioCpf")->setText("12345678909");
    achar<QPushButton>(telaVisitas, "funcionarioCadastrar")->click();
    int porteiroId = 0;
    for (const auto& pessoa : pessoas.listar()) {
        if (pessoa->tipo() == "funcionario") porteiroId = pessoa->id();
    }
    exigir(porteiroId > 0, "GUI nao cadastrou funcionario");
    achar<QTableWidget>(telaVisitas, "funcionariosTabela")->setCurrentCell(0, 0);
    achar<QLineEdit>(telaVisitas, "funcionarioTurno")->setText("tarde");
    achar<QPushButton>(telaVisitas, "funcionarioEditar")->click();
    auto porteiro = pessoas.buscarPorId(porteiroId);
    exigir(dynamic_cast<Funcionario*>(porteiro.get())->turno() == "tarde",
           "GUI nao editou funcionario");
    achar<QLineEdit>(telaVisitas, "funcionarioNome")->setText("Diego Lima");
    achar<QLineEdit>(telaVisitas, "funcionarioCpf")->setText("93541134780");
    achar<QComboBox>(telaVisitas, "funcionarioCargo")->setCurrentIndex(1);
    achar<QPushButton>(telaVisitas, "funcionarioCadastrar")->click();
    achar<QTableWidget>(telaVisitas, "funcionariosTabela")->setCurrentCell(1, 0);
    achar<QPushButton>(telaVisitas, "funcionarioRemover")->click();
    exigir(pessoas.listar().size() == 2, "GUI nao removeu funcionario sem visitas");

    achar<QLineEdit>(telaVisitas, "visitanteNome")->setText("Bruno Lima");
    achar<QLineEdit>(telaVisitas, "visitanteCpf")->setText("11144477735");
    achar<QLineEdit>(telaVisitas, "visitanteTelefone")->setText("81988888888");
    achar<QPushButton>(telaVisitas, "visitanteCadastrar")->click();
    int visitanteId = 0;
    for (const auto& pessoa : pessoas.listar()) {
        if (dynamic_cast<Visitante*>(pessoa.get())) visitanteId = pessoa->id();
    }
    exigir(visitanteId > 0, "GUI nao cadastrou visitante");

    selecionar(achar<QComboBox>(telaVisitas, "visitaVisitante"), visitanteId);
    selecionar(achar<QComboBox>(telaVisitas, "visitaApartamento"), aptoId);
    selecionar(achar<QComboBox>(telaVisitas, "visitaPorteiro"), porteiroId);
    achar<QPushButton>(telaVisitas, "visitaEntrada")->click();
    const auto entradas = visitas.listar();
    exigir(entradas.size() == 1 && entradas.front()->estaAberta(),
           "GUI nao registrou entrada da visita");
    auto* tabela = achar<QTableWidget>(telaVisitas, "visitasTabela");
    tabela->setCurrentCell(0, 0);
    achar<QPushButton>(telaVisitas, "visitaSaida")->click();
    const auto saidas = visitas.listar();
    exigir(saidas.size() == 1 && !saidas.front()->estaAberta(),
           "GUI nao registrou saida da visita");
    exigir(dialogosErro == 0, "Uma operacao valida mostrou dialogo de erro");
    std::cout << "GUI-U02/U03/U04: PASSOU\n";
}
