#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>

#include "infra/Database.h"
#include "interface/MenuPrincipal.h"

#include <exception>
#include <iostream>
#include <stdexcept>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
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
