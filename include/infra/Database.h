#pragma once

#include <string>

// declaracao antecipada, assim nao precisa incluir o sqlite3.h aqui
struct sqlite3;

// guarda a unica conexao com o sqlite que o sistema inteiro usa (padrao singleton)
// final = ninguem pode herdar dessa classe
class Database final {
public:
    // a primeira chamada abre o banco e le o schema, as outras so devolvem a mesma instancia
    static Database& instancia(
        const std::string& caminhoBanco = "condominio.db",
        const std::string& caminhoSchema = "sql/schema.sql");

    // da a conexao pros repositorios usarem, quem fecha ela e a propria Database
    sqlite3* conexao() const noexcept;

    // roda um ou mais comandos sql, se der erro joga ErroBanco
    void executar(const std::string& sql);

    // copia e move desligados, assim nunca existem duas conexoes
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;
    Database(Database&&) = delete;
    Database& operator=(Database&&) = delete;

private:
    // construtor privado: so o instancia() consegue criar o objeto
    Database(const std::string& caminhoBanco, const std::string& caminhoSchema);
    // fecha a conexao quando o objeto e destruido
    ~Database();

    // abre o arquivo do banco (ou cria se nao existir)
    void abrir(const std::string& caminhoBanco);
    // cria as tabelas lendo o schema.sql, so se o banco estiver vazio
    void inicializarSchema(const std::string& caminhoSchema);
    // diz se o banco ja tem alguma tabela
    bool possuiTabelas() const;

    // ponteiro da conexao aberta (nullptr = nao tem conexao)
    sqlite3* conexao_ = nullptr;
};
