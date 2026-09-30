#pragma once
#include <string>
#include <vector>

using namespace std;


class Morador;// declara morador 

class Apartamento {
public:
    Apartamento(const string& bloco, const string& numero, int andar, int id = 0); // id 0 quer dizer que ainda nao foi salvo no banco

    int getId() const;
    const string& getBloco() const;
    const string& getNumero() const;
    int getAndar() const;

    void setId(int id);
    void setBloco(const string& bloco);
    void setNumero(const string& numero);
    void setAndar(int andar);

    // ligacao com os moradores
    void adicionarMorador(Morador* morador);
    bool removerMorador(int moradorId);   // true se achou e tirou
    const vector<Morador*>& getMoradores() const;
    int quantidadeMoradores() const;
    bool temMoradores() const;            // usado na rn08
 
    string descricao() const;// texto pras telas

private:
    int id_;
    string bloco_;
    string numero_;
    int andar_;
    vector<Morador*> moradores_;  // nao e dono: nunca da delete aqui
};