#include "servico/VisitaService.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <utility>
#include "modelo/Visitante.h"
#include "modelo/Funcionario.h"
#include "infra/ErroCondominio.h"

namespace {

// devolve a data e hora de agora no formato AAAA-MM-DD HH:MM (o mesmo da visita)
string agoraTexto() {
    time_t tempo = chrono::system_clock::to_time_t(chrono::system_clock::now());
    tm local{};
#if defined(_WIN32)
    localtime_s(&local, &tempo);
#else
    localtime_r(&tempo, &local);
#endif
    ostringstream texto;
    texto << put_time(&local, "%Y-%m-%d %H:%M");
    return texto.str();
}

// rn07: o cpf tem que ser unico entre todas as pessoas
bool cpfEmUso(IRepositorio<Pessoa>& repo, const string& cpf) {
    for (const auto& pessoa : repo.listar()) {
        if (pessoa->cpf() == cpf) {
            return true;
        }
    }
    return false;
}

}  // namespace

VisitaService::VisitaService(IRepositorio<Pessoa>& repoPessoa,
                             IRepositorio<Visita>& repoVisita)
    : repoPessoa_(repoPessoa), repoVisita_(repoVisita) {}

int VisitaService::cadastrarVisitante(const string& nome, const string& cpf,
                                      const string& telefone) {
    // o construtor valida nome, cpf e telefone (ErroValidacao)
    Visitante novo(nome, cpf, telefone);

    if (cpfEmUso(repoPessoa_, novo.cpf())) {
        throw ErroRegraNegocio("ja existe uma pessoa com o CPF " + novo.cpf());
    }
    return repoPessoa_.inserir(novo);
}

int VisitaService::registrarEntrada(int visitanteId, int apartamentoId, int porteiroId) {
    // o visitante precisa existir e ser mesmo um visitante
    unique_ptr<Pessoa> visitante = repoPessoa_.buscarPorId(visitanteId);
    if (dynamic_cast<Visitante*>(visitante.get()) == nullptr) {
        throw ErroRegraNegocio("visitante " + to_string(visitanteId) + " nao encontrado");
    }

    // rn05: so funcionario com cargo de porteiro registra entrada
    unique_ptr<Pessoa> pessoa = repoPessoa_.buscarPorId(porteiroId);
    Funcionario* porteiro = dynamic_cast<Funcionario*>(pessoa.get());
    if (porteiro == nullptr || !porteiro->ehPorteiro()) {
        throw ErroRegraNegocio("so um funcionario com cargo de porteiro registra visitas");
    }

    // o construtor de visita valida os ids e a data (ErroValidacao)
    Visita visita(visitanteId, apartamentoId, porteiroId, agoraTexto());
    return repoVisita_.inserir(visita);
}

void VisitaService::registrarSaida(int visitaId) {
    unique_ptr<Visita> visita = repoVisita_.buscarPorId(visitaId);
    if (!visita) {
        throw ErroRegraNegocio("visita " + to_string(visitaId) + " nao encontrada");
    }
    if (!visita->estaAberta()) {
        throw ErroRegraNegocio("essa visita ja tem saida registrada");
    }

    // rn06: o setSaida recusa saida antes da entrada (ErroValidacao)
    visita->setSaida(agoraTexto());
    repoVisita_.atualizar(*visita);
}

vector<unique_ptr<Visita>> VisitaService::listarAbertas() {
    vector<unique_ptr<Visita>> abertas;
    for (auto& visita : repoVisita_.listar()) {
        if (visita->estaAberta()) {
            abertas.push_back(std::move(visita));
        }
    }
    return abertas;
}

vector<unique_ptr<Visita>> VisitaService::historicoPorApartamento(int apartamentoId) {
    vector<unique_ptr<Visita>> historico;
    for (auto& visita : repoVisita_.listar()) {
        if (visita->getApartamentoId() == apartamentoId) {
            historico.push_back(std::move(visita));
        }
    }
    return historico;
}
