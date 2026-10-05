#pragma once
#include <string>
#include <vector>



class Morador;// avisa que Morador existe (basta isso porque so guardamos ponteiros)

// apartamento do condominio, guarda ponteiros dos moradores que moram nele
class Apartamento {

    private:
        int id_;
        std::string bloco_;
        std::string numero_;
        int andar_;
        std::vector<Morador*> moradores_;  // nao e dono: nunca da delete aqui

    public:
        Apartamento(const std::string& bloco, const std::string& numero, int andar, int id = 0); // id 0 quer dizer que ainda nao foi salvo no banco

        // getters e setters (os setters de bloco, numero e andar validam)
        int getId() const;
        const std::string& getBloco() const;
        const std::string& getNumero() const;
        int getAndar() const;

        void setId(int id);
        void setBloco(const std::string& bloco);
        void setNumero(const std::string& numero);
        void setAndar(int andar);

        // ligacao com os moradores
        void adicionarMorador(Morador* morador);
        bool removerMorador(int moradorId);   // true se achou e tirou
        const std::vector<Morador*>& getMoradores() const;
        int quantidadeMoradores() const;
        bool temMoradores() const;            
    
        std::string descricao() const;// texto pras telas


};