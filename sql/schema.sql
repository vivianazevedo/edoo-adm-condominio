-- I01: estrutura do banco SQLite do MVP.
-- Ativar esta configuracao em toda conexao, inclusive na futura classe Database.
PRAGMA foreign_keys = ON;

CREATE TABLE IF NOT EXISTS apartamento (
    id INTEGER PRIMARY KEY,
    bloco TEXT NOT NULL CHECK (length(trim(bloco)) > 0),
    numero TEXT NOT NULL CHECK (length(trim(numero)) > 0),
    andar INTEGER NOT NULL,
    UNIQUE (bloco, numero)
);

-- Visitantes ficam somente nesta tabela, com tipo = 'visitante'.
CREATE TABLE IF NOT EXISTS pessoa (
    id INTEGER PRIMARY KEY,
    nome TEXT NOT NULL CHECK (length(trim(nome)) > 0),
    cpf TEXT NOT NULL UNIQUE
        CHECK (length(cpf) = 11 AND cpf NOT GLOB '*[^0-9]*'),
    telefone TEXT NOT NULL CHECK (length(trim(telefone)) > 0),
    tipo TEXT NOT NULL CHECK (tipo IN ('morador', 'visitante', 'funcionario'))
);

-- pessoa_id identifica o morador e tambem referencia seu cadastro em pessoa.
CREATE TABLE IF NOT EXISTS morador (
    pessoa_id INTEGER PRIMARY KEY,
    apartamento_id INTEGER NOT NULL,
    tipo_ocupacao TEXT NOT NULL CHECK (length(trim(tipo_ocupacao)) > 0),
    data_entrada TEXT NOT NULL
        CHECK (data_entrada GLOB '[0-9][0-9][0-9][0-9]-[0-9][0-9]-[0-9][0-9]'),
    FOREIGN KEY (pessoa_id) REFERENCES pessoa(id) ON DELETE RESTRICT,
    FOREIGN KEY (apartamento_id) REFERENCES apartamento(id) ON DELETE RESTRICT
);

CREATE TABLE IF NOT EXISTS funcionario (
    pessoa_id INTEGER PRIMARY KEY,
    cargo TEXT NOT NULL CHECK (length(trim(cargo)) > 0),
    turno TEXT NOT NULL CHECK (length(trim(turno)) > 0),
    data_admissao TEXT NOT NULL
        CHECK (data_admissao GLOB '[0-9][0-9][0-9][0-9]-[0-9][0-9]-[0-9][0-9]'),
    FOREIGN KEY (pessoa_id) REFERENCES pessoa(id) ON DELETE RESTRICT
);

CREATE TABLE IF NOT EXISTS area_comum (
    id INTEGER PRIMARY KEY,
    nome TEXT NOT NULL CHECK (length(trim(nome)) > 0),
    tipo TEXT NOT NULL CHECK (tipo IN ('SalaoFestas', 'Piscina', 'Churrasqueira')),
    capacidade INTEGER NOT NULL CHECK (capacidade > 0),
    taxa_base REAL NOT NULL DEFAULT 0 CHECK (taxa_base >= 0),
    hora_abertura TEXT NOT NULL
        CHECK (hora_abertura GLOB '[0-2][0-9]:[0-5][0-9]'),
    hora_fechamento TEXT NOT NULL
        CHECK (hora_fechamento GLOB '[0-2][0-9]:[0-5][0-9]'),
    CHECK (hora_fechamento > hora_abertura)
);

CREATE TABLE IF NOT EXISTS reserva (
    id INTEGER PRIMARY KEY,
    morador_id INTEGER NOT NULL,
    area_id INTEGER NOT NULL,
    data TEXT NOT NULL
        CHECK (data GLOB '[0-9][0-9][0-9][0-9]-[0-9][0-9]-[0-9][0-9]'),
    hora_inicio TEXT NOT NULL
        CHECK (hora_inicio GLOB '[0-2][0-9]:[0-5][0-9]'),
    hora_fim TEXT NOT NULL
        CHECK (hora_fim GLOB '[0-2][0-9]:[0-5][0-9]'),
    num_convidados INTEGER NOT NULL DEFAULT 0 CHECK (num_convidados >= 0),
    status TEXT NOT NULL DEFAULT 'ativa' CHECK (status IN ('ativa', 'cancelada')),
    valor REAL NOT NULL DEFAULT 0 CHECK (valor >= 0),
    CHECK (hora_fim > hora_inicio),
    FOREIGN KEY (morador_id) REFERENCES morador(pessoa_id) ON DELETE RESTRICT,
    FOREIGN KEY (area_id) REFERENCES area_comum(id) ON DELETE RESTRICT
);

-- Uma pessoa visitante pode ter varias visitas; cada linha guarda um evento.
CREATE TABLE IF NOT EXISTS visita (
    id INTEGER PRIMARY KEY,
    visitante_id INTEGER NOT NULL,
    apartamento_id INTEGER NOT NULL,
    registrado_por INTEGER NOT NULL,
    entrada TEXT NOT NULL,
    saida TEXT,
    CHECK (saida IS NULL OR saida >= entrada),
    FOREIGN KEY (visitante_id) REFERENCES pessoa(id) ON DELETE RESTRICT,
    FOREIGN KEY (apartamento_id) REFERENCES apartamento(id) ON DELETE RESTRICT,
    FOREIGN KEY (registrado_por) REFERENCES funcionario(pessoa_id) ON DELETE RESTRICT
);
