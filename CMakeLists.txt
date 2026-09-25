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

    add_test(
        NAME database_i02
        COMMAND test_database_i02
            "${CMAKE_CURRENT_BINARY_DIR}/database_i02_test.db"
            "${CMAKE_CURRENT_SOURCE_DIR}/sql/schema.sql"
    )
endif()
