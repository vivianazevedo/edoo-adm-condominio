# Interface grafica (Qt 6)

O executavel `condominio_gui` usa os servicos e repositorios do nucleo. As telas
nao executam SQL diretamente. O arquivo `condominio.db` e criado no diretorio
de onde o programa e iniciado; por isso, execute-o sempre a partir da mesma
pasta se quiser reencontrar os dados cadastrados.

## Preparacao

Instale Qt 6 com o modulo Widgets e um kit compativel com o compilador C++.
Configure o projeto com CMake, informando a pasta de instalacao desse kit em
`CMAKE_PREFIX_PATH`, e compile o alvo `condominio_gui`. O arquivo
`sql/schema.sql` deve estar na pasta de trabalho ou em `sql/` ao lado do
executavel. O PR de integracao do terminal (G05) copia essa pasta para o build.

No Windows, as DLLs do Qt precisam estar no `PATH` ao iniciar o programa. No
Linux, use a configuracao de bibliotecas do kit instalado. Se a janela nao
abrir, verifique primeiro o caminho do Qt e do `sql/schema.sql`.

## Uso

1. Em **Moradores e Aptos**, cadastre primeiro um apartamento e depois um
   morador. Clique numa linha para preencher o formulario antes de editar ou
   remover. Um apartamento ocupado nao pode ser removido.
2. Em **Areas e Reservas**, cadastre uma area, escolha um morador e reserve um
   horario. O sistema recusa horarios sobrepostos e aplica as regras de
   capacidade, horario e antecedencia para cancelamento.
3. Em **Portaria e Visitas**, cadastre um funcionario com cargo **Porteiro** e
   um visitante. Selecione visitante, apartamento e porteiro para registrar
   entrada. Escolha a visita na tabela para registrar a saida. O filtro permite
   consultar o historico por apartamento.

As mensagens de erro dos servicos aparecem em janelas explicativas. Os dados
sao atualizados quando se muda de aba.

## Verificacao automatica

Com Qt encontrado pelo CMake, `ctest --test-dir <pasta-build> --output-on-failure`
inclui o caso `gui_fluxos`. Ele aciona os botoes reais das telas com o plugin
Qt `offscreen` e usa SQLite em memoria. Cobre cadastro, edicao e remocao,
entrada e saida de visita, cancelamento de reserva e algumas recusas de regras
de negocio. Ele nao substitui a verificacao visual da janela em uma maquina
com ambiente grafico.

O argumento `--smoke <caminho-para-schema.sql>` abre banco em memoria, constroi
as tres telas e encerra sem mostrar janela. Ele e util para conferir a
inicializacao, mas nao prova a usabilidade visual.
