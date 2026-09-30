#pragma once
#include <memory>
#include <string>
#include <vector>
#include "modelo/Pessoa.h"
#include "modelo/Morador.h"
#include "modelo/Apartamento.h"
#include "repositorio/IRepositorio.h"

using namespace std;

class MoradorService {

    private:
        IRepositorio<Pessoa>& repoPessoa_;
        IRepositorio<Apartamento>& repoApto_;
        
    public:
        
        MoradorService(IRepositorio<Pessoa>& repoPessoa,// precisa do repo de apartamento pra ver se o apartamento existe
                    IRepositorio<Apartamento>& repoApto);

        // cadastra o morador num apartamento e devolve o id gerado
        int cadastrar(const string& nome, const string& cpf, const string& telefone,
                    int apartamentoId, TipoOcupacao tipoOcupacao,
                    const string& dataEntrada);

        vector<unique_ptr<Pessoa>> listar();// devolve todos os moradores

        vector<unique_ptr<Pessoa>> listarPorApartamento(int apartamentoId); // devolve so os moradores apartamento x 

        // altera um morador que ja existe
        void editar(int id, const string& nome, const string& cpf,
                    const string& telefone, int apartamentoId,
                    TipoOcupacao tipoOcupacao, const string& dataEntrada);

        // remove o morador 
        void remover(int id);

};