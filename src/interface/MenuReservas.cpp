#include "interface/MenuReservas.h"
#include <iostream>
#include <stdexcept>

namespace {

std::string lerTexto(const char* pergunta) {
    std::cout << pergunta;
    std::string valor;
    if (!std::getline(std::cin, valor)) throw std::runtime_error("entrada encerrada");
    return valor;
}

int lerInteiro(const char* pergunta) {
    for (;;) {
        const std::string entrada = lerTexto(pergunta);
        try {
            std::size_t usados = 0;
            const int valor = std::stoi(entrada, &usados);
            if (usados == entrada.size()) return valor;
        } catch (const std::exception&) {
            // Repete a pergunta abaixo.
        }
        std::cout << "Numero invalido, tente novamente.\n";
    }
}

double lerDecimal(const char* pergunta) {
    for (;;) {
        const std::string entrada = lerTexto(pergunta);
        try {
            std::size_t usados = 0;
            const double valor = std::stod(entrada, &usados);
            if (usados == entrada.size()) return valor;
        } catch (const std::exception&) {
            // Repete a pergunta abaixo.
        }
        std::cout << "Valor invalido, tente novamente.\n";
    }
}

}  // namespace

void MenuReservas::exibirMenu() {
    while (std::cin) {
        std::cout << "\n========================================\n";
        std::cout << "      Gesta'o de A'reas e Reservas      \n";
        std::cout << "========================================\n";
        std::cout << "1. Listar A'reas Comuns\n";
        std::cout << "2. Cadastrar A'rea Comum\n";
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

void MenuReservas::listarAreas() {
    auto areas = areaService_.listar();
    std::cout << "\n--- A'reas Comuns Cadastradas ---\n";
    if (areas.empty()) {
        std::cout << "Nenhuma a'rea cadastrada.\n";
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

void MenuReservas::cadastrarArea() {
    std::cout << "\n--- Nova A'rea Comum ---\n";
    const std::string tipo = lerTexto("Tipo (SalaoFestas, Piscina, Churrasqueira): ");
    const std::string nome = lerTexto("Nome: ");
    const int capacidade = lerInteiro("Capacidade: ");
    const double taxaBase = lerDecimal("Taxa Base (R$): ");
    const std::string abertura = lerTexto("Hora Abertura (HH:MM): ");
    const std::string fechamento = lerTexto("Hora Fechamento (HH:MM): ");

    int res = areaService_.cadastrar(tipo, nome, capacidade, taxaBase, abertura, fechamento);
    if (res > 0) {
        std::cout << "A'rea cadastrada com sucesso! ID: " << res << "\n";
    } else {
        std::cout << "Erro ao cadastrar a'rea.\n";
    }
}

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
        std::cout << "Falha ao criar reserva (verifique conflito de hora'rio ou limite de convidados).\n";
    }
}

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
                  << " | A'rea ID: " << r.getAreaId()
                  << " | Data: " << r.getData()
                  << " | Hor'ario: " << r.getHoraInicio() << " a's " << r.getHoraFim()
                  << " | Valor: R$ " << r.getValor() << "\n";
    }
}

void MenuReservas::cancelarReserva() {
    std::cout << "\n--- Cancelar Reserva ---\n";
    const int reservaId = lerInteiro("ID da Reserva: ");

    if (reservaService_.cancelar(reservaId)) {
        std::cout << "Reserva cancelada com sucesso!\n";
    } else {
        std::cout << "Erro ao cancelar reserva. ID na'o encontrado.\n";
    }
}
