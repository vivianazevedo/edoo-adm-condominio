#pragma once 
#include <string> 
#include "infra/ErroCondominio.h"

using namespace std;

class Pessoa {

    private:
        static string textoObrigatorio(const string& valor, const string& campo); // remove espacos das pontas e recusa texto sem nada 
        int id = 0;
        string nome_;
        string cpf_; 
        string telefone_; 

    public:
        Pessoa (string nome, string cpf, string telefone, int id = 0);
        virtual ~Pessoa() = default; 

        virtual string tipo () const = 0;

        int id() const { return id_; }
        const string& nome() const { return nome_; }
        const string& cpf() const { return cpf_; }
        const string& telefone() const { return telefone_; }

        static string normalizarCpf(const string& cpf);
        static bool cpfValido(const string& cpf);

        void setId(int id); // lanca errovalidacao se o valor for invalaido
        void setNome(const string& nome);
        void setCpf(const string& cpf);
        void setTelefone(const string& telefone);

};