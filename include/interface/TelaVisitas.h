#ifndef TELAVISITAS_H
#define TELAVISITAS_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

class TelaVisitas : public QWidget {
    Q_OBJECT

public:
    explicit TelaVisitas(QWidget *parent = nullptr);
    ~TelaVisitas();

private slots:
    void aoClicarRegistrarEntrada();

private:
    QLineEdit *txtVisitante;
    QLineEdit *txtDocumento;
    QLineEdit *txtApto;
    QPushButton *btnEntrada;
    QTableWidget *tabelaVisitas;

    void configurarLayout();
};

#endif // TELAVISITAS_H