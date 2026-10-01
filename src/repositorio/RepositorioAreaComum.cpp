#include "repositorio/RepositorioAreaComum.h"

#include "infra/FabricaAreaComum.h"
#include "modelo/Churrasqueira.h"
#include "modelo/Piscina.h"
#include "modelo/SalaoFestas.h"
#include "SqliteComando.h"

#include <memory>

namespace {

std::unique_ptr<AreaComum> montar(const SqliteComando& comando) {
    const int id = comando.inteiroEm(0);
    const std::string nome = comando.textoEm(1);
    const std::string tipo = comando.textoEm(2);
    const int capacidade = comando.inteiroEm(3);
    const double taxa = comando.realEm(4);
    const std::string abertura = comando.textoEm(5);
    const std::string fechamento = comando.textoEm(6);

    FabricaAreaComum fabrica;
    fabrica.registrarSalaoFestas([&] {
        return std::make_unique<SalaoFestas>(id, nome, capacidade, taxa,
                                             abertura, fechamento);
    });
    fabrica.registrarPiscina([&] {
        return std::make_unique<Piscina>(id, nome, capacidade, taxa,
                                         abertura, fechamento);
    });
    fabrica.registrarChurrasqueira([&] {
        return std::make_unique<Churrasqueira>(id, nome, capacidade, taxa,
                                               abertura, fechamento);
    });
    return fabrica.criar(tipo);
}

}  // namespace

int RepositorioAreaComum::inserir(const AreaComum& entidade) {
    SqliteComando comando(
        "INSERT INTO area_comum (nome, tipo, capacidade, taxa_base, "
        "hora_abertura, hora_fechamento) VALUES (?, ?, ?, ?, ?, ?)");
    comando.texto(1, entidade.getNome());
    comando.texto(2, entidade.getTipo());
    comando.inteiro(3, entidade.getCapacidade());
    comando.real(4, entidade.getTaxaBase());
    comando.texto(5, entidade.getHoraAbertura());
    comando.texto(6, entidade.getHoraFechamento());
    comando.executar();
    return comando.ultimoId();
}

std::unique_ptr<AreaComum> RepositorioAreaComum::buscarPorId(int id) {
    SqliteComando comando(
        "SELECT id, nome, tipo, capacidade, taxa_base, hora_abertura, "
        "hora_fechamento FROM area_comum WHERE id = ?");
    comando.inteiro(1, id);
    return comando.proxima() ? montar(comando) : nullptr;
}

std::vector<std::unique_ptr<AreaComum>> RepositorioAreaComum::listar() {
    SqliteComando comando(
        "SELECT id, nome, tipo, capacidade, taxa_base, hora_abertura, "
        "hora_fechamento FROM area_comum ORDER BY id");
    std::vector<std::unique_ptr<AreaComum>> resultado;
    while (comando.proxima()) resultado.push_back(montar(comando));
    return resultado;
}

bool RepositorioAreaComum::atualizar(const AreaComum& entidade) {
    SqliteComando comando(
        "UPDATE area_comum SET nome = ?, tipo = ?, capacidade = ?, taxa_base = ?, "
        "hora_abertura = ?, hora_fechamento = ? WHERE id = ?");
    comando.texto(1, entidade.getNome());
    comando.texto(2, entidade.getTipo());
    comando.inteiro(3, entidade.getCapacidade());
    comando.real(4, entidade.getTaxaBase());
    comando.texto(5, entidade.getHoraAbertura());
    comando.texto(6, entidade.getHoraFechamento());
    comando.inteiro(7, entidade.getId());
    comando.executar();
    return comando.alteradas() > 0;
}

bool RepositorioAreaComum::remover(int id) {
    SqliteComando comando("DELETE FROM area_comum WHERE id = ?");
    comando.inteiro(1, id);
    comando.executar();
    return comando.alteradas() > 0;
}
