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
)

target_include_directories(condominio_core PUBLIC include)
target_link_libraries(condominio_core PUBLIC SQLite::SQLite3)

if(MSVC)
    target_compile_options(condominio_core PRIVATE /W4)
else()
    target_compile_options(condominio_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

include(CTest)

if(BUILD_TESTING)
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
        src/modelo/Pessoa.cpp
        src/modelo/Morador.cpp
    )
    target_link_libraries(test_fabrica_pessoa_real_i06 PRIVATE condominio_core)

    add_test(
        NAME database_i02
        COMMAND test_database_i02
            "${CMAKE_CURRENT_BINARY_DIR}/database_i02_test.db"
            "${CMAKE_CURRENT_SOURCE_DIR}/sql/schema.sql"
    )

    add_test(NAME irepositorio_i04 COMMAND test_irepositorio_i04)
    add_test(NAME fabricas_i06 COMMAND test_fabricas_i06)
    add_test(NAME fabrica_pessoa_real_i06 COMMAND test_fabrica_pessoa_real_i06)
endif()

cmake_minimum_required(VERSION 3.16)

project(edoo_adm_condominio LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Suporte ao MOC e Recursos do Qt ---
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# --- SQLite3 e Núcleo do Projeto ---
add_library(sqlite3 STATIC third_party/sqlite/sqlite3.c)
target_include_directories(sqlite3 PUBLIC third_party/sqlite)
add_library(SQLite::SQLite3 ALIAS sqlite3)

add_library(condominio_core
    src/infra/Database.cpp
    src/infra/ErroCondominio.cpp
)

target_include_directories(condominio_core PUBLIC include)
target_link_libraries(condominio_core PUBLIC SQLite::SQLite3)

if(MSVC)
    target_compile_options(condominio_core PRIVATE /W4)
else()
    target_compile_options(condominio_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

# --- Procura o pacote do Qt 6 ---
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# --- Executável da Interface Gráfica (U01) ---
add_executable(condominio_gui
    src/main.cpp
    src/interface/MenuPrincipal.cpp
    include/interface/MenuPrincipal.h
)

target_include_directories(condominio_gui PRIVATE include)
target_link_libraries(condominio_gui PRIVATE Qt6::Widgets condominio_core)

# --- Testes ---
include(CTest)

if(BUILD_TESTING)
    add_executable(test_database_i02 tests/test_database_i02.cpp)
    target_link_libraries(test_database_i02 PRIVATE condominio_core)

    add_executable(test_irepositorio_i04 tests/test_irepositorio_i04.cpp)
    target_include_directories(test_irepositorio_i04 PRIVATE include)

    add_test(
        NAME database_i02
        COMMAND test_database_i02
            "${CMAKE_CURRENT_BINARY_DIR}/database_i02_test.db"
            "${CMAKE_CURRENT_SOURCE_DIR}/sql/schema.sql"
    )

    add_test(NAME irepositorio_i04 COMMAND test_irepositorio_i04)
endif()
