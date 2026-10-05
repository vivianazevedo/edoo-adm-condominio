#include "interface/MenuReservas.h"
#include <iostream>
#include <stdexcept>

namespace {

// le uma linha, se a entrada acabar joga erro pro menu conseguir sair
std::string lerTexto(const char* pergunta) {
    std::cout << pergunta;
    std::string valor;
    if (!std::getline(std::cin, valor)) throw std::runtime_error("entrada encerrada");
    return valor;
}

// repete a pergunta ate digitarem um numero inteiro valido
int lerInteiro(const char* pergunta) {
    for (;;) {
        const std::string entrada = lerTexto(pergunta);
        try {
            std::size_t usados = 0;
            const int valor = std::stoi(entrada, &usados);
            if (usados == entrada.size()) return valor;
        } catch (const std::exception&) {
            // nao faz nada aqui, a pergunta repete la embaixo
        }
        std::cout << "Numero invalido, tente novamente.\n";
    }
}

// repete a pergunta ate digitarem um numero decimal valido
double lerDecimal(const char* pergunta) {
    for (;;) {
        const std::string entrada = lerTexto(pergunta);
        try {
            std::size_t usados = 0;
            const double valor = std::stod(entrada, &usados);
            if (usados == entrada.size()) return valor;
        } catch (const std::exception&) {
            // nao faz nada aqui, a pergunta repete la embaixo
        }
        std::cout << "Valor invalido, tente novamente.\n";
    }
}

}  // namespace

// repete o menu ate escolher 0 ou a entrada acabar, erros aparecem na tela
void MenuReservas::exibirMenu() {
    while (std::cin) {
        std::cout << "\n========================================\n";
        std::cout << "      Gestao de Areas e Reservas      \n";
        std::cout << "========================================\n";
        std::cout << "1. Listar Areas Comuns\n";
        std::cout << "2. Cadastrar Area Comum\n";
        std::cout << "3. Solicitar Reserva\n";
        std::cout << "4. Listar Reservas por Morador\n";
        std::cout << "5. Cancelar Reserva\n";
        std::cout << "0. Voltar ao Menu Principal\n";
        try {
            const int opcao = lerInteiro("Escolha uma opcao: ");
            if (opcao == 0) return;
            switch (opcao) {
                case 1: listarAreas(); break;
                case 2: cadastrarArea(); break;
                case 3: criarReserva(); break;
                case 4: listarMinhasReservas(); break;
                case 5: cancelarReserva(); break;
                default: std::cout << "Opcao invalida!\n"; break;
            }
        } catch (const std::exception& erro) {
            if (!std::cin) return;
            std::cerr << "Operacao nao concluida: " << erro.what() << '\n';
        }
    }
}

// lista as areas cadastradas (ou avisa que nao tem nenhuma)
void MenuReservas::listarAreas() {
    auto areas = areaService_.listar();
    std::cout << "\n--- Areas Comuns Cadastradas ---\n";
    if (areas.empty()) {
        std::cout << "Nenhuma area cadastrada.\n";
        return;
    }
    for (const auto& a : areas) {
        std::cout << "ID: " << a->getId()
                  << " | Nome: " << a->getNome()
                  << " | Tipo: " << a->getTipo()
                  << " | Cap.: " << a->getCapacidade()
                  << " | Taxa Base: R$ " << a->getTaxaBase() << "\n";
    }
}

// pede os dados da area e chama o service pra cadastrar
void MenuReservas::cadastrarArea() {
    std::cout << "\n--- Nova Area Comum ---\n";
    const std::string tipo = lerTexto("Tipo (SalaoFestas, Piscina, Churrasqueira): ");
    const std::string nome = lerTexto("Nome: ");
    const int capacidade = lerInteiro("Capacidade: ");
    const double taxaBase = lerDecimal("Taxa Base (R$): ");
    const std::string abertura = lerTexto("Hora Abertura (HH:MM): ");
    const std::string fechamento = lerTexto("Hora Fechamento (HH:MM): ");

    int res = areaService_.cadastrar(tipo, nome, capacidade, taxaBase, abertura, fechamento);
    if (res > 0) {
        std::cout << "Area cadastrada com sucesso! ID: " << res << "\n";
    } else {
        std::cout << "Erro ao cadastrar area.\n";
    }
}

// pede os dados e chama o service, as regras de reserva estao la dentro
void MenuReservas::criarReserva() {
    std::cout << "\n--- Nova Reserva ---\n";
    const int moradorId = lerInteiro("ID do Morador: ");
    const int areaId = lerInteiro("ID da Area Comum: ");
    const std::string data = lerTexto("Data (AAAA-MM-DD): ");
    const std::string horaInicio = lerTexto("Hora Inicio (HH:MM): ");
    const std::string horaFim = lerTexto("Hora Fim (HH:MM): ");
    const int convidados = lerInteiro("Numero de Convidados: ");

    int idReserva = reservaService_.criar(moradorId, areaId, data, horaInicio, horaFim, convidados);
    if (idReserva > 0) {
        std::cout << "Reserva efetuada com sucesso! ID da Reserva: " << idReserva << "\n";
    } else {
        std::cout << "Falha ao criar reserva (verifique conflito de horario ou limite de convidados).\n";
    }
}

// pede o id do morador e lista as reservas dele
void MenuReservas::listarMinhasReservas() {
    std::cout << "\n--- Reservas do Morador ---\n";
    const int moradorId = lerInteiro("ID do Morador: ");

    auto reservas = reservaService_.listarPorMorador(moradorId);
    if (reservas.empty()) {
        std::cout << "Nenhuma reserva encontrada para este morador.\n";
        return;
    }
    for (const auto& r : reservas) {
        std::cout << "ID Reserva: " << r.getId()
                  << " | Area ID: " << r.getAreaId()
                  << " | Data: " << r.getData()
                  << " | Horario: " << r.getHoraInicio() << " as " << r.getHoraFim()
                  << " | Valor: R$ " << r.getValor() << "\n";
    }
}

// pede o id da reserva e chama o service pra cancelar
void MenuReservas::cancelarReserva() {
    std::cout << "\n--- Cancelar Reserva ---\n";
    const int reservaId = lerInteiro("ID da Reserva: ");

    if (reservaService_.cancelar(reservaId)) {
        std::cout << "Reserva cancelada com sucesso!\n";
    } else {
        std::cout << "Erro ao cancelar reserva. ID nao encontrado.\n";
    }
}
