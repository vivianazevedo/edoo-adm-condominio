#ifndef IREPOSITORIOAREACOMUM_H
#define IREPOSITORIOAREACOMUM_H

#include "repositorio/IRepositorio.h"

// Forward declaration para evitar dependência circular / inclusão prematura
class AreaComum;

class IRepositorioAreaComum : public IRepositorio<AreaComum> {
public:
    virtual ~IRepositorioAreaComum() = default;
};

#endif 