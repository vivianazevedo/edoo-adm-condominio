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
    return std::make_unique<Morador>(/* campos lidos da linha */);
});
// Registre também Visitante e Funcionario antes de consultar esses tipos.
std::unique_ptr<Pessoa> pessoa = fabrica.criar(tipoLidoDoBanco);
```

Para áreas comuns, use `registrarSalaoFestas`, `registrarPiscina` e
`registrarChurrasqueira`. Um tipo desconhecido lança `std::invalid_argument`;
um construtor que retorna `nullptr` lança `std::runtime_error`. Não há SQL nem
regras de negócio nas fábricas.

**Pendente para a integração:** os modelos em `include/modelo/` ainda estavam
vazios em 27/09. Quando Vivian e Beatriz publicarem seus construtores finais,
os repositórios P04 e A04 devem registrar essas funções com os campos da linha
SQLite, e os testes devem usar os modelos reais além dos exemplos isolados.
