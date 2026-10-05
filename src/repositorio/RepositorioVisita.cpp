#include "repositorio/RepositorioVisita.h"
#include <sqlite3.h>
#include <string>
#include "infra/Database.h"
#include "infra/ErroCondominio.h"

using namespace std;  // permitido em .cpp (so e proibido nos .h)

namespace {

// comando preparado que se fecha sozinho quando sai do escopo
using Comando = unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)>;

// colunas que o select devolve, sempre nessa ordem (o montar depende dela)
const char* const SELECT_VISITA =
    "SELECT id, visitante_id, apartamento_id, registrado_por, entrada, saida FROM visita";

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

// liga os campos da visita (sem o id) nos ? de 1 a 5
// saida vazia vira NULL no banco, que e o jeito de marcar visita aberta
void ligarCampos(Comando& stmt, const Visita& visita) {
    sqlite3_bind_int(stmt.get(), 1, visita.getVisitanteId());
    sqlite3_bind_int(stmt.get(), 2, visita.getApartamentoId());
    sqlite3_bind_int(stmt.get(), 3, visita.getRegistradoPor());
    sqlite3_bind_text(stmt.get(), 4, visita.getEntrada().c_str(), -1, SQLITE_TRANSIENT);
    if (visita.getSaida().empty()) {
        sqlite3_bind_null(stmt.get(), 5);
    } else {
        sqlite3_bind_text(stmt.get(), 5, visita.getSaida().c_str(), -1, SQLITE_TRANSIENT);
    }
}

// transforma a linha atual (id, visitante, apartamento, porteiro, entrada, saida) em uma Visita
unique_ptr<Visita> montar(Comando& stmt) {
    int id = sqlite3_column_int(stmt.get(), 0);
    int visitanteId = sqlite3_column_int(stmt.get(), 1);
    int apartamentoId = sqlite3_column_int(stmt.get(), 2);
    int registradoPor = sqlite3_column_int(stmt.get(), 3);
    string entrada = lerTexto(stmt, 4);
    string saida = lerTexto(stmt, 5);
    return make_unique<Visita>(visitanteId, apartamentoId, registradoPor, entrada, saida, id);
}

}  // namespace

// create: insere a visita e devolve o id
int RepositorioVisita::inserir(const Visita& visita) {
    Comando stmt = preparar(
        "INSERT INTO visita (visitante_id, apartamento_id, registrado_por, entrada, saida) "
        "VALUES (?, ?, ?, ?, ?)");
    ligarCampos(stmt, visita);
    executarComando(stmt);

    // devolve o id que o banco gerou
    return static_cast<int>(sqlite3_last_insert_rowid(conexao()));
}

// read: busca uma visita pelo id (nullptr se nao existir)
unique_ptr<Visita> RepositorioVisita::buscarPorId(int id) {
    Comando stmt = preparar(string(SELECT_VISITA) + " WHERE id = ?");
    sqlite3_bind_int(stmt.get(), 1, id);

    int codigo = sqlite3_step(stmt.get());
    if (codigo == SQLITE_ROW) {
        return montar(stmt);
    }
    if (codigo == SQLITE_DONE) {
        return nullptr;  // nao achou
    }
    throw ErroBanco(string("erro ao buscar visita: ") + sqlite3_errmsg(conexao()));
}

// read: lista todas as visitas
vector<unique_ptr<Visita>> RepositorioVisita::listar() {
    // mais antigas primeiro, o id desempata entradas no mesmo minuto
    Comando stmt = preparar(string(SELECT_VISITA) + " ORDER BY entrada, id");

    vector<unique_ptr<Visita>> lista;
    int codigo = sqlite3_step(stmt.get());
    while (codigo == SQLITE_ROW) {
        lista.push_back(montar(stmt));
        codigo = sqlite3_step(stmt.get());
    }
    if (codigo != SQLITE_DONE) {
        throw ErroBanco(string("erro ao listar visitas: ") + sqlite3_errmsg(conexao()));
    }
    return lista;
}

// update: muda os dados da visita (e assim que a saida e registrada)
bool RepositorioVisita::atualizar(const Visita& visita) {
    Comando stmt = preparar(
        "UPDATE visita SET visitante_id = ?, apartamento_id = ?, registrado_por = ?, "
        "entrada = ?, saida = ? WHERE id = ?");
    ligarCampos(stmt, visita);
    sqlite3_bind_int(stmt.get(), 6, visita.getId());
    executarComando(stmt);

    // se nenhuma linha mudou, o id nao existia
    return sqlite3_changes(conexao()) > 0;
}

// delete: apaga a visita pelo id
bool RepositorioVisita::remover(int id) {
    Comando stmt = preparar("DELETE FROM visita WHERE id = ?");
    sqlite3_bind_int(stmt.get(), 1, id);
    executarComando(stmt);

    return sqlite3_changes(conexao()) > 0;
}