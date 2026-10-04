#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>

#include "infra/Database.h"
#include "interface/MenuPrincipal.h"

#include <exception>
#include <iostream>
#include <stdexcept>

// tema de cores da interface (qss, parecido com css)
static const char* kTema = R"QSS(
QMainWindow, QWidget { background-color: #F4F7FA; color: #1F2D3A; font-size: 13px; }
QLabel { background: transparent; }

/* abas */
QTabWidget::pane { border: 1px solid #C9D6E0; background: #F4F7FA; }
QTabBar::tab { background: #DCE7EF; padding: 8px 18px; margin-right: 2px;
               border-top-left-radius: 6px; border-top-right-radius: 6px; }
QTabBar::tab:selected { background: #2F6F8F; color: white; }
QTabBar::tab:hover:!selected { background: #C5D8E5; }

/* caixas de grupo */
QGroupBox { background: white; border: 1px solid #C9D6E0; border-radius: 8px;
            margin-top: 14px; padding: 10px; font-weight: bold; }
QGroupBox::title { subcontrol-origin: margin; left: 12px; padding: 0 6px; color: #2F6F8F; }

/* campos de formulario */
QLineEdit, QComboBox, QSpinBox, QDoubleSpinBox, QDateEdit, QTimeEdit {
    background: white; border: 1px solid #B7C6D2; border-radius: 4px; padding: 4px; }
QLineEdit:focus, QComboBox:focus, QSpinBox:focus, QDoubleSpinBox:focus,
QDateEdit:focus, QTimeEdit:focus { border: 1px solid #2F6F8F; }

/* botoes */
QPushButton { background: #2F6F8F; color: white; border: none;
              border-radius: 6px; padding: 7px 14px; }
QPushButton:hover { background: #3C86AB; }
QPushButton:pressed { background: #245870; }

/* botoes de remover e cancelar em vermelho */
QPushButton#aptoRemover, QPushButton#moradorRemover, QPushButton#areaRemover,
QPushButton#funcionarioRemover, QPushButton#reservaCancelar { background: #C0392B; }
QPushButton#aptoRemover:hover, QPushButton#moradorRemover:hover, QPushButton#areaRemover:hover,
QPushButton#funcionarioRemover:hover, QPushButton#reservaCancelar:hover { background: #D9534F; }

/* tabelas */
QTableWidget { background: white; alternate-background-color: #EEF4F8;
               gridline-color: #E1E8EE; border: 1px solid #C9D6E0;
               selection-background-color: #CFE8F5; selection-color: #1F2D3A; }
QHeaderView::section { background: #2F6F8F; color: white; padding: 6px;
                       border: none; font-weight: bold; }
)QSS";

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setStyle("Fusion");  // estilo base igual em qualquer sistema
    app.setStyleSheet(QString::fromUtf8(kTema));  // aplica o tema nas telas todas

    const bool smoke = argc >= 2 && QString::fromLocal8Bit(argv[1]) == "--smoke";
    try {
        QString schema = QDir::current().filePath("sql/schema.sql");
        if (!QFileInfo::exists(schema)) {
            schema = QDir(QApplication::applicationDirPath()).filePath("sql/schema.sql");
        }
        if (smoke && argc >= 3) schema = QString::fromLocal8Bit(argv[2]);
        if (!QFileInfo::exists(schema)) {
            throw std::runtime_error("sql/schema.sql nao encontrado");
        }
        Database::instancia(smoke ? ":memory:" : "condominio.db", schema.toStdString());
        MenuPrincipal menu;
        if (smoke) return 0;
        menu.show();
        return app.exec();
    } catch (const std::exception& erro) {
        std::cerr << "Falha ao iniciar a interface: " << erro.what() << '\n';
        if (!smoke) QMessageBox::critical(nullptr, "Falha ao iniciar", QString::fromUtf8(erro.what()));
        return 1;
    }
}