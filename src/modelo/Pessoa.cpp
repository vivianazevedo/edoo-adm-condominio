#include "modelo/Pessoa.h"
#include "infra/ErroCondominio.h"

#include <cctype>

using namespace std; 


//verificacao cpf 
string Pessoa :: normalizarCpf (const string& cpf) { //tira tudo que nao for digito

    string digitos; 
    for (char c : cpf){

        if (isdigit(static_cast <unsigned char> (c))){

            digitos += c;
        }
}

    return digitos; 
}


// VERIFICACAO CPF 

bool Pessoa::cpfValido(const string& cpf) {
    string digitos = normalizarCpf(cpf);

    if (digitos.size() != 11) { // checa se tem 11 digitos
        return false;
    }

bool todosIguais = true; // se tudo for igual recusa
    for (char c : digitos) {
        if (c != digitos[0]) {
            todosIguais = false;
        }
    }
    if (todosIguais) {
        return false;
    }  

//regra de chegagem dos digitos verificadores 10 e 11 

int soma = 0;
    for (int i = 0; i < 9; i++) {
        soma += (digitos[i] - '0') * (10 - i);
    }
    int dv1 = (soma * 10) % 11;
    if (dv1 == 10) {
        dv1 = 0;
    }
    if (dv1 != digitos[9] - '0') {
        return false;
    }

soma = 0;
    for (int i = 0; i < 10; i++) {
        soma += (digitos[i] - '0') * (11 - i);
    }
    int dv2 = (soma * 10) % 11;
    if (dv2 == 10) {
        dv2 = 0;
    }
    return dv2 == digitos[10] - '0';
}

string Pessoa::textoObrigatorio(const string& valor, const string& campo) {
    // tira os espacos das pontas e recusa texto que fica vazio (so espacos conta como vazio)
    const char* espacos = " \t\r\n";
    size_t inicio = valor.find_first_not_of(espacos);
    if (inicio == string::npos) {
        throw ErroValidacao(campo + " e obrigatorio.");
    }
    size_t fim = valor.find_last_not_of(espacos);
    return valor.substr(inicio, fim - inicio + 1);
}

// descrever os 4 sets 

void Pessoa::setId(int id) {
    if (id < 0) {
        throw ErroValidacao("O id da pessoa nao pode ser negativo.");
    }
    id_ = id;
}

void Pessoa::setNome(const string& nome) {
    nome_ = textoObrigatorio(nome, "Nome");
}

void Pessoa::setCpf(const string& cpf) {
    if (!cpfValido(cpf)) {
        throw ErroValidacao("CPF invalido: " + cpf);
    }
    cpf_ = normalizarCpf(cpf);
}

void Pessoa::setTelefone(const string& telefone) {
    telefone_ = textoObrigatorio(telefone, "Telefone");
}

//definicao do construtor 

Pessoa::Pessoa(string nome, string cpf, string telefone, int id) {
    setId(id);
    setNome(nome);
    setCpf(cpf);
    setTelefone(telefone);
}

string Pessoa::dataIsoValida(const string& data, const string& campo) {
    // formato AAAA-MM-DD: tamanho 10, hifens nas posicoes 4 e 7 e o resto so digitos
    bool formatoOk = data.size() == 10 && data[4] == '-' && data[7] == '-';
    for (size_t i = 0; formatoOk && i < data.size(); ++i) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(static_cast<unsigned char>(data[i]))) formatoOk = false;
    }
    if (!formatoOk) {
        throw ErroValidacao(campo + " invalida (use AAAA-MM-DD): " + data);
    }
    // confere se a data existe de verdade (mes 1 a 12, dia dentro do mes, ano bissexto)
    int ano = stoi(data.substr(0, 4));
    int mes = stoi(data.substr(5, 2));
    int dia = stoi(data.substr(8, 2));
    const int diasMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool bissexto = ano % 400 == 0 || (ano % 4 == 0 && ano % 100 != 0);
    if (ano == 0 || mes < 1 || mes > 12) {
        throw ErroValidacao(campo + " invalida (data inexistente): " + data);
    }
    int limite = diasMes[mes - 1] + ((mes == 2 && bissexto) ? 1 : 0);
    if (dia < 1 || dia > limite) {
        throw ErroValidacao(campo + " invalida (data inexistente): " + data);
    }
    return data;
}
