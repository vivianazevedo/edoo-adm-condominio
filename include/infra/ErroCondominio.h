#pragma once 
#include <string> 
#include <exception> 


class ErroCondominio : public std::exception { // classe base de todos os erros do sistema, herda de exception da biblioteca padrao 

    private:

        // o texto do erro
        std::string mensagem_; 

    public: 
        explicit ErroCondominio(std::string mensagem); // construtor 

        const char* what() const noexcept override; // pega o objeto de erro e transforma em texto legivel

};

class ErroValidacao : public ErroCondominio { // dado invalido (cpf errado, campo vazio, data fora do formato...)

    public:
        explicit ErroValidacao (const std::string& mensagem); //construtor 

};


class ErroRegraNegocio : public ErroCondominio { // uma regra de negocio foi quebrada (ex: reserva em horario ocupado)

    public:
        explicit ErroRegraNegocio (const std::string& mensagem); // construtor 

};


// erro que vem do sqlite (falha ao abrir, preparar ou rodar um sql)
class ErroBanco : public ErroCondominio {

    public:
        explicit ErroBanco (const std::string& mensagem); //construtor 
};