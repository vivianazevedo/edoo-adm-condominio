#include "interface/MenuVisitas.h"
#include <exception>
#include <functional>
#include <iostream>
#include <string>
#include "infra/ErroCondominio.h"
#include "modelo/Visita.h"

namespace {

// le uma linha inteira (aceita espacos), se a entrada acabar devolve vazio
std::string lerLinha(const std::string& pergunta) {
    std::cout << pergunta;
    std::string linha;
    std::getline(std::cin, linha);
    return linha;
}

// le um numero inteiro, repete a pergunta se digitarem outra coisa
// se a entrada acabar (ctrl+d) devolve 0 para o menu voltar
int lerInteiro(const std::string& pergunta) {
    while (true) {
        std::string linha = lerLinha(pergunta);
        if (!std::cin) {
            return 0;
        }
        try {
            std::size_t usados = 0;
            int valor = std::stoi(linha, &usados);
            if (usados == linha.size()) {
                return valor;
            }
        } catch (const std::exception&) {
            // cai na mensagem de baixo
        }
        std::cout << "Numero invalido, tente de novo.\n";
    }
}

// roda a acao e mostra o erro (se der) sem derrubar o menu
void executar(const std::function<void()>& acao) {
    try {
        acao();
    } catch (const ErroValidacao& e) {
        std::cout << "Dados invalidos: " << e.what() << "\n";
    } catch (const ErroRegraNegocio& e) {
        std::cout << "Regra de negocio: " << e.what() << "\n";
    } catch (const ErroBanco& e) {
        std::cout << "Erro no banco: " << e.what() << "\n";
    }
}

// mostra uma visita por linha, com o id para usar na saida
void imprimirVisitas(const std::vector<std::unique_ptr<Visita>>& visitas) {
    if (visitas.empty()) {
        std::cout << "Nenhuma visita encontrada.\n";
        return;
    }
    for (const auto& visita : visitas) {
        std::cout << "ID " << visita->getId() << " | " << visita->descricao()
                  << " | porteiro " << visita->getRegistradoPor() << "\n";
    }
}

}  // namespace

// guarda a referencia do service
MenuVisitas::MenuVisitas(VisitaService& visitaService) : visitaService_(visitaService) {}

// menu da portaria: repete ate a pessoa escolher 0 (voltar)
// cada opcao roda dentro do executar(), assim o erro aparece na tela e o programa nao fecha
void MenuVisitas::exibirMenu() {
    int opcao = -1;
    while (opcao != 0) {
        std::cout << "\n========================================\n";
        std::cout << "  Portaria: visitantes e visitas\n";
        std::cout << "========================================\n";
        std::cout << "1. Cadastrar visitante\n";
        std::cout << "2. Registrar entrada\n";
        std::cout << "3. Registrar saida\n";
        std::cout << "4. Listar visitas em aberto\n";
        std::cout << "5. Historico por apartamento\n";
        std::cout << "0. Voltar ao menu principal\n";
        opcao = lerInteiro("Escolha uma opcao: ");

        switch (opcao) {
            case 1: executar([this] { cadastrarVisitante(); }); break;
            case 2: executar([this] { registrarEntrada(); }); break;
            case 3: executar([this] { registrarSaida(); }); break;
            case 4: executar([this] { listarAbertas(); }); break;
            case 5: executar([this] { historicoPorApartamento(); }); break;
            case 0: break;
            default: std::cout << "Opcao invalida!\n"; break;
        }
    }
}

// pede os dados e chama o service pra cadastrar o visitante
void MenuVisitas::cadastrarVisitante() {
    std::cout << "\n--- Novo visitante ---\n";
    std::string nome = lerLinha("Nome: ");
    std::string cpf = lerLinha("CPF: ");
    std::string telefone = lerLinha("Telefone: ");
    int id = visitaService_.cadastrarVisitante(nome, cpf, telefone);
    std::cout << "Visitante cadastrado! ID: " << id << "\n";
}

// pede os ids e chama o service, as regras da portaria (so porteiro) estao la dentro
void MenuVisitas::registrarEntrada() {
    std::cout << "\n--- Registrar entrada ---\n";
    int visitanteId = lerInteiro("ID do visitante: ");
    int apartamentoId = lerInteiro("ID do apartamento de destino: ");
    int porteiroId = lerInteiro("ID do porteiro (funcionario com cargo Porteiro): ");
    int id = visitaService_.registrarEntrada(visitanteId, apartamentoId, porteiroId);
    std::cout << "Entrada registrada! ID da visita: " << id << "\n";
}

// pede o id da visita e chama o service pra registrar a saida
void MenuVisitas::registrarSaida() {
    std::cout << "\n--- Registrar saida ---\n";
    int visitaId = lerInteiro("ID da visita: ");
    visitaService_.registrarSaida(visitaId);
    std::cout << "Saida registrada!\n";
}

// mostra so as visitas que ainda nao tem saida
void MenuVisitas::listarAbertas() {
    std::cout << "\n--- Visitas em aberto ---\n";
    imprimirVisitas(visitaService_.listarAbertas());
}

// pede o id do apartamento e mostra todas as visitas dele
void MenuVisitas::historicoPorApartamento() {
    std::cout << "\n--- Historico por apartamento ---\n";
    int apartamentoId = lerInteiro("ID do apartamento: ");
    imprimirVisitas(visitaService_.historicoPorApartamento(apartamentoId));
}
