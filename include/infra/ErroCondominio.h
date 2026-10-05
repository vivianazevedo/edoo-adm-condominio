#pragma once 
#include <string> 
#include <exception> 


class ErroCondominio : public std::exception { // classe herda publicamente da bibilioteca 

    private:

        std::string mensagem_; 

    public: 
        explicit ErroCondominio(std::string mensagem); // construtor 

        const char* what() const noexcept override; // pega o objeto de erro e transforma em texto legivel

};

class ErroValidacao : public ErroCondominio { // erro de CPF invalido

    public:
        explicit ErroValidacao (const std::string& mensagem); //construtor 

};


class ErroRegraNegocio : public ErroCondominio { //regra de negocio invalidada

    public:
        explicit ErroRegraNegocio (const std::string& mensagem); // construtor 

};


class ErroBanco : public ErroCondominio {

    public:
        explicit ErroBanco (const std::string& mensagem); //construtor 
};