# D02 — Cenários de integração do MVP

Data da execução: 02/10/2026. O teste `tests/test_cenarios_integracao_d02.cpp` usa os serviços e repositórios reais sobre um banco SQLite temporário em memória, criado a partir de `sql/schema.sql`. As dez verificações rodam em sequência; uma falha interrompe o executável e faz o CTest retornar erro.

| Cenário | Fluxo e resultado esperado | Resultado |
| --- | --- | --- |
| D02-01 | Inicializar banco vazio e encontrar as sete tabelas do MVP. | Passou |
| D02-02 | Cadastrar e editar apartamento; ler a alteração persistida. | Passou |
| D02-03 | Cadastrar morador vinculado a apartamento; reconstruí-lo pelo repositório e listá-lo pelo apartamento. | Passou |
| D02-04 | Cadastrar funcionário e rejeitar CPF já usado por um morador. | Passou |
| D02-05 | Impedir remoção de apartamento com morador cadastrado, preservando o registro. | Passou |
| D02-06 | Rejeitar entrada de visita registrada por zelador e aceitar a feita por porteiro. | Passou |
| D02-07 | Registrar saída e verificar visitas abertas e histórico por apartamento. | Passou |
| D02-08 | Cadastrar/editar área comum e reconstruir o subtipo `Piscina` pelo repositório. | Passou |
| D02-09 | Criar reserva; rejeitar conflito de horário, horário fechado e excesso de capacidade; conferir valor. | Passou |
| D02-10 | Cancelar com antecedência e liberar horário; rejeitar cancelamento com menos de 24 horas. | Passou |

O relógio do teste de reservas é fixado em 01/05/2030 às 10h para que o resultado não dependa do dia em que a suíte for executada. Os demais testes já existentes continuam registrados separadamente.

## Como reproduzir no Windows

Abra um terminal com o compilador C++ configurado (por exemplo, o *Developer Command Prompt* do Visual Studio) na raiz do repositório e execute:

```powershell
cmake -S . -B build -G "NMake Makefiles"
cmake --build build
ctest --test-dir build --output-on-failure
```

Em 02/10/2026, no ambiente local com MSVC 19.51, a compilação do núcleo passou e o CTest registrou **11/11 testes aprovados**, incluindo `cenarios_integracao_d02`. O Qt 6 Widgets não foi encontrado nessa máquina; portanto, esse resultado **não** comprova compilação ou funcionamento da interface gráfica. A validação da GUI deve ser feita em máquina com Qt 6 instalado.
