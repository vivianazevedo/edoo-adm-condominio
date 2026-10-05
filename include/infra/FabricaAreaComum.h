#pragma once

#include "infra/FabricaPorTipo.h"

class AreaComum;

// cria SalaoFestas, Piscina ou Churrasqueira conforme o tipo salvo na tabela area_comum
class FabricaAreaComum final : public FabricaPorTipo<AreaComum> {
public:
    // liga o texto SalaoFestas a funcao que cria um SalaoFestas
    void registrarSalaoFestas(Criador criador);

    // liga o texto Piscina a funcao que cria uma Piscina
    void registrarPiscina(Criador criador);

    // liga o texto Churrasqueira a funcao que cria uma Churrasqueira
    void registrarChurrasqueira(Criador criador);
};
