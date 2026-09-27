# Guia rápido: `IRepositorio<T>`

`IRepositorio<T>` é o contrato comum para os repositórios de dados. Cada módulo cria
uma classe que herda de `IRepositorio<Entidade>` e implementa os cinco métodos. A
interface fica em `include/repositorio/IRepositorio.h`; não contém SQL.

| Método | Resultado esperado |
| --- | --- |
| `inserir(const T&)` | Grava e devolve o ID criado. |
| `buscarPorId(int)` | Devolve um objeto ou `nullptr` se não encontrou. |
| `listar()` | Devolve uma coleção de objetos. |
| `atualizar(const T&)` | Devolve `true` se alterou, `false` se o ID não existe. |
| `remover(int)` | Devolve `true` se removeu, `false` se o ID não existe. |

As consultas devolvem `std::unique_ptr<T>`: quem recebe o resultado passa a ser o
dono do objeto. Isso permite que um `IRepositorio<Pessoa>` devolva um `Morador` ou
`Visitante` sem copiar apenas a parte `Pessoa`. Nunca devolva um ponteiro para uma
variável local ou para memória controlada pelo repositório.

Exemplo de assinatura de uma implementação para apartamentos:

```cpp
class RepositorioApartamento : public IRepositorio<Apartamento> {
public:
    int inserir(const Apartamento& apartamento) override;
    std::unique_ptr<Apartamento> buscarPorId(int id) override;
    std::vector<std::unique_ptr<Apartamento>> listar() override;
    bool atualizar(const Apartamento& apartamento) override;
    bool remover(int id) override;
};
```

Use `override` em todos os métodos: o compilador avisará se a assinatura estiver
incompatível. Os métodos que retornam `bool` indicam apenas se o registro existia;
falhas de SQLite devem gerar `ErroBanco`. SQL pertence às classes de repositório;
regras como conflito de reserva pertencem aos serviços.

Um exemplo completo e executável, usando memória no lugar do SQLite para deixar o
contrato visível, está em `tests/test_irepositorio_i04.cpp`.
