#pragma once

#include <string>

enum class StatusReserva { ATIVA, CANCELADA };

class Reserva {
public:
    Reserva(int id, int moradorId, int areaId, std::string data,
            std::string horaInicio, std::string horaFim, int numConvidados,
            StatusReserva status, double valor);

    int getId() const noexcept { return id_; }
    int getMoradorId() const noexcept { return moradorId_; }
    int getAreaId() const noexcept { return areaId_; }
    const std::string& getData() const noexcept { return data_; }
    const std::string& getHoraInicio() const noexcept { return horaInicio_; }
    const std::string& getHoraFim() const noexcept { return horaFim_; }
    int getNumConvidados() const noexcept { return numConvidados_; }
    StatusReserva getStatus() const noexcept { return status_; }
    double getValor() const noexcept { return valor_; }

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
