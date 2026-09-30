#include "infra/FabricaPessoa.h"

#include <utility>

void FabricaPessoa::registrarMorador(Criador criador) {
    registrar("morador", std::move(criador));
}

void FabricaPessoa::registrarVisitante(Criador criador) {
    registrar("visitante", std::move(criador));
}

void FabricaPessoa::registrarFuncionario(Criador criador) {
    registrar("funcionario", std::move(criador));
}
