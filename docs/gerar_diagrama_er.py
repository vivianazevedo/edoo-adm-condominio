"""Gera docs/diagrama-er.png a partir da representação de sql/schema.sql.

Requer Pillow: python -m pip install Pillow
Atualize tabelas e relações abaixo sempre que o esquema SQL for alterado.
"""

from pathlib import Path

from PIL import Image, ImageDraw, ImageFont


LARGURA, ALTURA = 1800, 1580
FUNDO = "#f6f8fc"
TEXTO = "#192438"
AZUL = "#315ea8"
VERDE = "#16836d"

TABELAS = {
    "apartamento": ((65, 205, 515, 485), [
        "id                   PK", "bloco                TEXT", "numero               TEXT",
        "andar                INTEGER", "UNIQUE (bloco, numero)",
    ]),
    "pessoa": ((675, 205, 1125, 510), [
        "id                   PK", "nome                 TEXT", "cpf                  UQ",
        "telefone             TEXT", "tipo                 TEXT",
    ]),
    "area_comum": ((1285, 205, 1735, 590), [
        "id                   PK", "nome                 TEXT", "tipo                 TEXT",
        "capacidade           INTEGER", "taxa_base            REAL",
        "hora_abertura        TEXT", "hora_fechamento      TEXT",
    ]),
    "funcionario": ((65, 690, 515, 970), [
        "pessoa_id            PK, FK", "cargo                TEXT",
        "turno                TEXT", "data_admissao        TEXT",
    ]),
    "morador": ((675, 690, 1125, 970), [
        "pessoa_id            PK, FK", "apartamento_id       FK",
        "tipo_ocupacao        TEXT", "data_entrada         TEXT",
    ]),
    "visita": ((65, 1175, 515, 1500), [
        "id                   PK", "visitante_id         FK",
        "apartamento_id       FK", "registrado_por       FK",
        "entrada              TEXT", "saida                TEXT?",
    ]),
    "reserva": ((1285, 1090, 1735, 1500), [
        "id                   PK", "morador_id           FK", "area_id              FK",
        "data                 TEXT", "hora_inicio          TEXT", "hora_fim             TEXT",
        "num_convidados       INTEGER", "status               TEXT", "valor                REAL",
    ]),
}

# Uma linha por FK: origem (PK) -> destino (FK). O destino admite N linhas.
RELACOES = [
    ([(900, 510), (900, 690)], "pessoa 1:0..1 morador", (915, 593)),
    ([(775, 510), (775, 625), (290, 625), (290, 690)],
     "pessoa 1:0..1 funcionario", (326, 605)),
    ([(515, 395), (555, 395), (555, 650), (760, 650), (760, 690)],
     "apartamento 1:N morador", (575, 629)),
    ([(65, 370), (35, 370), (35, 1280), (65, 1280)],
     "apartamento 1:N visita", (52, 1025)),
    ([(675, 405), (590, 405), (590, 1120), (475, 1120), (475, 1175)],
     "pessoa 1:N visita", (537, 1045)),
    ([(290, 970), (290, 1175)], "funcionario 1:N visita", (305, 1052)),
    ([(1125, 835), (1220, 835), (1220, 1240), (1285, 1240)],
     "morador 1:N reserva", (1182, 1010)),
    ([(1510, 590), (1510, 1090)], "area_comum 1:N reserva", (1525, 829)),
]


def fonte(tamanho, negrito=False):
    nome = "arialbd.ttf" if negrito else "arial.ttf"
    caminho = Path("C:/Windows/Fonts") / nome
    try:
        return ImageFont.truetype(str(caminho), tamanho)
    except OSError:
        return ImageFont.load_default()


def gerar():
    imagem = Image.new("RGB", (LARGURA, ALTURA), FUNDO)
    d = ImageDraw.Draw(imagem)
    titulo = fonte(44, True)
    subtitulo = fonte(23)
    rotulo = fonte(27, True)
    campo = fonte(20)
    relacao = fonte(17, True)

    d.text((65, 52), "Modelo entidade-relacionamento | MVP condominio", font=titulo, fill=TEXTO)
    d.text((65, 120), "7 tabelas  •  PK = chave primaria  •  FK = chave estrangeira  •  UQ = unico",
           font=subtitulo, fill="#4a5a72")

    # Desenhar conectores atras das tabelas para manter os campos legiveis.
    for pontos, _, _ in RELACOES:
        d.line(pontos, fill=VERDE, width=5, joint="curve")
        x, y = pontos[-1]
        d.ellipse((x - 6, y - 6, x + 6, y + 6), fill=VERDE)

    for nome, (caixa, campos) in TABELAS.items():
        x1, y1, x2, y2 = caixa
        d.rounded_rectangle(caixa, radius=19, fill="white", outline="#cbd5e3", width=3)
        d.rounded_rectangle((x1, y1, x2, y1 + 65), radius=19, fill=AZUL)
        d.rectangle((x1, y1 + 42, x2, y1 + 65), fill=AZUL)
        d.text((x1 + 22, y1 + 16), nome, font=rotulo, fill="white")
        for indice, item in enumerate(campos):
            d.text((x1 + 22, y1 + 87 + indice * 35), item, font=campo, fill=TEXTO)

    for _, texto, (x, y) in RELACOES:
        largura = d.textlength(texto, font=relacao)
        d.rounded_rectangle((x - 6, y - 4, x + largura + 6, y + 24),
                            radius=6, fill=FUNDO)
        d.text((x, y), texto, font=relacao, fill="#086b58")

    d.text((665, 1540), "Fonte: sql/schema.sql  •  Atualizado em 02/10/2026",
           font=fonte(18), fill="#526177")
    destino = Path(__file__).with_name("diagrama-er.png")
    imagem.save(destino)
    print(destino)


if __name__ == "__main__":
    gerar()
