#pragma once

#include <string>

// situacao da reserva: ATIVA ocupa o horario, CANCELADA libera ele
enum class StatusReserva { ATIVA, CANCELADA };

// reserva de uma area comum feita por um morador
// guarda so os ids, quem junta tudo e o ReservaService
class Reserva {
public:
    // valida os dados na criacao (data, horario, ids e valor) e joga ErroValidacao se estiver errado
    Reserva(int id, int moradorId, int areaId, std::string data,
            std::string horaInicio, std::string horaFim, int numConvidados,
            StatusReserva status, double valor);

    // getters
    int getId() const noexcept { return id_; }
    int getMoradorId() const noexcept { return moradorId_; }
    int getAreaId() const noexcept { return areaId_; }
    const std::string& getData() const noexcept { return data_; }
    const std::string& getHoraInicio() const noexcept { return horaInicio_; }
    const std::string& getHoraFim() const noexcept { return horaFim_; }
    int getNumConvidados() const noexcept { return numConvidados_; }
    StatusReserva getStatus() const noexcept { return status_; }
    double getValor() const noexcept { return valor_; }

    // unico setter: serve pra cancelar a reserva
    void setStatus(StatusReserva status) noexcept { status_ = status; }

private:
    int id_;
    int moradorId_;
    int areaId_;
    std::string data_;
    std::string horaInicio_;
    std::string horaFim_;
    int numConvidados_;
    StatusReserva status_;
    double valor_;
};
