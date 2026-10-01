#ifndef TELARESERVAS_H
#define TELARESERVAS_H

#include <QWidget>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

class TelaReservas : public QWidget {
    Q_OBJECT

public:
    explicit TelaReservas(QWidget *parent = nullptr);
    ~TelaReservas();

private slots:
    void aoClicarCriarReserva();

private:
    QComboBox *comboMoradores;
    QComboBox *comboAreasComuns;
    QDateTimeEdit *dateTimeInicio;
    QDateTimeEdit *dateTimeFim;
    QPushButton *btnReservar;
    QTableWidget *tabelaReservas;

    void configurarLayout();
};

#endif // TELARESERVAS_H