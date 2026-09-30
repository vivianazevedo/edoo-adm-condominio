#pragma once
#include <memory>
#include <string>
#include <vector>
#include "modelo/Pessoa.h"
#include "modelo/Funcionario.h"
#include "repositorio/IRepositorio.h"

using namespace std;

class FuncionarioService {

    private:
        IRepositorio<Pessoa>& repoPessoa_;

    public:
        explicit FuncionarioService(IRepositorio<Pessoa>& repoPessoa);

        // cadastra o funcionario e devolve o id gerado
        int cadastrar(const string& nome, const string& cpf, const string& telefone,
                    Cargo cargo, const string& turno, const string& dataAdmissao);

        // devolve todos os funcionarios
        vector<unique_ptr<Pessoa>> listar();

        // altera um funcionario que ja existe
        void editar(int id, const string& nome, const string& cpf,
                    const string& telefone, Cargo cargo, const string& turno,
                    const string& dataAdmissao);

        // remove o funcionario
        void remover(int id);


};