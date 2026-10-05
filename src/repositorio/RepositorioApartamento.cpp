#include "repositorio/RepositorioApartamento.h"
#include <sqlite3.h>
#include <string>
#include "infra/Database.h"
#include "infra/ErroCondominio.h"

using namespace std;  // permitido em .cpp (so e proibido nos .h)

namespace {

// comando preparado que se fecha sozinho quando sai do escopo
using Comando = unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)>;

// pega a conexao unica do banco
sqlite3* conexao() {
    return Database::instancia().conexao();
}

// prepara um sql com ? nos valores, se der erro joga ErroBanco
Comando preparar(const string& sql) {
    sqlite3_stmt* stmt = nullptr;
    int codigo = sqlite3_prepare_v2(conexao(), sql.c_str(), -1, &stmt, nullptr);
    if (codigo != SQLITE_OK) {
        throw ErroBanco(string("erro ao preparar sql: ") + sqlite3_errmsg(conexao()));
    }
    return Comando(stmt, sqlite3_finalize);
}

// roda um comando que nao devolve linhas (insert, update, delete)
void executarComando(Comando& stmt) {
    int codigo = sqlite3_step(stmt.get());
    if (codigo != SQLITE_DONE) {
        throw ErroBanco(string("erro ao executar sql: ") + sqlite3_errmsg(conexao()));
    }
}

// le uma coluna de texto, se vier nula devolve string vazia
string lerTexto(Comando& stmt, int coluna) {
    const unsigned char* texto = sqlite3_column_text(stmt.get(), coluna);
    if (texto == nullptr) {
        return string();
    }
    return string(reinterpret_cast<const char*>(texto));
}

// transforma a linha atual (id, bloco, numero, andar) em um Apartamento
unique_ptr<Apartamento> montar(Comando& stmt) {
    int id = sqlite3_column_int(stmt.get(), 0);
    string bloco = lerTexto(stmt, 1);
    string numero = lerTexto(stmt, 2);
    int andar = sqlite3_column_int(stmt.get(), 3);
    return make_unique<Apartamento>(bloco, numero, andar, id);
}

}  // namespace

int RepositorioApartamento::inserir(const Apartamento& apartamento) {
    Comando stmt = preparar(
        "INSERT INTO apartamento (bloco, numero, andar) VALUES (?, ?, ?)");
    sqlite3_bind_text(stmt.get(), 1, apartamento.getBloco().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt.get(), 2, apartamento.getNumero().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt.get(), 3, apartamento.getAndar());
    executarComando(stmt);

    // devolve o id que o banco gerou
    return static_cast<int>(sqlite3_last_insert_rowid(conexao()));
}

unique_ptr<Apartamento> RepositorioApartamento::buscarPorId(int id) {
    Comando stmt = preparar(
        "SELECT id, bloco, numero, andar FROM apartamento WHERE id = ?");
    sqlite3_bind_int(stmt.get(), 1, id);

    int codigo = sqlite3_step(stmt.get());
    if (codigo == SQLITE_ROW) {
        return montar(stmt);
    }
    if (codigo == SQLITE_DONE) {
        return nullptr;  // nao achou
    }
    throw ErroBanco(string("erro ao buscar apartamento: ") + sqlite3_errmsg(conexao()));
}

vector<unique_ptr<Apartamento>> RepositorioApartamento::listar() {
    Comando stmt = preparar(
        "SELECT id, bloco, numero, andar FROM apartamento ORDER BY bloco, numero");

    vector<unique_ptr<Apartamento>> lista;
    int codigo = sqlite3_step(stmt.get());
    while (codigo == SQLITE_ROW) {
        lista.push_back(montar(stmt));
        codigo = sqlite3_step(stmt.get());
    }
    if (codigo != SQLITE_DONE) {
        throw ErroBanco(string("erro ao listar apartamentos: ") + sqlite3_errmsg(conexao()));
    }
    return lista;
}

bool RepositorioApartamento::atualizar(const Apartamento& apartamento) {
    Comando stmt = preparar(
        "UPDATE apartamento SET bloco = ?, numero = ?, andar = ? WHERE id = ?");
    sqlite3_bind_text(stmt.get(), 1, apartamento.getBloco().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt.get(), 2, apartamento.getNumero().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt.get(), 3, apartamento.getAndar());
    sqlite3_bind_int(stmt.get(), 4, apartamento.getId());
    executarComando(stmt);

    // se nenhuma linha mudou, o id nao existia
    return sqlite3_changes(conexao()) > 0;
}

bool RepositorioApartamento::remover(int id) {
    Comando stmt = preparar("DELETE FROM apartamento WHERE id = ?");
    sqlite3_bind_int(stmt.get(), 1, id);
    executarComando(stmt);

    return sqlite3_changes(conexao()) > 0;
}