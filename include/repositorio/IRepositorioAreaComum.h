#ifndef IREPOSITORIOAREACOMUM_H
#define IREPOSITORIOAREACOMUM_H

#include "repositorio/IRepositorio.h"

class AreaComum;

class IRepositorioAreaComum : public IRepositorio<AreaComum> {
public:
    virtual ~IRepositorioAreaComum() = default;
};

#endif 