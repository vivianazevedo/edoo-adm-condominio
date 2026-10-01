#ifndef TELAMORADORES_H
#define TELAMORADORES_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

class TelaMoradores : public QWidget {
    Q_OBJECT

public:
    explicit TelaMoradores(QWidget *parent = nullptr);
    ~TelaMoradores();

private slots:
    void aoClicarCadastrar();

private:
    QLineEdit *txtNome;
    QLineEdit *txtCpf;
    QLineEdit *txtTelefone;
    QLineEdit *txtApartamento;
    QPushButton *btnCadastrar;
    QTableWidget *tabelaMoradores;

    void configurarLayout();
};

#endif // TELAMORADORES_H