#include "repositorio/RepositorioPessoa.h"

#include <sqlite3.h>

#include <string>

#include "infra/Database.h"
#include "infra/ErroCondominio.h"
#include "infra/FabricaPessoa.h"
#include "modelo/Funcionario.h"
#include "modelo/Morador.h"
#include "modelo/Visitante.h"

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

// liga um texto no ? da posicao indicada (comeca em 1)
void ligarTexto(Comando& stmt, int posicao, const string& texto) {
    sqlite3_bind_text(stmt.get(), posicao, texto.c_str(), -1, SQLITE_TRANSIENT);
}

// le uma coluna de texto, se vier nula devolve string vazia
string lerTexto(Comando& stmt, int coluna) {
    const unsigned char* texto = sqlite3_column_text(stmt.get(), coluna);
    if (texto == nullptr) {
        return string();
    }
    return string(reinterpret_cast<const char*>(texto));
}

// abre uma transacao e desfaz tudo sozinha se nao chamar confirmar()
// assim pessoa e morador/funcionario entram juntos ou nao entram nenhum
class Transacao {
public:
    Transacao() { Database::instancia().executar("BEGIN"); }

    ~Transacao() {
        if (!confirmada_) {
            sqlite3_exec(conexao(), "ROLLBACK", nullptr, nullptr, nullptr);
        }
    }

    void confirmar() {
        Database::instancia().executar("COMMIT");
        confirmada_ = true;
    }

    Transacao(const Transacao&) = delete;
    Transacao& operator=(const Transacao&) = delete;

private:
    bool confirmada_ = false;
};

// os enums viram texto no banco e voltam
string ocupacaoParaTexto(TipoOcupacao ocupacao) {
    if (ocupacao == TipoOcupacao::Proprietario) return "Proprietario";
    if (ocupacao == TipoOcupacao::Inquilino) return "Inquilino";
    return "Dependente";
}

TipoOcupacao textoParaOcupacao(const string& texto) {
    if (texto == "Proprietario") return TipoOcupacao::Proprietario;
    if (texto == "Inquilino") return TipoOcupacao::Inquilino;
    if (texto == "Dependente") return TipoOcupacao::Dependente;
    throw ErroBanco("tipo de ocupacao invalido no banco: " + texto);
}

string cargoParaTexto(Cargo cargo) {
    if (cargo == Cargo::Porteiro) return "Porteiro";
    if (cargo == Cargo::Zelador) return "Zelador";
    if (cargo == Cargo::Faxineiro) return "Faxineiro";
    return "Administrador";
}

Cargo textoParaCargo(const string& texto) {
    if (texto == "Porteiro") return Cargo::Porteiro;
    if (texto == "Zelador") return Cargo::Zelador;
    if (texto == "Faxineiro") return Cargo::Faxineiro;
    if (texto == "Administrador") return Cargo::Administrador;
    throw ErroBanco("cargo invalido no banco: " + texto);
}

// select que junta pessoa com as tabelas extras
// colunas: 0 id, 1 nome, 2 cpf, 3 telefone, 4 tipo,
//          5 apartamento_id, 6 tipo_ocupacao, 7 data_entrada (morador)
//          8 cargo, 9 turno, 10 data_admissao (funcionario)
const char* const SELECT_PESSOA =
    "SELECT p.id, p.nome, p.cpf, p.telefone, p.tipo, "
    "m.apartamento_id, m.tipo_ocupacao, m.data_entrada, "
    "f.cargo, f.turno, f.data_admissao "
    "FROM pessoa p "
    "LEFT JOIN morador m ON m.pessoa_id = p.id "
    "LEFT JOIN funcionario f ON f.pessoa_id = p.id ";

