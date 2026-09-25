-- I01: dados ficticios para demonstrar as relacoes do MVP.
-- Execute em um banco vazio, depois de schema.sql.
PRAGMA foreign_keys = ON;
BEGIN TRANSACTION;

INSERT INTO apartamento (id, bloco, numero, andar)
VALUES (1, 'A', '101', 1);

INSERT INTO pessoa (id, nome, cpf, telefone, tipo) VALUES
    (1, 'Ana Exemplo', '52998224725', '81999990001', 'morador'),
    (2, 'Bruno Exemplo', '11144477735', '81999990002', 'funcionario'),
    (3, 'Carla Exemplo', '12345678909', '81999990003', 'visitante');

INSERT INTO morador (pessoa_id, apartamento_id, tipo_ocupacao, data_entrada)
VALUES (1, 1, 'proprietario', '2026-09-01');

INSERT INTO funcionario (pessoa_id, cargo, turno, data_admissao)
VALUES (2, 'Porteiro', 'diurno', '2026-09-01');

INSERT INTO area_comum
    (id, nome, tipo, capacidade, taxa_base, hora_abertura, hora_fechamento)
VALUES (1, 'Salao de Festas', 'SalaoFestas', 80, 250.0, '08:00', '23:00');

INSERT INTO reserva
    (id, morador_id, area_id, data, hora_inicio, hora_fim,
     num_convidados, status, valor)
VALUES (1, 1, 1, '2026-10-03', '14:00', '18:00', 10, 'ativa', 280.0);

INSERT INTO visita
    (id, visitante_id, apartamento_id, registrado_por, entrada, saida)
VALUES (1, 3, 1, 2, '2026-09-24 10:00', '2026-09-24 11:00');

COMMIT;
