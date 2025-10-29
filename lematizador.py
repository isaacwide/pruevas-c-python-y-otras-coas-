import spacy
import re 
import pdfquery

nlp = spacy.load("es_core_news_sm")

ROMANOS = re.compile(r"^\s*([IVXLCDM]+)[\.\s]*$", re.IGNORECASE)

def lematizar_texto(texto):
    doc = nlp(texto)
    lemmas = [token.lemma_ for token in doc if not token.is_punct and not token.is_space]
    return lemmas

def romano_a_decimal(romano):
    roman_map = {'I': 1, 'V': 5, 'X': 10, 'L': 50, 'C': 100, 'D': 500, 'M': 1000}
    valor_final = 0 
    prev_value = 0
    for char in reversed(romano):
        value = roman_map.get(char.upper(), 0)
        if value < prev_value:
            valor_final -= value
        else:
            valor_final += value
        prev_value = value
    return valor_final

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
dic = eliminar_duplicados(lemmas)  # Usa directamente la lista 'lemmas'
with open("dic.txt", "w", encoding="utf-8") as f_out:
    f_out.write("\n".join(dic))

print(f"Diccionario creado con {len(dic)} palabras únicas en dic.txt")