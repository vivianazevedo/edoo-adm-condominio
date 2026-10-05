#pragma once
#include <memory>
#include <string>
#include <vector>
#include "modelo/Pessoa.h"
#include "modelo/Morador.h"
#include "modelo/Apartamento.h"
#include "repositorio/IRepositorio.h"


// regras de negocio dos moradores: o apartamento tem que existir e o cpf nao pode repetir
class MoradorService {

    private:
        // morador e guardado como Pessoa, no repositorio de pessoas
        IRepositorio<Pessoa>& repoPessoa_;
        IRepositorio<Apartamento>& repoApto_;
        
    public:
        
        MoradorService(IRepositorio<Pessoa>& repoPessoa,// precisa do repo de apartamento pra ver se o apartamento existe
                    IRepositorio<Apartamento>& repoApto);

        // cadastra o morador num apartamento e devolve o id gerado
        int cadastrar(const std::string& nome, const std::string& cpf, const std::string& telefone,
                    int apartamentoId, TipoOcupacao tipoOcupacao,
                    const std::string& dataEntrada);

        std::vector<std::unique_ptr<Pessoa>> listar();// devolve todos os moradores

        std::vector<std::unique_ptr<Pessoa>> listarPorApartamento(int apartamentoId); // devolve so os moradores apartamento x 

        // altera um morador que ja existe
        void editar(int id, const std::string& nome, const std::string& cpf,
                    const std::string& telefone, int apartamentoId,
                    TipoOcupacao tipoOcupacao, const std::string& dataEntrada);

        // remove o morador 
        void remover(int id);

};