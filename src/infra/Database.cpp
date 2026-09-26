#include "infra/Database.h"

#include "infra/ErroCondominio.h"

#include <fstream>
#include <sstream>
#include <utility>

#include <sqlite3.h>

namespace {

std::string lerArquivo(const std::string& caminho) {
    std::ifstream arquivo(caminho);
    if (!arquivo) {
        throw ErroBanco("Nao foi possivel abrir o arquivo de schema: " + caminho);
    }

    std::ostringstream conteudo;
    conteudo << arquivo.rdbuf();
    return conteudo.str();
}

}  // namespace

Database& Database::instancia(
    const std::string& caminhoBanco,
    const std::string& caminhoSchema) {
    static Database unicaInstancia(caminhoBanco, caminhoSchema);
    return unicaInstancia;
}

Database::Database(
    const std::string& caminhoBanco,
    const std::string& caminhoSchema) {
    abrir(caminhoBanco);

    try {
        executar("PRAGMA foreign_keys = ON;");
        inicializarSchema(caminhoSchema);
    } catch (...) {
        sqlite3_close(conexao_);
        conexao_ = nullptr;
        throw;
    }
}

Database::~Database() {
    if (conexao_ != nullptr) {
        sqlite3_close(conexao_);
    }
}

sqlite3* Database::conexao() const noexcept {
    return conexao_;
}

void Database::abrir(const std::string& caminhoBanco) {
    const int resultado = sqlite3_open(caminhoBanco.c_str(), &conexao_);
    if (resultado == SQLITE_OK) {
        return;
    }

    const std::string mensagem = conexao_ != nullptr
        ? sqlite3_errmsg(conexao_)
        : "erro desconhecido";

    if (conexao_ != nullptr) {
        sqlite3_close(conexao_);
        conexao_ = nullptr;
    }

    throw ErroBanco("Nao foi possivel abrir o banco SQLite: " + mensagem);
}

void Database::executar(const std::string& sql) {
    char* mensagemErro = nullptr;
    const int resultado = sqlite3_exec(
        conexao_, sql.c_str(), nullptr, nullptr, &mensagemErro);

    if (resultado == SQLITE_OK) {
        return;
    }

    std::string mensagem = mensagemErro != nullptr
        ? mensagemErro
        : sqlite3_errmsg(conexao_);
    sqlite3_free(mensagemErro);

    throw ErroBanco("Falha ao executar SQL: " + mensagem);
}

bool Database::possuiTabelas() const {
    constexpr const char* consulta =
        "SELECT 1 FROM sqlite_master "
        "WHERE type = 'table' AND name NOT LIKE 'sqlite_%' LIMIT 1;";

    sqlite3_stmt* comando = nullptr;
    if (sqlite3_prepare_v2(conexao_, consulta, -1, &comando, nullptr) != SQLITE_OK) {
        throw ErroBanco(
            "Falha ao verificar a estrutura do banco: " +
            std::string(sqlite3_errmsg(conexao_)));
    }

    const int resultado = sqlite3_step(comando);
    sqlite3_finalize(comando);

    if (resultado == SQLITE_ROW) {
        return true;
    }
    if (resultado == SQLITE_DONE) {
        return false;
    }

    throw ErroBanco(
        "Falha ao verificar a estrutura do banco: " +
        std::string(sqlite3_errmsg(conexao_)));
}

void Database::inicializarSchema(const std::string& caminhoSchema) {
    if (!possuiTabelas()) {
        executar(lerArquivo(caminhoSchema));
    }
}