// transforma a linha atual na subclasse certa usando a fabrica
unique_ptr<Pessoa> montar(Comando& stmt) {
    int id = sqlite3_column_int(stmt.get(), 0);
    string nome = lerTexto(stmt, 1);
    string cpf = lerTexto(stmt, 2);
    string telefone = lerTexto(stmt, 3);
    string tipo = lerTexto(stmt, 4);
    int apartamentoId = sqlite3_column_int(stmt.get(), 5);
    string ocupacao = lerTexto(stmt, 6);
    string dataEntrada = lerTexto(stmt, 7);
    string cargo = lerTexto(stmt, 8);
    string turno = lerTexto(stmt, 9);
    string dataAdmissao = lerTexto(stmt, 10);

    // uma fabrica nova por linha (ela nao deixa registrar o mesmo tipo duas vezes)
    // so o criador do tipo da linha e chamado, os outros ficam parados
    FabricaPessoa fabrica;
    fabrica.registrarMorador([&]() -> unique_ptr<Pessoa> {
        return make_unique<Morador>(nome, cpf, telefone, apartamentoId,
                                    textoParaOcupacao(ocupacao), dataEntrada, id);
    });
    fabrica.registrarFuncionario([&]() -> unique_ptr<Pessoa> {
        return make_unique<Funcionario>(nome, cpf, telefone, textoParaCargo(cargo),
                                        turno, dataAdmissao, id);
    });
    fabrica.registrarVisitante([&]() -> unique_ptr<Pessoa> {
        return make_unique<Visitante>(nome, cpf, telefone, id);
    });
    return fabrica.criar(tipo);
}

}  // namespace

int RepositorioPessoa::inserir(const Pessoa& pessoa) {
    Transacao transacao;

    // primeiro os dados comuns
    Comando stmtPessoa = preparar(
        "INSERT INTO pessoa (nome, cpf, telefone, tipo) VALUES (?, ?, ?, ?)");
    ligarTexto(stmtPessoa, 1, pessoa.nome());
    ligarTexto(stmtPessoa, 2, pessoa.cpf());
    ligarTexto(stmtPessoa, 3, pessoa.telefone());
    ligarTexto(stmtPessoa, 4, pessoa.tipo());
    executarComando(stmtPessoa);
    int id = static_cast<int>(sqlite3_last_insert_rowid(conexao()));

    // depois a tabela extra do tipo (visitante nao tem)
    if (const Morador* morador = dynamic_cast<const Morador*>(&pessoa)) {
        Comando stmt = preparar(
            "INSERT INTO morador (pessoa_id, apartamento_id, tipo_ocupacao, data_entrada) "
            "VALUES (?, ?, ?, ?)");
        sqlite3_bind_int(stmt.get(), 1, id);
        sqlite3_bind_int(stmt.get(), 2, morador->apartamentoId());
        ligarTexto(stmt, 3, ocupacaoParaTexto(morador->tipoOcupacao()));
        ligarTexto(stmt, 4, morador->dataEntrada());
        executarComando(stmt);
    } else if (const Funcionario* funcionario = dynamic_cast<const Funcionario*>(&pessoa)) {
        Comando stmt = preparar(
            "INSERT INTO funcionario (pessoa_id, cargo, turno, data_admissao) "
            "VALUES (?, ?, ?, ?)");
        sqlite3_bind_int(stmt.get(), 1, id);
        ligarTexto(stmt, 2, cargoParaTexto(funcionario->cargo()));
        ligarTexto(stmt, 3, funcionario->turno());
        ligarTexto(stmt, 4, funcionario->dataAdmissao());
        executarComando(stmt);
    } else if (dynamic_cast<const Visitante*>(&pessoa) == nullptr) {
        throw ErroBanco("tipo de pessoa nao suportado: " + pessoa.tipo());
    }

    transacao.confirmar();
    return id;
}

unique_ptr<Pessoa> RepositorioPessoa::buscarPorId(int id) {
    Comando stmt = preparar(string(SELECT_PESSOA) + "WHERE p.id = ?");
    sqlite3_bind_int(stmt.get(), 1, id);

    int codigo = sqlite3_step(stmt.get());
    if (codigo == SQLITE_ROW) {
        return montar(stmt);
    }
    if (codigo == SQLITE_DONE) {
        return nullptr;  // nao achou
    }
    throw ErroBanco(string("erro ao buscar pessoa: ") + sqlite3_errmsg(conexao()));
}

