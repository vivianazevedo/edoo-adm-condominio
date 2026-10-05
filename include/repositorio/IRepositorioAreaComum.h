#ifndef IREPOSITORIOAREACOMUM_H
#define IREPOSITORIOAREACOMUM_H

#include "repositorio/IRepositorio.h"

class AreaComum;

// repositorio de areas comuns: so define que o T do IRepositorio e AreaComum
class IRepositorioAreaComum : public IRepositorio<AreaComum> {
public:
    virtual ~IRepositorioAreaComum() = default;
};

#endif 