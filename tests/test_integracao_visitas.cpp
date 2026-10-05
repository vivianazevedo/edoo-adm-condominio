// teste de integracao do RepositorioVisita: grava, le, registra saida e remove uma visita
#include "infra/Database.h"
#include "modelo/Apartamento.h"
#include "modelo/Funcionario.h"
#include "modelo/Visitante.h"
#include "modelo/Visita.h"
#include "repositorio/RepositorioApartamento.h"
#include "repositorio/RepositorioPessoa.h"
#include "repositorio/RepositorioVisita.h"

#include <stdexcept>
#include <string>

namespace {

// joga excecao se a condicao for falsa, e assim que o teste falha
void exigir(bool condicao, const char* mensagem) {
    if (!condicao) throw std::runtime_error(mensagem);
}

}  // namespace

int main(int argc, char* argv[]) {
    if (argc != 2) throw std::invalid_argument("Informe o caminho de sql/schema.sql");

    // um banco em memoria permite verificar as relacoes sem alterar dados reais
    Database::instancia(":memory:", argv[1]);
    RepositorioApartamento apartamentos;
    RepositorioPessoa pessoas;
    RepositorioVisita visitas;

    const int apartamentoId = apartamentos.inserir(Apartamento("A", "101", 1));
    const int visitanteId = pessoas.inserir(
        Visitante("Bruno Lima", "11144477735", "81988888888"));
    const int porteiroId = pessoas.inserir(
        Funcionario("Carla Dias", "12345678909", "81977777777",
                    Cargo::Porteiro, "manha", "2026-09-01"));

    const int visitaId = visitas.inserir(
        Visita(visitanteId, apartamentoId, porteiroId, "2026-09-30 10:15"));
    auto visitaLida = visitas.buscarPorId(visitaId);
    exigir(visitaLida != nullptr, "Visita nao encontrada apos cadastro");
    exigir(visitaLida->getVisitanteId() == visitanteId &&
               visitaLida->getApartamentoId() == apartamentoId &&
               visitaLida->getRegistradoPor() == porteiroId &&
               visitaLida->estaAberta(),
           "Dados da visita mudaram ao ler o SQLite");
    exigir(visitas.listar().size() == 1, "Listagem de visitas incorreta");

    visitaLida->setSaida("2026-09-30 11:20");
    exigir(visitas.atualizar(*visitaLida), "Saida da visita nao foi atualizada");
    auto visitaEncerrada = visitas.buscarPorId(visitaId);
    exigir(visitaEncerrada != nullptr && !visitaEncerrada->estaAberta() &&
               visitaEncerrada->getSaida() == "2026-09-30 11:20",
           "Saida da visita nao persistiu");

    exigir(visitas.remover(visitaId), "Visita nao foi removida");
    exigir(visitas.buscarPorId(visitaId) == nullptr, "Visita removida ainda existe");
    exigir(pessoas.remover(visitanteId), "Visitante nao foi removido");
    exigir(pessoas.remover(porteiroId), "Porteiro nao foi removido");
    exigir(apartamentos.remover(apartamentoId), "Apartamento nao foi removido");
}
