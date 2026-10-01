#pragma once

#include "infra/Database.h"
#include "infra/ErroCondominio.h"

#include <sqlite3.h>

#include <memory>
#include <string>

class SqliteComando {
public:
    explicit SqliteComando(const char* sql)
        : banco_(Database::instancia().conexao()), stmt_(nullptr, sqlite3_finalize) {
        sqlite3_stmt* preparado = nullptr;
        if (sqlite3_prepare_v2(banco_, sql, -1, &preparado, nullptr) != SQLITE_OK) {
            throw ErroBanco(std::string("Erro ao preparar SQL: ") + sqlite3_errmsg(banco_));
        }
        stmt_.reset(preparado);
    }

    void inteiro(int coluna, int valor) {
        conferir(sqlite3_bind_int(stmt_.get(), coluna, valor));
    }
    void real(int coluna, double valor) {
        conferir(sqlite3_bind_double(stmt_.get(), coluna, valor));
    }
    void texto(int coluna, const std::string& valor) {
        conferir(sqlite3_bind_text(stmt_.get(), coluna, valor.c_str(), -1,
                                  SQLITE_TRANSIENT));
    }
    bool proxima() {
        const int resultado = sqlite3_step(stmt_.get());
        if (resultado == SQLITE_ROW) return true;
        if (resultado == SQLITE_DONE) return false;
        throw ErroBanco(std::string("Erro ao consultar SQLite: ") + sqlite3_errmsg(banco_));
    }
    void executar() {
        if (sqlite3_step(stmt_.get()) != SQLITE_DONE) {
            throw ErroBanco(std::string("Erro ao gravar SQLite: ") + sqlite3_errmsg(banco_));
        }
    }
    int inteiroEm(int coluna) const { return sqlite3_column_int(stmt_.get(), coluna); }
    double realEm(int coluna) const { return sqlite3_column_double(stmt_.get(), coluna); }
    std::string textoEm(int coluna) const {
        const auto* valor = sqlite3_column_text(stmt_.get(), coluna);
        return valor != nullptr ? reinterpret_cast<const char*>(valor) : "";
    }
    int alteradas() const { return sqlite3_changes(banco_); }
    int ultimoId() const { return static_cast<int>(sqlite3_last_insert_rowid(banco_)); }

private:
    void conferir(int resultado) const {
        if (resultado != SQLITE_OK) {
            throw ErroBanco(std::string("Erro ao vincular parametro: ") + sqlite3_errmsg(banco_));
        }
    }
    sqlite3* banco_;
    std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> stmt_;
};
