#include "infra/FabricaPessoa.h"

#include <utility>

// cada metodo so chama o registrar da classe base com o mesmo texto que fica salvo no banco
void FabricaPessoa::registrarMorador(Criador criador) {
    registrar("morador", std::move(criador));
}

void FabricaPessoa::registrarVisitante(Criador criador) {
    registrar("visitante", std::move(criador));
}

void FabricaPessoa::registrarFuncionario(Criador criador) {
    registrar("funcionario", std::move(criador));
}
