#pragma once
#include <string>

using namespace std;

// uma visita e um evento: um visitante entrou num apartamento, registrado por um porteiro
// cada entrada vira uma visita nova, assim o historico da pessoa fica guardado
// guarda so os ids, quem carrega os objetos completos e o service
class Visita {

    private:
        int id_;
        int visitanteId_;
        int apartamentoId_;
        int registradoPor_;   // id do funcionario (porteiro) que registrou
        string entrada_;      // formato AAAA-MM-DD HH:MM
        string saida_;        // vazio = visita ainda aberta

    public:
        // id 0 quer dizer que ainda nao foi salvo no banco
        Visita(int visitanteId, int apartamentoId, int registradoPor,
               const string& entrada, const string& saida = "", int id = 0);

        int getId() const;
        int getVisitanteId() const;
        int getApartamentoId() const;
        int getRegistradoPor() const;
        const string& getEntrada() const;
        const string& getSaida() const;

        void setId(int id);
        void setVisitanteId(int visitanteId);
        void setApartamentoId(int apartamentoId);
        void setRegistradoPor(int funcionarioId);
        void setEntrada(const string& entrada);
        void setSaida(const string& saida);   // "" reabre a visita

        bool estaAberta() const;              // true se ainda nao tem saida
        string descricao() const;             // texto pras telas
};