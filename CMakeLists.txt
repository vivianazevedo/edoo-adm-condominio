cmake_minimum_required(VERSION 3.16)

project(edoo_adm_condominio LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(sqlite3 STATIC third_party/sqlite/sqlite3.c)
target_include_directories(sqlite3 PUBLIC third_party/sqlite)
add_library(SQLite::SQLite3 ALIAS sqlite3)

add_library(condominio_core
    src/infra/Database.cpp
    src/infra/ErroCondominio.cpp
    src/infra/FabricaPessoa.cpp
    src/infra/FabricaAreaComum.cpp
    src/modelo/Apartamento.cpp
    src/modelo/Pessoa.cpp
    src/modelo/Morador.cpp
    src/modelo/Visitante.cpp
    src/modelo/Funcionario.cpp
    src/modelo/Visita.cpp
    src/repositorio/RepositorioApartamento.cpp
    src/repositorio/RepositorioPessoa.cpp
    src/servico/ApartamentoService.cpp
    src/servico/MoradorService.cpp
    src/servico/FuncionarioService.cpp
    src/repositorio/RepositorioVisita.cpp
    src/modelo/AreaComum.cpp
    src/modelo/SalaoFestas.cpp
    src/modelo/Piscina.cpp
    src/modelo/Churrasqueira.cpp
    src/modelo/Reserva.cpp
    src/repositorio/RepositorioAreaComum.cpp
    src/repositorio/RepositorioReserva.cpp
    src/servico/AreaComumService.cpp
    src/servico/ReservaService.cpp
        src/servico/ReservaService.cpp
    src/servico/VisitaService.cpp
)

target_include_directories(condominio_core PUBLIC include)
target_link_libraries(condominio_core PUBLIC SQLite::SQLite3)

# Os executaveis encontram o schema ao iniciar a partir da pasta de build.
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/sql" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

add_executable(condominio_terminal
    src/terminal/main_terminal.cpp
    src/interface/MenuMoradores.cpp
    src/interface/MenuReservas.cpp
)
target_include_directories(condominio_terminal PRIVATE include)
target_link_libraries(condominio_terminal PRIVATE condominio_core)

if(MSVC)
    target_compile_options(condominio_core PRIVATE /W4)
    target_compile_options(condominio_terminal PRIVATE /W4)
else()
    target_compile_options(condominio_core PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(condominio_terminal PRIVATE -Wall -Wextra -Wpedantic)
endif()

find_package(Qt6 QUIET COMPONENTS Widgets)

if(Qt6_FOUND)
    add_executable(condominio_gui
        src/main.cpp
        src/interface/MenuPrincipal.cpp
        src/interface/TelaMoradores.cpp
        src/interface/TelaReservas.cpp
        src/interface/TelaVisitas.cpp
        include/interface/MenuPrincipal.h
        include/interface/TelaMoradores.h
        include/interface/TelaReservas.h
        include/interface/TelaVisitas.h
    )
    target_include_directories(condominio_gui PRIVATE include)
    target_link_libraries(condominio_gui PRIVATE Qt6::Widgets condominio_core)
    set_target_properties(condominio_gui PROPERTIES AUTOMOC ON AUTORCC ON AUTOUIC ON)
else()
    message(STATUS "Qt 6 Widgets nao encontrado: compilando apenas o nucleo e os testes")
endif()

include(CTest)

if(BUILD_TESTING)
    add_test(NAME terminal_inicia COMMAND condominio_terminal --smoke)

    add_executable(test_database_i02 tests/test_database_i02.cpp)
    target_link_libraries(test_database_i02 PRIVATE condominio_core)

    add_executable(test_irepositorio_i04 tests/test_irepositorio_i04.cpp)
    target_include_directories(test_irepositorio_i04 PRIVATE include)

    add_executable(test_fabricas_i06
        tests/test_fabricas_i06.cpp
        src/infra/FabricaPessoa.cpp
        src/infra/FabricaAreaComum.cpp
    )
    target_include_directories(test_fabricas_i06 PRIVATE include)

    add_executable(test_fabrica_pessoa_real_i06
        tests/test_fabrica_pessoa_real_i06.cpp
    )
    target_link_libraries(test_fabrica_pessoa_real_i06 PRIVATE condominio_core)

    add_executable(test_integracao_pessoas_i06 tests/test_integracao_pessoas_i06.cpp)
    target_link_libraries(test_integracao_pessoas_i06 PRIVATE condominio_core)

    add_executable(test_servicos_pessoas_p05 tests/test_servicos_pessoas_p05.cpp)
    target_link_libraries(test_servicos_pessoas_p05 PRIVATE condominio_core)
    add_executable(test_integracao_visitas tests/test_integracao_visitas.cpp)
    target_link_libraries(test_integracao_visitas PRIVATE condominio_core)
    add_executable(test_modelos_areas_reservas tests/test_modelos_areas_reservas.cpp)
    target_link_libraries(test_modelos_areas_reservas PRIVATE condominio_core)

    add_executable(test_repositorios_areas_reservas tests/test_repositorios_areas_reservas.cpp)
    target_link_libraries(test_repositorios_areas_reservas PRIVATE condominio_core)

    add_executable(test_servicos_areas_reservas tests/test_servicos_areas_reservas.cpp)
    target_link_libraries(test_servicos_areas_reservas PRIVATE condominio_core)

    add_test(
        NAME database_i02
        COMMAND test_database_i02
            "${CMAKE_CURRENT_BINARY_DIR}/database_i02_test.db"
            "${CMAKE_CURRENT_SOURCE_DIR}/sql/schema.sql"
    )

    add_test(NAME irepositorio_i04 COMMAND test_irepositorio_i04)
    add_test(NAME fabricas_i06 COMMAND test_fabricas_i06)
    add_test(NAME fabrica_pessoa_real_i06 COMMAND test_fabrica_pessoa_real_i06)
    add_test(
        NAME integracao_pessoas_i06
        COMMAND test_integracao_pessoas_i06 "${CMAKE_CURRENT_SOURCE_DIR}/sql/schema.sql"
    )
    add_test(
        NAME servicos_pessoas_p05
        COMMAND test_servicos_pessoas_p05 "${CMAKE_CURRENT_SOURCE_DIR}/sql/schema.sql"
    )
    add_test(
        NAME integracao_visitas
        COMMAND test_integracao_visitas "${CMAKE_CURRENT_SOURCE_DIR}/sql/schema.sql"
    )
    add_test(NAME modelos_areas_reservas COMMAND test_modelos_areas_reservas)
    add_test(
        NAME repositorios_areas_reservas
        COMMAND test_repositorios_areas_reservas "${CMAKE_CURRENT_SOURCE_DIR}/sql/schema.sql"
    )
    add_test(
        NAME servicos_areas_reservas
        COMMAND test_servicos_areas_reservas "${CMAKE_CURRENT_SOURCE_DIR}/sql/schema.sql"
    )
endif()
