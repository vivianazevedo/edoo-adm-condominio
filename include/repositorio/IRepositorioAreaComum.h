#ifndef IREPOSITORIOAREACOMUM_H
#define IREPOSITORIOAREACOMUM_H

#include "repositorio/IRepositorio.h"

<<<<<<< HEAD
=======
// Forward declaration para evitar dependência circular / inclusão prematura
>>>>>>> 30482671ac437199012fbc935df4a4c102074ec9
class AreaComum;

class IRepositorioAreaComum : public IRepositorio<AreaComum> {
public:
    virtual ~IRepositorioAreaComum() = default;
};

#endif 