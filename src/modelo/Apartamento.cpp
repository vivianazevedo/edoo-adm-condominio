#include "modelo/Apartamento.h"
#include "modelo/Morador.h"
#include "infra/ErroCondominio.h"

using namespace std; 

// usa os setters pra validacao valer ja na criacao
Apartamento::Apartamento(const string& bloco, const string& numero,
                         int andar, int id)
    : id_(id), andar_(0) {
    setBloco(bloco);
    setNumero(numero);
    setAndar(andar);
}

int Apartamento::getId() const { return id_; }
const string& Apartamento::getBloco() const { return bloco_; }
const string& Apartamento::getNumero() const { return numero_; }
int Apartamento::getAndar() const { return andar_; }

void Apartamento::setId(int id) { id_ = id; }

void Apartamento::setBloco(const string& bloco) {
    if (bloco.empty()) {
        throw ErroValidacao("O bloco não pode ser vazio.");
    }
    bloco_ = bloco;
}

void Apartamento::setNumero(const string& numero) {
    if (numero.empty()) {
        throw ErroValidacao("O número do apartamento não pode ser vazio.");
    }
    numero_ = numero;
}

void Apartamento::setAndar(int andar) {
    if (andar < 0) {
        throw ErroValidacao("O andar não pode ser negativo.");
    }
    andar_ = andar;
}

void Apartamento::adicionarMorador(Morador* morador) {// guarda o ponteiro na lista, sem repetir e sem aceitar nullptr
    if (morador == nullptr) {
        throw ErroValidacao("Morador inválido.");
    }
    for (Morador* m : moradores_) {
        if (m == morador) {
            return;  // ja ta na lista
        }
    }
    moradores_.push_back(morador);
}

// procura o morador pelo id e tira da lista (o objeto NAO e apagado)
bool Apartamento::removerMorador(int moradorId) {
    for (auto it = moradores_.begin(); it != moradores_.end(); ++it) {
        if ((*it)->id() == moradorId)  {
            moradores_.erase(it);
            return true;  // iterador nao vale mais depois do erase
        }
    }
    return false;
}

const vector<Morador*>& Apartamento::getMoradores() const { return moradores_; }

int Apartamento::quantidadeMoradores() const {
    return static_cast<int>(moradores_.size());
}

bool Apartamento::temMoradores() const { return !moradores_.empty(); }

string Apartamento::descricao() const {
    return "Bloco " + bloco_ + ", apto " + numero_ +
           " (andar " + to_string(andar_) + ")";
}