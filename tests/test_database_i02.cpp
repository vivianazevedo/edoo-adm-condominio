#include "infra/Database.h"
#include "infra/ErroCondominio.h"

#include <sqlite3.h>

#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <string>

namespace {

int valorInteiro(sqlite3* conexao, const char* sql) {
    sqlite3_stmt* comando = nullptr;
    if (sqlite3_prepare_v2(conexao, sql, -1, &comando, nullptr) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(conexao));
    }

    if (sqlite3_step(comando) != SQLITE_ROW) {
        sqlite3_finalize(comando);
        throw std::runtime_error("A consulta de teste nao retornou uma linha");
    }

    const int valor = sqlite3_column_int(comando, 0);
    sqlite3_finalize(comando);
    return valor;
}

}  // namespace

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Uso: teste <banco.db> <schema.sql>\n";
        return 2;
    }

    std::remove(argv[1]);

    try {
        Database& primeira = Database::instancia(argv[1], argv[2]);
        Database& segunda = Database::instancia();

        if (&primeira != &segunda) {
            throw std::runtime_error("O Singleton devolveu instancias diferentes");
        }
        if (valorInteiro(primeira.conexao(), "PRAGMA foreign_keys;") != 1) {
            throw std::runtime_error("As chaves estrangeiras nao foram ativadas");
        }
        if (valorInteiro(
                primeira.conexao(),
                "SELECT COUNT(*) FROM sqlite_master WHERE type='table' "
                "AND name NOT LIKE 'sqlite_%';") != 7) {
            throw std::runtime_error("O schema nao criou as sete tabelas do MVP");
        }

        bool erroConvertido = false;
        try {
            primeira.executar("SQL INVALIDO;");
        } catch (const ErroBanco&) {
            erroConvertido = true;
        }

        if (!erroConvertido) {
            throw std::runtime_error("Uma falha SQL nao gerou ErroBanco");
        }

        std::cout << "I02: todos os testes passaram\n";
        return 0;
    } catch (const std::exception& erro) {
        std::cerr << "I02: teste falhou: " << erro.what() << '\n';
        return 1;
    }
}
