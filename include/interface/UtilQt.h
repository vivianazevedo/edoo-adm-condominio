#pragma once

// funcoes de ajuda que todas as telas qt usam
#include "infra/ErroCondominio.h"

#include <QMessageBox>
#include <QTableWidget>
#include <QWidget>

#include <exception>
#include <functional>

// devolve o id (coluna 0) da linha selecionada na tabela, se nao tiver linha joga ErroValidacao
// inline pra poder ficar no .h sem dar erro de definicao repetida
inline int idSelecionado(QTableWidget* tabela) {
    const int linha = tabela->currentRow();
    if (linha < 0 || tabela->item(linha, 0) == nullptr) {
        throw ErroValidacao("Selecione uma linha da tabela primeiro.");
    }
    return tabela->item(linha, 0)->text().toInt();
}

// roda a acao e transforma cada tipo de erro numa janela de mensagem (aviso ou erro)
// assim o programa nao fecha quando um service joga excecao
inline void executarNaTela(QWidget* tela, const std::function<void()>& acao) {
    try {
        acao();
    } catch (const ErroValidacao& erro) {
        QMessageBox::warning(tela, "Dados invalidos", QString::fromUtf8(erro.what()));
    } catch (const ErroRegraNegocio& erro) {
        QMessageBox::warning(tela, "Regra de negocio", QString::fromUtf8(erro.what()));
    } catch (const ErroBanco& erro) {
        QMessageBox::critical(tela, "Erro no banco", QString::fromUtf8(erro.what()));
    } catch (const std::exception& erro) {
        QMessageBox::critical(tela, "Erro", QString::fromUtf8(erro.what()));
    }
}
