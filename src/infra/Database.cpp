#include "infra/Database.h"

#include "infra/ErroCondominio.h"

#include <fstream>
#include <sstream>
#include <sqlite3.h>

namespace {

// le o arquivo inteiro e devolve como texto (usado pra ler o schema.sql)
std::string lerArquivo(const std::string& caminho) {
    std::ifstream arquivo(caminho);
    if (!arquivo) throw ErroBanco("Nao foi possivel abrir o schema: " + caminho);
    std::ostringstream conteudo;
    conteudo << arquivo.rdbuf();
    return conteudo.str();
}

}  // namespace

// o static dentro da funcao so e criado na primeira chamada, depois e sempre o mesmo objeto
Database& Database::instancia(const std::string& caminhoBanco,
                              const std::string& caminhoSchema) {
    static Database unicaInstancia(caminhoBanco, caminhoSchema);
    return unicaInstancia;
}

// abre o banco, liga as chaves estrangeiras e cria as tabelas se precisar
Database::Database(const std::string& caminhoBanco,
                   const std::string& caminhoSchema) {
    abrir(caminhoBanco);
    try {
        executar("PRAGMA foreign_keys = ON;");
        inicializarSchema(caminhoSchema);
    } catch (...) {
        // se deu erro no meio, fecha a conexao antes de repassar o erro
        // (se o construtor falha o destrutor nao roda, por isso fecha aqui)
        sqlite3_close(conexao_);
        conexao_ = nullptr;
        throw;
    }
}

// fecha a conexao quando o programa termina
Database::~Database() {
    if (conexao_ != nullptr) sqlite3_close(conexao_);
}

// so devolve o ponteiro da conexao
sqlite3* Database::conexao() const noexcept { return conexao_; }

// abre ou cria o arquivo do banco, se falhar joga ErroBanco com a mensagem do sqlite
void Database::abrir(const std::string& caminhoBanco) {
    const int resultado = sqlite3_open(caminhoBanco.c_str(), &conexao_);
    if (resultado == SQLITE_OK) return;
    const std::string mensagem = conexao_ != nullptr
        ? sqlite3_errmsg(conexao_) : "erro desconhecido";
    if (conexao_ != nullptr) {
        sqlite3_close(conexao_);
        conexao_ = nullptr;
    }
    throw ErroBanco("Nao foi possivel abrir o banco SQLite: " + mensagem);
}

// roda o sql, se falhar pega a mensagem de erro do sqlite e joga ErroBanco
void Database::executar(const std::string& sql) {
    char* mensagemErro = nullptr;
    const int resultado = sqlite3_exec(conexao_, sql.c_str(), nullptr, nullptr,
                                      &mensagemErro);
    if (resultado == SQLITE_OK) return;
    const std::string mensagem = mensagemErro != nullptr
        ? mensagemErro : sqlite3_errmsg(conexao_);
    sqlite3_free(mensagemErro);
    throw ErroBanco("Falha ao executar SQL: " + mensagem);
}

// pergunta pro sqlite se existe alguma tabela (sem contar as tabelas internas dele)
bool Database::possuiTabelas() const {
    constexpr const char* consulta =
        "SELECT 1 FROM sqlite_master WHERE type = 'table' "
        "AND name NOT LIKE 'sqlite_%' LIMIT 1;";
    sqlite3_stmt* comando = nullptr;
    if (sqlite3_prepare_v2(conexao_, consulta, -1, &comando, nullptr) != SQLITE_OK) {
        throw ErroBanco("Falha ao verificar o banco: " +
                       std::string(sqlite3_errmsg(conexao_)));
    }
    const int resultado = sqlite3_step(comando);
    sqlite3_finalize(comando);
    if (resultado == SQLITE_ROW) return true;
    if (resultado == SQLITE_DONE) return false;
    throw ErroBanco("Falha ao verificar o banco: " +
                   std::string(sqlite3_errmsg(conexao_)));
}

// so aplica o schema.sql se o banco ainda estiver vazio
void Database::inicializarSchema(const std::string& caminhoSchema) {
    if (!possuiTabelas()) executar(lerArquivo(caminhoSchema));
}
