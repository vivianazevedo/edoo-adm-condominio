#pragma once 
#include <string> 
#include "infra/ErroCondominio.h"


class Pessoa {

    private:
        int id_ = 0;
        std::string nome_;
        std::string cpf_; 
        std::string telefone_; 

    protected:
        static std::string textoObrigatorio(const std::string& valor, const std::string& campo); // remove espacos das pontas e recusa texto sem nada 
        static std::string dataIsoValida(const std::string& data, const std::string& campo); // confere data (morador e funcionario usam por isso deixo em pessoa)
       
    public:
        Pessoa (std::string nome, std::string cpf, std::string telefone, int id = 0);
        virtual ~Pessoa() = default; 

        virtual std::string tipo () const = 0;

        int id() const { return id_; }
        const std::string& nome() const { return nome_; }
        const std::string& cpf() const { return cpf_; }
        const std::string& telefone() const { return telefone_; }

        static std::string normalizarCpf(const std::string& cpf);
        static bool cpfValido(const std::string& cpf);

        void setId(int id); // lanca errovalidacao se o valor for invalaido
        void setNome(const std::string& nome);
        void setCpf(const std::string& cpf);
        void setTelefone(const std::string& telefone);

};