#include "infra/ErroCondominio.h"

#include <utility>

using namespace std; 

ErroCondominio:: ErroCondominio (string mensagem) : mensagem_(move(mensagem)){}; 

const char*  ErroCondominio :: what() const noexcept {
    return mensagem_.c_str();
}

ErroValidacao :: ErroValidacao(const string& mensagem) : ErroCondominio(mensagem) {}

ErroRegraNegocio :: ErroRegraNegocio (const string& mensagem) : ErroCondominio(mensagem) {}

ErroBanco :: ErroBanco (const string& mensagem) : ErroCondominio(mensagem) {}
