#include "infra/Database.h"
#include "interface/MenuMoradores.h"
#include "interface/MenuReservas.h"
#include "interface/MenuVisitas.h"
#include "repositorio/RepositorioApartamento.h"
#include "repositorio/RepositorioPessoa.h"
#include "repositorio/RepositorioVisita.h"
#include "servico/ApartamentoService.h"
#include "servico/FuncionarioService.h"
#include "servico/MoradorService.h"
#include "servico/VisitaService.h"

#include <exception>
#include <iostream>
#include <string>

// main do terminal: abre o banco, monta repositorios, services e menus e mostra o menu principal
int main(int argc, char* argv[]) {
    try {
        // o cmake copia a pasta sql pra pasta de build, e na raiz do projeto ela ja existe
        Database::instancia("condominio.db", "sql/schema.sql");
        // com --smoke so abre o banco e sai (usado no teste automatico)
        if (argc == 2 && std::string(argv[1]) == "--smoke") {
            std::cout << "Terminal e banco inicializados.\n";
            return 0;
        }
        if (argc != 1) {
            std::cerr << "Uso: condominio_terminal [--smoke]\n";
            return 2;
        }

        // montagem de baixo pra cima: repositorios -> services -> menus
        // cada um recebe o de baixo por referencia
        RepositorioApartamento repoApartamentos;
        RepositorioPessoa repoPessoas;
        ApartamentoService apartamentos(repoApartamentos);
        MoradorService moradores(repoPessoas, repoApartamentos);
        FuncionarioService funcionarios(repoPessoas);
        MenuMoradores menuPessoas(apartamentos, moradores, funcionarios);
        MenuReservas menuReservas;
        RepositorioVisita repoVisitas;
        VisitaService visitas(repoPessoas, repoVisitas);
        MenuVisitas menuVisitas(visitas);

        // repete o menu principal ate digitar 0 ou a entrada acabar
        std::string opcao;
        while (std::cin) {
            std::cout << "\n=== Sistema de Gestao de Condominio ===\n"
                      << "1. Apartamentos, moradores e funcionarios\n"
                      << "2. Areas comuns e reservas\n"
                      << "3. Portaria: visitantes e visitas\n"
                      << "0. Sair\nEscolha: ";
            if (!std::getline(std::cin, opcao) || opcao == "0") break;
            try {
                if (opcao == "1") menuPessoas.exibirMenu();
                else if (opcao == "2") menuReservas.exibirMenu();
                else if (opcao == "3") menuVisitas.exibirMenu();
                else std::cout << "Opcao invalida.\n";
            } catch (const std::exception& erro) {
                std::cerr << "Operacao nao concluida: " << erro.what() << '\n';
            }
        }
        return 0;
    } catch (const std::exception& erro) {
        std::cerr << "Falha ao iniciar o sistema: " << erro.what() << '\n';
        return 1;
    }
}
