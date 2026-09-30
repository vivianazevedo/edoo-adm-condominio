# I06: integração das fábricas com os modelos

As duas fábricas recebem o valor da coluna `tipo` do banco e devolvem um
`std::unique_ptr` para a classe-base. Os valores aceitos são exatamente os do
`sql/schema.sql`:

| Coluna | Valores |
| --- | --- |
| `pessoa.tipo` | `morador`, `visitante`, `funcionario` |
| `area_comum.tipo` | `SalaoFestas`, `Piscina`, `Churrasqueira` |

O repositório dono da entidade registra uma função de construção para cada
subclasse. Essa função pode capturar os campos lidos da linha SQLite e chamar o
construtor real da classe. Por exemplo, depois de publicar `Morador`:

```cpp
FabricaPessoa fabrica;
fabrica.registrarMorador([&] {
    return std::make_unique<Morador>(
        nome, cpf, telefone, apartamentoId,
        TipoOcupacao::Proprietario, dataEntrada, id);
});
// Registre também Visitante e Funcionario antes de consultar esses tipos.
std::unique_ptr<Pessoa> pessoa = fabrica.criar(tipoLidoDoBanco);
```

Para áreas comuns, use `registrarSalaoFestas`, `registrarPiscina` e
`registrarChurrasqueira`. Um tipo desconhecido lança `std::invalid_argument`;
um construtor que retorna `nullptr` lança `std::runtime_error`. Não há SQL nem
regras de negócio nas fábricas.

**Estado da integração (29/09):** `Pessoa` e `Morador` já estão na branch
principal. `test_fabrica_pessoa_real_i06` exercita a fábrica com o construtor
real de `Morador` e verifica os dados e o tipo dinâmico da entidade. Esse teste
simula campos lidos do banco; a ligação com `RepositorioPessoa` (P04) ainda
depende da implementação da Vivian. `Visitante`, `Funcionario` e os modelos
de áreas comuns ainda não estão completos na branch principal. Quando forem
incorporados, adicionar testes reais equivalentes e conectar os repositórios.
