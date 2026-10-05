#pragma once
#include "servico/ApartamentoService.h"
#include "servico/FuncionarioService.h"
#include "servico/MoradorService.h"


// menu de terminal de apartamentos, moradores e funcionarios
// nao guarda dados: so le o que a pessoa digita e chama os servicos
class MenuMoradores {
private:
    ApartamentoService& apartamentoService_;
    MoradorService& moradorService_;
    FuncionarioService& funcionarioService_;

    // submenus
    void menuApartamentos();
    void menuMoradores();
    void menuFuncionarios();

    // apartamentos
    void cadastrarApartamento();
    void listarApartamentos();
    void editarApartamento();
    void removerApartamento();

    // moradores
    void cadastrarMorador();
    void listarMoradores();
    void listarMoradoresPorApartamento();
    void editarMorador();
    void removerMorador();

    // funcionarios
    void cadastrarFuncionario();
    void listarFuncionarios();
    void editarFuncionario();
    void removerFuncionario();

public:
    MenuMoradores(ApartamentoService& apartamentoService,
                  MoradorService& moradorService,
                  FuncionarioService& funcionarioService);

    // mostra o menu ate a pessoa escolher voltar
    void exibirMenu();
};