#pragma once 
#include <string> 
#include "infra/ErroCondominio.h"


// classe base abstrata de Morador, Visitante e Funcionario
// guarda os dados que todo mundo tem: id, nome, cpf e telefone
class Pessoa {

    private:
        int id_ = 0;
        std::string nome_;
        std::string cpf_; 
        std::string telefone_; 

    protected:
        static std::string textoObrigatorio(const std::string& valor, const std::string& campo); // remove espacos das pontas e recusa texto sem nada 
        static std::string dataIsoValida(const std::string& data, const std::string& campo); // confere o formato AAAA-MM-DD e se a data existe de verdade, fica aqui porque morador e funcionario usam
       
    public:
        // o construtor passa tudo pelos setters, assim ja valida na criacao
        Pessoa (std::string nome, std::string cpf, std::string telefone, int id = 0);
        // destrutor virtual pra apagar certo pelo ponteiro da classe base
        virtual ~Pessoa() = default; 

        // virtual puro: cada subclasse diz se e morador, visitante ou funcionario (polimorfismo)
        virtual std::string tipo () const = 0;

        // getters
        int id() const { return id_; }
        const std::string& nome() const { return nome_; }
        const std::string& cpf() const { return cpf_; }
        const std::string& telefone() const { return telefone_; }

        // tira tudo que nao for numero do cpf
        static std::string normalizarCpf(const std::string& cpf);
        // confere o tamanho e os digitos verificadores do cpf
        static bool cpfValido(const std::string& cpf);

        void setId(int id); // joga ErroValidacao se o valor for invalido (id negativo)
        // os outros setters tambem validam e jogam ErroValidacao se estiver errado
        void setNome(const std::string& nome);
        void setCpf(const std::string& cpf);
        void setTelefone(const std::string& telefone);

};