#pragma once

#include "infra/Database.h"
#include "infra/ErroCondominio.h"

#include <sqlite3.h>

#include <memory>
#include <string>

// embrulha um comando sql preparado do sqlite (raii: fecha sozinho quando o objeto some)
// nos binds a posicao do ? comeca em 1, na leitura das colunas (xxxEm) comeca em 0
class SqliteComando {
public:
    // prepara o sql, se der erro joga ErroBanco
    explicit SqliteComando(const char* sql)
        : banco_(Database::instancia().conexao()), stmt_(nullptr, sqlite3_finalize) {
        sqlite3_stmt* preparado = nullptr;
        if (sqlite3_prepare_v2(banco_, sql, -1, &preparado, nullptr) != SQLITE_OK) {
            throw ErroBanco(std::string("Erro ao preparar SQL: ") + sqlite3_errmsg(banco_));
        }
        stmt_.reset(preparado);
    }

    // liga um valor no ? da posicao indicada (inteiro, real e texto)
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
    // vai pra proxima linha do resultado: true se tem linha, false se acabou
    bool proxima() {
        const int resultado = sqlite3_step(stmt_.get());
        if (resultado == SQLITE_ROW) return true;
        if (resultado == SQLITE_DONE) return false;
        throw ErroBanco(std::string("Erro ao consultar SQLite: ") + sqlite3_errmsg(banco_));
    }
    // roda um insert, update ou delete
    void executar() {
        if (sqlite3_step(stmt_.get()) != SQLITE_DONE) {
            throw ErroBanco(std::string("Erro ao gravar SQLite: ") + sqlite3_errmsg(banco_));
        }
    }
    // le a coluna da linha atual (inteiro, real e texto)
    int inteiroEm(int coluna) const { return sqlite3_column_int(stmt_.get(), coluna); }
    double realEm(int coluna) const { return sqlite3_column_double(stmt_.get(), coluna); }
    std::string textoEm(int coluna) const {
        const auto* valor = sqlite3_column_text(stmt_.get(), coluna);
        return valor != nullptr ? reinterpret_cast<const char*>(valor) : "";
    }
    // quantas linhas o ultimo comando mudou (0 quer dizer que o id nao existia)
    int alteradas() const { return sqlite3_changes(banco_); }
    // id gerado pelo ultimo insert
    int ultimoId() const { return static_cast<int>(sqlite3_last_insert_rowid(banco_)); }

private:
    // joga ErroBanco se o bind nao deu certo
    void conferir(int resultado) const {
        if (resultado != SQLITE_OK) {
            throw ErroBanco(std::string("Erro ao vincular parametro: ") + sqlite3_errmsg(banco_));
        }
    }
    sqlite3* banco_;
    // unique_ptr com sqlite3_finalize como deleter: o comando e fechado sozinho
    std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> stmt_;
};
