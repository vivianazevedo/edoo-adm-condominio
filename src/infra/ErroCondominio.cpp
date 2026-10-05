#include "infra/ErroCondominio.h"

#include <utility>

using namespace std; 

// guarda a mensagem com move pra nao copiar o texto
ErroCondominio:: ErroCondominio (string mensagem) : mensagem_(move(mensagem)){}; 

// devolve a mensagem em texto de c, e isso que aparece quando a excecao e capturada
const char*  ErroCondominio :: what() const noexcept {
    return mensagem_.c_str();
}

// as tres subclasses so repassam a mensagem pra classe base
ErroValidacao :: ErroValidacao(const string& mensagem) : ErroCondominio(mensagem) {}

ErroRegraNegocio :: ErroRegraNegocio (const string& mensagem) : ErroCondominio(mensagem) {}

ErroBanco :: ErroBanco (const string& mensagem) : ErroCondominio(mensagem) {}
