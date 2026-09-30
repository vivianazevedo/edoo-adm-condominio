#include "interface/MenuReservas.h"
#include <iostream>
#include <limits>

void MenuReservas::exibirMenu() {
    int opcao = -1;
    while (opcao != 0) {
        std::cout << "\n========================================\n";
        std::cout << "      Gesta'o de A'reas e Reservas      \n";
        std::cout << "========================================\n";
        std::cout << "1. Listar A'reas Comuns\n";
        std::cout << "2. Cadastrar A'rea Comum\n";
        std::cout << "3. Solicitar Reserva\n";
        std::cout << "4. Listar Reservas por Morador\n";
        std::cout << "5. Cancelar Reserva\n";
        std::cout << "0. Voltar ao Menu Principal\n";
        std::cout << "Escolha uma opca'o: ";
        
        if (!(std::cin >> opcao)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (opcao) {
            case 1: listarAreas(); break;
            case 2: cadastrarArea(); break;
            case 3: criarReserva(); break;
            case 4: listarMinhasReservas(); break;
            case 5: cancelarReserva(); break;
            case 0: std::cout << "A voltar...\n"; break;
            default: std::cout << "Opca'o inva'lida!\n"; break;
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
    std::string tipo, nome, abertura, fechamento;
    int capacidade;
    double taxaBase;

    std::cout << "\n--- Nova A'rea Comum ---\n";
    std::cout << "Tipo (SalaoFestas, Piscina, Churrasqueira): ";
    std::cin >> tipo;
    std::cout << "Nome: ";
    std::cin.ignore();
    std::getline(std::cin, nome);
    std::cout << "Capacidade: ";
    std::cin >> capacidade;
    std::cout << "Taxa Base (R$): ";
    std::cin >> taxaBase;
    std::cout << "Hora Abertura (HH:MM): ";
    std::cin >> abertura;
    std::cout << "Hora Fechamento (HH:MM): ";
    std::cin >> fechamento;

    int res = areaService_.cadastrar(tipo, nome, capacidade, taxaBase, abertura, fechamento);
    if (res > 0) {
        std::cout << "A'rea cadastrada com sucesso! ID: " << res << "\n";
    } else {
        std::cout << "Erro ao cadastrar a'rea.\n";
    }
}

void MenuReservas::criarReserva() {
    int moradorId, areaId, convidados;
    std::string data, horaInicio, horaFim;

    std::cout << "\n--- Nova Reserva ---\n";
    std::cout << "ID do Morador: ";
    std::cin >> moradorId;
    std::cout << "ID da A'rea Comum: ";
    std::cin >> areaId;
    std::cout << "Data (AAAA-MM-DD): ";
    std::cin >> data;
    std::cout << "Hora In'cio (HH:MM): ";
    std::cin >> horaInicio;
    std::cout << "Hora Fim (HH:MM): ";
    std::cin >> horaFim;
    std::cout << "Nu'mero de Convidados: ";
    std::cin >> convidados;

    int idReserva = reservaService_.criar(moradorId, areaId, data, horaInicio, horaFim, convidados);
    if (idReserva > 0) {
        std::cout << "Reserva efetuada com sucesso! ID da Reserva: " << idReserva << "\n";
    } else {
        std::cout << "Falha ao criar reserva (verifique conflito de hora'rio ou limite de convidados).\n";
    }
}

void MenuReservas::listarMinhasReservas() {
    int moradorId;
    std::cout << "\n--- Reservas do Morador ---\n";
    std::cout << "ID do Morador: ";
    std::cin >> moradorId;

    auto reservas = reservaService_.listarPorMorador(moradorId);
    if (reservas.empty()) {
        std::cout << "Nenhuma reserva encontrada para este morador.\n";
        return;
    }
    for (const auto& r : reservas) {
        std::cout << "ID Reserva: " << r->getId()
                  << " | A'rea ID: " << r->getAreaId()
                  << " | Data: " << r->getData()
                  << " | Hor'ario: " << r->getHoraInicio() << " a's " << r->getHoraFim()
                  << " | Valor: R$ " << r->getValor() << "\n";
    }
}

void MenuReservas::cancelarReserva() {
    int reservaId;
    std::cout << "\n--- Cancelar Reserva ---\n";
    std::cout << "ID da Reserva: ";
    std::cin >> reservaId;

    if (reservaService_.cancelar(reservaId)) {
        std::cout << "Reserva cancelada com sucesso!\n";
    } else {
        std::cout << "Erro ao cancelar reserva. ID na'o encontrado.\n";
    }
}