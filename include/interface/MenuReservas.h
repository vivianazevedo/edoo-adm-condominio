#pragma once

#include "servico/AreaComumService.h"
#include "servico/ReservaService.h"

class MenuReservas {
private:
    AreaComumService areaService_;
    ReservaService reservaService_;

    void listarAreas();
    void cadastrarArea();
    void criarReserva();
    void listarMinhasReservas();
    void cancelarReserva();

public:
    void exibirMenu();
};