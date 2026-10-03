# Compilar e executar localmente

No Windows, abra o **Developer Command Prompt** do Visual Studio na raiz do repositório. O compilador C++ e o CMake precisam estar instalados. Para verificar o núcleo e o terminal:

```powershell
cmake -S . -B build -G "NMake Makefiles"
cmake --build build
ctest --test-dir build --output-on-failure
cd build
./condominio_terminal.exe
```

O CMake copia `sql/schema.sql` e `sql/seed.sql` para `build/sql/`. Na primeira execução, `condominio_terminal` cria `condominio.db` na pasta corrente e aplica o esquema. O arquivo `.db` não deve ser versionado. O seed contém dados fictícios para demonstração e **não** é aplicado automaticamente; não o execute sobre um banco já preenchido.

Para compilar também a interface gráfica, é preciso instalar um Qt 6 Widgets compatível com o compilador utilizado e informar sua localização ao CMake, se ele não for encontrado automaticamente:

```powershell
cmake -S . -B build-qt -G "NMake Makefiles" -DCMAKE_PREFIX_PATH="CAMINHO_DA_INSTALACAO_QT"
cmake --build build-qt
```

Quando Qt 6 Widgets não está disponível, o CMake informa isso e compila **apenas** núcleo, terminal e testes; não se deve interpretar esse build como validação da GUI. O critério G05 de build do zero em **duas máquinas** exige repetir os passos acima em outro computador e registrar o resultado.
