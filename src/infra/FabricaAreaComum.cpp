#include "infra/FabricaAreaComum.h"

#include <utility>

void FabricaAreaComum::registrarSalaoFestas(Criador criador) {
    registrar("SalaoFestas", std::move(criador));
}

void FabricaAreaComum::registrarPiscina(Criador criador) {
    registrar("Piscina", std::move(criador));
}

void FabricaAreaComum::registrarChurrasqueira(Criador criador) {
    registrar("Churrasqueira", std::move(criador));
}
