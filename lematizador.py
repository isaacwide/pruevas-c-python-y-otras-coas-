import spacy

nlp = spacy.load("es_core_news_sm")#cargar el modelo de lenguaje español

def lematizar_texto(texto):
    doc = nlp(texto)
    lemmas = [token.lemma_ for token in doc if not token.is_punct and not token.is_space]
    return lemmas

texto = "Los gatos corrían por las calles mojadas"
print(lematizar_texto(texto))

import pdfquery
# aqui extraemos el texto de un pdf
pdf = pdfquery.PDFQuery("principito.pdf")
pdf.load()
text_elements =  pdf.pq('LTTextLineHorizontal')
text = [t.text for t in text_elements if t.text is not None]

# Extraer texto de un PDF a un txt archivo boludo 
with open("principito.txt", "w", encoding="utf-8") as f:
    for line in text:
        f.write(line + "\n")

print("Texto extraído del PDF y guardado en principito.txt")

# alchile lematizamos el texto del archivo txt pero me revolvi xd 
with open("principito.txt", "r", encoding="utf-8") as f:
    contenido = f.read()
    lemmas = lematizar_texto(contenido)
    with open("principito_lemas.txt", "w", encoding="utf-8") as f_out:
        f_out.write("\n".join(lemmas))

