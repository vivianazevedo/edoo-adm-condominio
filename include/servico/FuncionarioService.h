#pragma once
#include <memory>
#include <string>
#include <vector>
#include "modelo/Pessoa.h"
#include "modelo/Funcionario.h"
#include "repositorio/IRepositorio.h"


// regras de negocio dos funcionarios (o cpf nao pode repetir em nenhuma pessoa)
class FuncionarioService {

    private:
        // funcionario e guardado como Pessoa, no repositorio de pessoas
        IRepositorio<Pessoa>& repoPessoa_;

    public:
        explicit FuncionarioService(IRepositorio<Pessoa>& repoPessoa);

        // cadastra o funcionario e devolve o id gerado
        int cadastrar(const std::string& nome, const std::string& cpf, const std::string& telefone,
                    Cargo cargo, const std::string& turno, const std::string& dataAdmissao);

        // devolve todos os funcionarios
        std::vector<std::unique_ptr<Pessoa>> listar();

        // altera um funcionario que ja existe
        void editar(int id, const std::string& nome, const std::string& cpf,
                    const std::string& telefone, Cargo cargo, const std::string& turno,
                    const std::string& dataAdmissao);

        // remove o funcionario
        void remover(int id);


};