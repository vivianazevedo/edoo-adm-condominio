#pragma once 
#include <string> 
#include <exception> 

using namespace std; 

class ErroCondominio : public exception { // classe herda publicamente da bibilioteca 

    private:

        string mensagem_; 

    public: 
        explicit ErroCondominio(string mensagem); // construtor 

        const char* what() const noexcept override; // pega o objeto de erro e transforma em texto legivel

};

class ErroValidacao : public ErroCondominio { // erro de CPF invalido

    public:
        explicit ErroValidacao (const string& mensagem); //construtor 

};


class ErroRegraNegocio : public ErroCondominio { //regra de negocio invalidada

    public:
        explicit ErroRegraNegocio (const string& mensagem); // construtor 

};


class ErroBanco : public ErroCondominio {

    public:
        explicit ErroBanco (const string& mensagem); //construtor 
};