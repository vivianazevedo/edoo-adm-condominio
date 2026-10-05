#pragma once
#include <memory>
#include <string>
#include <vector>
#include "modelo/Pessoa.h"
#include "modelo/Visita.h"
#include "repositorio/IRepositorio.h"


class Visita; //declara a classe visita (o include de cima ja traz ela tambem)

// regras da portaria: cadastrar visitante, registrar entrada (so porteiro) e saida, e consultar o historico
class VisitaService {

    private:
        // usa o repositorio de pessoas (visitante e porteiro) e o de visitas
        IRepositorio<Pessoa>& repoPessoa_;
        IRepositorio<Visita>& repoVisita_;
        
    public:
        VisitaService(IRepositorio<Pessoa>& repoPessoa,
                    IRepositorio<Visita>& repoVisita);

        // cadastra o visitante e devolve o id gerado
        int cadastrarVisitante(const std::string& nome, const std::string& cpf,
                            const std::string& telefone);

        // registra a entrada e devolve o id da visita.
        // porteiroId e o funcionario escolhido no formulario (rn05)
        int registrarEntrada(int visitanteId, int apartamentoId, int porteiroId);

        // registra a saida de uma visita aberta (rn06)
        void registrarSaida(int visitaId);

        // visitas que ainda nao tem saida
        std::vector<std::unique_ptr<Visita>> listarAbertas();

        // historico de visitas de um apartamento
        std::vector<std::unique_ptr<Visita>> historicoPorApartamento(int apartamentoId);


};