#include "servico/VisitaService.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <utility>
#include "modelo/Visitante.h"
#include "modelo/Funcionario.h"
#include "infra/ErroCondominio.h"

using namespace std;  // permitido em .cpp (so e proibido nos .h)

namespace {

// devolve a data e hora de agora no formato AAAA-MM-DD HH:MM (o mesmo da visita)
string agoraTexto() {
    time_t tempo = chrono::system_clock::to_time_t(chrono::system_clock::now());
    tm local{};
// localtime_s e do windows e localtime_r e do linux e mac, cada sistema tem a sua
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

// guarda as referencias dos repositorios
VisitaService::VisitaService(IRepositorio<Pessoa>& repoPessoa,
                             IRepositorio<Visita>& repoVisita)
    : repoPessoa_(repoPessoa), repoVisita_(repoVisita) {}

// valida pelo construtor, confere se o cpf ja existe (RN07) e salva
int VisitaService::cadastrarVisitante(const string& nome, const string& cpf,
                                      const string& telefone) {
    // o construtor valida nome, cpf e telefone (ErroValidacao)
    Visitante novo(nome, cpf, telefone);

    if (cpfEmUso(repoPessoa_, novo.cpf())) {
        throw ErroRegraNegocio("ja existe uma pessoa com o CPF " + novo.cpf());
    }
    return repoPessoa_.inserir(novo);
}

// confere o visitante e o porteiro (RN05) e grava a entrada com a hora de agora
// (se o apartamento nao existir o banco recusa e vira ErroRegraNegocio)
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
    // visitante e porteiro ja foram conferidos acima, entao se o banco recusar por chave
    // estrangeira o que sobra e o apartamento (traduz para mensagem de regra de negocio)
    try {
        return repoVisita_.inserir(visita);
    } catch (const ErroBanco& e) {
        if (string(e.what()).find("FOREIGN KEY") != string::npos) {
            throw ErroRegraNegocio("apartamento " + to_string(apartamentoId) + " nao encontrado");
        }
        throw;
    }
}

// a visita tem que existir e estar aberta, grava a saida com a hora de agora
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

// pega todas as visitas e fica so com as que nao tem saida
vector<unique_ptr<Visita>> VisitaService::listarAbertas() {
    vector<unique_ptr<Visita>> abertas;
    for (auto& visita : repoVisita_.listar()) {
        if (visita->estaAberta()) {
            abertas.push_back(std::move(visita));
        }
    }
    return abertas;
}

// pega todas as visitas e fica so com as daquele apartamento
vector<unique_ptr<Visita>> VisitaService::historicoPorApartamento(int apartamentoId) {
    vector<unique_ptr<Visita>> historico;
    for (auto& visita : repoVisita_.listar()) {
        if (visita->getApartamentoId() == apartamentoId) {
            historico.push_back(std::move(visita));
        }
    }
    return historico;
}
