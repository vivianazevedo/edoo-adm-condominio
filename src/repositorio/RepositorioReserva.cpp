#include "repositorio/RepositorioReserva.h"

#include "infra/ErroCondominio.h"
#include "SqliteComando.h"

#include <memory>

namespace {

const char* statusTexto(StatusReserva status) {
    return status == StatusReserva::ATIVA ? "ativa" : "cancelada";
}

StatusReserva lerStatus(const std::string& texto) {
    if (texto == "ativa") return StatusReserva::ATIVA;
    if (texto == "cancelada") return StatusReserva::CANCELADA;
    throw ErroBanco("Status de reserva desconhecido: " + texto);
}

std::unique_ptr<Reserva> montar(const SqliteComando& comando) {
    return std::make_unique<Reserva>(
        comando.inteiroEm(0), comando.inteiroEm(1), comando.inteiroEm(2),
        comando.textoEm(3), comando.textoEm(4), comando.textoEm(5),
        comando.inteiroEm(6), lerStatus(comando.textoEm(7)), comando.realEm(8));
}

std::vector<std::unique_ptr<Reserva>> coletar(SqliteComando& comando) {
    std::vector<std::unique_ptr<Reserva>> resultado;
    while (comando.proxima()) resultado.push_back(montar(comando));
    return resultado;
}

}  // namespace

int RepositorioReserva::inserir(const Reserva& entidade) {
    SqliteComando comando(
        "INSERT INTO reserva (morador_id, area_id, data, hora_inicio, hora_fim, "
        "num_convidados, status, valor) VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
    comando.inteiro(1, entidade.getMoradorId());
    comando.inteiro(2, entidade.getAreaId());
    comando.texto(3, entidade.getData());
    comando.texto(4, entidade.getHoraInicio());
    comando.texto(5, entidade.getHoraFim());
    comando.inteiro(6, entidade.getNumConvidados());
    comando.texto(7, statusTexto(entidade.getStatus()));
    comando.real(8, entidade.getValor());
    comando.executar();
    return comando.ultimoId();
}

std::unique_ptr<Reserva> RepositorioReserva::buscarPorId(int id) {
    SqliteComando comando(
        "SELECT id, morador_id, area_id, data, hora_inicio, hora_fim, "
        "num_convidados, status, valor FROM reserva WHERE id = ?");
    comando.inteiro(1, id);
    return comando.proxima() ? montar(comando) : nullptr;
}

std::vector<std::unique_ptr<Reserva>> RepositorioReserva::listar() {
    SqliteComando comando(
        "SELECT id, morador_id, area_id, data, hora_inicio, hora_fim, "
        "num_convidados, status, valor FROM reserva ORDER BY id");
    return coletar(comando);
}

bool RepositorioReserva::atualizar(const Reserva& entidade) {
    SqliteComando comando(
        "UPDATE reserva SET morador_id = ?, area_id = ?, data = ?, "
        "hora_inicio = ?, hora_fim = ?, num_convidados = ?, status = ?, "
        "valor = ? WHERE id = ?");
    comando.inteiro(1, entidade.getMoradorId());
    comando.inteiro(2, entidade.getAreaId());
    comando.texto(3, entidade.getData());
    comando.texto(4, entidade.getHoraInicio());
    comando.texto(5, entidade.getHoraFim());
    comando.inteiro(6, entidade.getNumConvidados());
    comando.texto(7, statusTexto(entidade.getStatus()));
    comando.real(8, entidade.getValor());
    comando.inteiro(9, entidade.getId());
    comando.executar();
    return comando.alteradas() > 0;
}

bool RepositorioReserva::remover(int id) {
    SqliteComando comando("DELETE FROM reserva WHERE id = ?");
    comando.inteiro(1, id);
    comando.executar();
    return comando.alteradas() > 0;
}

std::vector<std::unique_ptr<Reserva>> RepositorioReserva::buscarPorMorador(int moradorId) {
    SqliteComando comando(
        "SELECT id, morador_id, area_id, data, hora_inicio, hora_fim, "
        "num_convidados, status, valor FROM reserva WHERE morador_id = ? ORDER BY data, hora_inicio");
    comando.inteiro(1, moradorId);
    return coletar(comando);
}

std::vector<std::unique_ptr<Reserva>> RepositorioReserva::buscarPorAreaEData(
    int areaId, const std::string& data) {
    SqliteComando comando(
        "SELECT id, morador_id, area_id, data, hora_inicio, hora_fim, "
        "num_convidados, status, valor FROM reserva WHERE area_id = ? AND data = ? "
        "ORDER BY hora_inicio");
    comando.inteiro(1, areaId);
    comando.texto(2, data);
    return coletar(comando);
}
