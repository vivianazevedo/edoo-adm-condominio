#pragma once
#include "servico/VisitaService.h"

// menu de terminal de visitantes e visitas (portaria)
// nao guarda dados: so le o que a pessoa digita e chama o VisitaService
class MenuVisitas {
private:
    VisitaService& visitaService_;

    // uma funcao pra cada opcao do menu
    void cadastrarVisitante();
    void registrarEntrada();
    void registrarSaida();
    void listarAbertas();
    void historicoPorApartamento();

public:
    // recebe o service por referencia (o menu nao cria nada, so usa)
    explicit MenuVisitas(VisitaService& visitaService);

    // mostra o menu ate a pessoa escolher voltar
    void exibirMenu();
};