vector<unique_ptr<Pessoa>> RepositorioPessoa::listar() {
    Comando stmt = preparar(string(SELECT_PESSOA) + "ORDER BY p.nome");

    vector<unique_ptr<Pessoa>> lista;
    int codigo = sqlite3_step(stmt.get());
    while (codigo == SQLITE_ROW) {
        lista.push_back(montar(stmt));
        codigo = sqlite3_step(stmt.get());
    }
    if (codigo != SQLITE_DONE) {
        throw ErroBanco(string("erro ao listar pessoas: ") + sqlite3_errmsg(conexao()));
    }
    return lista;
}

bool RepositorioPessoa::atualizar(const Pessoa& pessoa) {
    Transacao transacao;

    // o tipo da pessoa nao muda, so os dados
    Comando stmtPessoa = preparar(
        "UPDATE pessoa SET nome = ?, cpf = ?, telefone = ? WHERE id = ?");
    ligarTexto(stmtPessoa, 1, pessoa.nome());
    ligarTexto(stmtPessoa, 2, pessoa.cpf());
    ligarTexto(stmtPessoa, 3, pessoa.telefone());
    sqlite3_bind_int(stmtPessoa.get(), 4, pessoa.id());
    executarComando(stmtPessoa);

    // nenhuma linha mudou: o id nao existia
    if (sqlite3_changes(conexao()) == 0) {
        return false;
    }

    if (const Morador* morador = dynamic_cast<const Morador*>(&pessoa)) {
        Comando stmt = preparar(
            "UPDATE morador SET apartamento_id = ?, tipo_ocupacao = ?, data_entrada = ? "
            "WHERE pessoa_id = ?");
        sqlite3_bind_int(stmt.get(), 1, morador->apartamentoId());
        ligarTexto(stmt, 2, ocupacaoParaTexto(morador->tipoOcupacao()));
        ligarTexto(stmt, 3, morador->dataEntrada());
        sqlite3_bind_int(stmt.get(), 4, pessoa.id());
        executarComando(stmt);
        if (sqlite3_changes(conexao()) == 0) {
            throw ErroBanco("a pessoa " + to_string(pessoa.id()) + " nao e um morador");
        }
    } else if (const Funcionario* funcionario = dynamic_cast<const Funcionario*>(&pessoa)) {
        Comando stmt = preparar(
            "UPDATE funcionario SET cargo = ?, turno = ?, data_admissao = ? "
            "WHERE pessoa_id = ?");
        ligarTexto(stmt, 1, cargoParaTexto(funcionario->cargo()));
        ligarTexto(stmt, 2, funcionario->turno());
        ligarTexto(stmt, 3, funcionario->dataAdmissao());
        sqlite3_bind_int(stmt.get(), 4, pessoa.id());
        executarComando(stmt);
        if (sqlite3_changes(conexao()) == 0) {
            throw ErroBanco("a pessoa " + to_string(pessoa.id()) + " nao e um funcionario");
        }
    }

    transacao.confirmar();
    return true;
}

bool RepositorioPessoa::remover(int id) {
    Transacao transacao;

    // apaga as tabelas extras primeiro (a fk de pessoa impede o contrario)
    Comando stmtMorador = preparar("DELETE FROM morador WHERE pessoa_id = ?");
    sqlite3_bind_int(stmtMorador.get(), 1, id);
    executarComando(stmtMorador);

    Comando stmtFuncionario = preparar("DELETE FROM funcionario WHERE pessoa_id = ?");
    sqlite3_bind_int(stmtFuncionario.get(), 1, id);
    executarComando(stmtFuncionario);

    Comando stmtPessoa = preparar("DELETE FROM pessoa WHERE id = ?");
    sqlite3_bind_int(stmtPessoa.get(), 1, id);
    executarComando(stmtPessoa);
    bool removeu = sqlite3_changes(conexao()) > 0;

    transacao.confirmar();
    return removeu;
}