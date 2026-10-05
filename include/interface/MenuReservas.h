#pragma once

#include "servico/AreaComumService.h"
#include "servico/ReservaService.h"

// menu de terminal de areas comuns e reservas
// so le o que a pessoa digita e chama os services
class MenuReservas {
private:
    // os services ficam guardados dentro do menu
    AreaComumService areaService_;
    ReservaService reservaService_;

    // uma funcao pra cada opcao do menu
    void listarAreas();
    void cadastrarArea();
    void criarReserva();
    void listarMinhasReservas();
    void cancelarReserva();

public:
    // mostra o menu ate a pessoa escolher 0 (voltar)
    void exibirMenu();
};