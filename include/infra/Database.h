#pragma once

#include <string>

struct sqlite3;

// Mantem a unica conexao SQLite usada pela aplicacao.
class Database final {
public:
    // A primeira chamada define o arquivo do banco e o schema da aplicacao.
    static Database& instancia(
        const std::string& caminhoBanco = "condominio.db",
        const std::string& caminhoSchema = "sql/schema.sql");

    // Devolve a conexao para as classes de repositorio, sem transferir sua posse.
    sqlite3* conexao() const noexcept;

    // Executa um ou mais comandos SQL e lanca ErroBanco em caso de falha.
    void executar(const std::string& sql);

    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;
    Database(Database&&) = delete;
    Database& operator=(Database&&) = delete;

private:
    Database(const std::string& caminhoBanco, const std::string& caminhoSchema);
    ~Database();

    void abrir(const std::string& caminhoBanco);
    void inicializarSchema(const std::string& caminhoSchema);
    bool possuiTabelas() const;

    sqlite3* conexao_ = nullptr;
};
