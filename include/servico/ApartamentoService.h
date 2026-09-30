#pragma once
#include <memory>
#include <string>
#include <vector>
#include "modelo/Apartamento.h"
#include "repositorio/IRepositorio.h"

using namespace std;

class ApartamentoService {

    private:
        IRepositorio<Apartamento>& repo_;
        
    public:
        
        explicit ApartamentoService(IRepositorio<Apartamento>& repo);// recebe o repositorio por referencia 

        int cadastrar(const string& bloco, const string& numero, int andar);// cadastra e devolve o id gerado

        vector<unique_ptr<Apartamento>> listar();// devolve todos os apartamentos

        unique_ptr<Apartamento> buscar(int id);// busca por id, devolve nullptr se nao achar

        // altera um apartamento que ja existe
        void editar(int id, const string& bloco, const string& numero, int andar);

        // remove o apartamento (nao pode se tiver morador)
        void remover(int id);


};