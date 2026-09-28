#pragma once

#include <string>

enum class StatusReserva {
    ATIVA,
    CANCELADA,
    CONCLUIDA
};

class Reserva {
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

public:
    Reserva(int id, int moradorId, int areaId, const std::string& data,
            const std::string& horaInicio, const std::string& horaFim,
            int numConvidados, StatusReserva status, double valor)
        : id_(id), moradorId_(moradorId), areaId_(areaId), data_(data),
          horaInicio_(horaInicio), horaFim_(horaFim), numConvidados_(numConvidados),
          status_(status), valor_(valor) {}

    int getId() const { return id_; }
    int getMoradorId() const { return moradorId_; }
    int getAreaId() const { return areaId_; }
    std::string getData() const { return data_; }
    std::string getHoraInicio() const { return horaInicio_; }
    std::string getHoraFim() const { return horaFim_; }
    int getNumConvidados() const { return numConvidados_; }
    StatusReserva getStatus() const { return status_; }
    double getValor() const { return valor_; }

    void setStatus(StatusReserva status) { status_ = status; }
    void setValor(double valor) { valor_ = valor; }
};