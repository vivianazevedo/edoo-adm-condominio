#pragma once
#include <memory>
#include <string>
#include <vector>
#include "modelo/Apartamento.h"
#include "repositorio/IRepositorio.h"


class ApartamentoService {

    private:
        IRepositorio<Apartamento>& repo_;
        
    public:
        
        explicit ApartamentoService(IRepositorio<Apartamento>& repo);// recebe o repositorio por referencia 

        int cadastrar(const std::string& bloco, const std::string& numero, int andar);// cadastra e devolve o id gerado

        std::vector<std::unique_ptr<Apartamento>> listar();// devolve todos os apartamentos

        std::unique_ptr<Apartamento> buscar(int id);// busca por id, devolve nullptr se nao achar

        // altera um apartamento que ja existe
        void editar(int id, const std::string& bloco, const std::string& numero, int andar);

        // remove o apartamento (nao pode se tiver morador)
        void remover(int id);


};