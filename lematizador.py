import spacy
import re 
import pdfquery

nlp = spacy.load("es_core_news_sm")


def eliminar_conectoresydoc(texto_bruto):
    doc = ["DOCUMENT1", "DOCUMENT2", "DOCUMENT3", "DOCUMENT4", "DOCUMENT5",
        "DOCUMENT6", "DOCUMENT7", "DOCUMENT8", "DOCUMENT9", "DOCUMENT10",
        "DOCUMENT11", "DOCUMENT12", "DOCUMENT13", "DOCUMENT14", "DOCUMENT15",
        "DOCUMENT16", "DOCUMENT17", "DOCUMENT18", "DOCUMENT19", "DOCUMENT20",
        "DOCUMENT21", "DOCUMENT22", "DOCUMENT23", "DOCUMENT24", "DOCUMENT25",
        "DOCUMENT26", "DOCUMENT27",
        "document1", "document2", "document3", "document4", "document5",
        "document6", "document7", "document8", "document9", "document10",
        "document11", "document12", "document13", "document14", "document15",
        "document16", "document17", "document18", "document19", "document20",
        "document21", "document22", "document23", "document24", "document25",
        "document26", "document27"]
    conectores = ["y", "o", "e", "u", "ni", "pero", "mas", "sino", "aunque",
        "porque", "pues", "si", "cuando", "mientras", "que", "como", "donde",
        "aun", "así", "también", "además", "luego", "entonces", "por lo tanto",
        "sin embargo", "no obstante", "así que", "por consiguiente", "de modo que",
        "antes", "después", "hasta", "desde", "durante", "contra", "entre",
        "sobre", "bajo", "tras", "según", "mediante", "excepto", "salvo",
        "incluso", "más allá de", "a pesar de","a","él","de"]
    
    ROMANOS = re.compile(r"^\s*([IVXLCDM]+)[\.\s]*$", re.IGNORECASE)
    
    for palabra in texto_bruto:
        if palabra in doc or palabra in conectores or ROMANOS.match(palabra):
            texto_bruto.remove(palabra)
    
    return texto_bruto


def lematizar_texto(texto):
    doc = nlp(texto)
    lemmas = [token.lemma_ for token in doc if not token.is_punct and not token.is_space]
    return lemmas


def eliminar_duplicados(lista_lemmas):
    """Elimina duplicados manteniendo el orden"""
    palabras_vistas = set()
    palabras_unicas = []
    for palabra in lista_lemmas:
        if palabra not in palabras_vistas:
            palabras_vistas.add(palabra)
            palabras_unicas.append(palabra)
    return palabras_unicas

# Extraer texto del PDF
pdf = pdfquery.PDFQuery("principito.pdf")
pdf.load()
text_elements = pdf.pq('LTTextBoxHorizontal')
text = [t.text for t in text_elements if t.text is not None]

# Guardar texto dividido en documentos
with open("principito.txt", "w", encoding="utf-8") as f:
    contador = 0
    decimal_value = 1
    f.write(f"DOCUMENT{decimal_value}\n")
    
    for line in text:
        if line.strip():
            palabras = line.split()
            for palabra in palabras:
                f.write(palabra + "\n")
                contador += 1
            
            if contador >= 500:
                contador = 0
                decimal_value += 1
                f.write(f"\nDOCUMENT{decimal_value}\n")

print(f"Texto extraído y guardado en principito.txt con {decimal_value} documentos")

# Lematizar el texto
with open("principito.txt", "r", encoding="utf-8") as f:
    contenido = f.read()
    lemmas = lematizar_texto(contenido)
    with open("principito_lemas.txt", "w", encoding="utf-8") as f_out:
        f_out.write("\n".join(lemmas))

print("Lematización completada y guardada en principito_lemas.txt")

# Crear diccionario eliminando duplicados
dic = eliminar_duplicados(lemmas) 
dic2 = eliminar_conectoresydoc(dic) # Usa directamente la lista 'lemmas'
with open("dic.txt", "w", encoding="utf-8") as f_out:
    f_out.write("\n".join(dic2))

print(f"Diccionario creado con {len(dic)} palabras únicas en dic.txt")