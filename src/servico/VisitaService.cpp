#include "servico/VisitaService.h"
#include "repositorio/IRepositorioVisita.h"
#include "repositorio/IRepositorioPessoa.h"
#include "repositorio/IRepositorioApartamento.h"
#include "modelo/Visitante.h"
#include "modelo/Funcionario.h"
#include "modelo/Visita.h"
#include "infra/ErroCondominio.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

VisitaService::VisitaService(std::shared_ptr<IRepositorioVisita> repoVisita,
                             std::shared_ptr<IRepositorioPessoa> repoPessoa,
                             std::shared_ptr<IRepositorioApartamento> repoApto)
    : repoVisita_(repoVisita), repoPessoa_(repoPessoa), repoApto_(repoApto) {}

int VisitaService::cadastrarVisitante(const std::string& nome, const std::string& cpf, const std::string& telefone) {
    if (nome.empty() || cpf.empty()) {
        throw ErroValidacao("Nome e CPF do visitante são obrigatórios.");
    }
    
    // Instancia objeto Visitante (POO - Herança de Pessoa)
    auto visitante = std::make_unique<Visitante>(0, nome, cpf, telefone);
    return repoPessoa_->inserir(std::move(visitante));
}

int VisitaService::registrarEntrada(int visitanteId, int apartamentoId, int porteiroId) {
    if (visitanteId <= 0 || apartamentoId <= 0 || porteiroId <= 0) {
        throw ErroValidacao("IDs de visitante, apartamento e porteiro devem ser válidos.");
    }

    // Valida se quem está registrando é um funcionário/porteiro
    auto pessoa = repoPessoa_->buscarPorId(porteiroId);
    if (!pessoa || dynamic_cast<Funcionario*>(pessoa.get()) == nullptr) {
        throw ErroRegraNegocio("Apenas funcionários registrados (porteiros) podem registrar entrada.");
    }

    // Pega a data e hora atual do sistema
    auto agora = std::chrono::system_clock::now();
    std::time_t tempo = std::chrono::system_clock::to_time_t(agora);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&tempo), "%Y-%m-%d %H:%M:%S");
    std::string dataHoraAtual = ss.str();

    Visita visita(0, visitanteId, apartamentoId, porteiroId, dataHoraAtual, "");
    return repoVisita_->inserir(visita);
}

void VisitaService::registrarSaida(int visitaId) {
    if (visitaId <= 0) {
        throw ErroValidacao("ID de visita inválido.");
    }

    auto visita = repoVisita_->buscarPorId(visitaId);
    if (!visita) {
        throw ErroRegraNegocio("Visita não encontrada.");
    }

    auto agora = std::chrono::system_clock::now();
    std::time_t tempo = std::chrono::system_clock::to_time_t(agora);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&tempo), "%Y-%m-%d %H:%M:%S");

    visita->setDataHoraSaida(ss.str());
    repoVisita_->atualizar(*visita);
}

std::vector<Visita> VisitaService::listarAbertas() {
    return repoVisita_->listarAbertas();
}

std::vector<Visita> VisitaService::historicoPorApartamento(int apartamentoId) {
    return repoVisita_->listarPorApartamento(apartamentoId);
}