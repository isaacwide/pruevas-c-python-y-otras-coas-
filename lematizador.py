import spacy
import re 
import pdfquery
nlp = spacy.load("es_core_news_sm")#cargar el modelo de lenguaje español

ROMANOS = re.compile(r"^\s*([IVXLCDM]+)[\.\s]*$", re.IGNORECASE)

def lematizar_texto(texto):
    doc = nlp(texto)
    lemmas = [token.lemma_ for token in doc if not token.is_punct and not token.is_space]
    return lemmas

def romano_decimal(romano):
     roman_map = {'I': 1, 'V': 5, 'X': 10, 'L': 50, 'C': 100, 'D': 500, 'M': 1000}
     valor_relañ = 0 
     prev_value = 0
     for char in reversed(romano):
            value = roman_map.get(char.upper(), 0)
            if value < prev_value:
                  valor_relañ -= value
            else:
                  valor_relañ += value
            prev_value = value
            
     return valor_relañ

def eliminar_duplicados(mis_lemmas):
    palabras = set()
    palabras_unicas = []
    for palabra in mis_lemmas.splitlines():
         
         if palabra not in palabras:
               palabras.add(palabra)
               palabras_unicas.append(palabra)
  
    
    return palabras_unicas
         
# aqui extraemos el texto de un pdf
pdf = pdfquery.PDFQuery("principito.pdf")
pdf.load()
text_elements =  pdf.pq('LTTextBoxHorizontal')
text = [t.text for t in text_elements if t.text is not None]

# Extraer texto de un PDF a un txt archivo boludo 
with open("principito.txt", "w", encoding="utf-8") as f:
    contador = 0  # Empieza en 0
    decimal_value = 1
    f.write(f"DOCUMENT{decimal_value}\n")  # Escribe el primer documento
    
    for line in text:
        if line.strip():  # Si la línea no está vacía
            # como estamos llendo linea por linea nesesitamos usar algp para dividir palabra por palabra
            palabras = line.split()
            for palabra in palabras:
                 f.write(palabra + "\n")  # Escribe cada palabra en una nueva línea
                 contador += 1  # Incrementa el contador por cada palabra
            # Cuando llegues a 500 líneas, cambia de documento
            if contador >= 500:
                contador = 0  # Reinicia el contador
                decimal_value += 1
                f.write(f"\nDOCUMENT{decimal_value}\n")  # Nueva sección
                    
                       
print(f"Texto extraído del PDF y guardado en principito.txt", "con divisiones de DOCUMENT cada 500 palabras."," total de DOCUMENTS:",{decimal_value} )

# alchile lematizamos el texto del archivo txt pero me revolvi xd 
with open("principito.txt", "r", encoding="utf-8") as f:
    contenido = f.read()
    lemmas = lematizar_texto(contenido)
    with open("principito_lemas.txt", "w", encoding="utf-8") as f_out:
        f_out.write("\n".join(lemmas))

print("Lematización completada y guardada en principito_lemas.txt")

# creamos un diccionario de lemas 
with open("principito_lemas.txt", "r", encoding="utf-8") as f:
    contenido = f.read()
    dic = eliminar_duplicados(contenido)
    with open("dic.txt", "w", encoding="utf-8") as f_out:
        f_out.write("\n".join(dic))

print("Diccionario de lemas creado y guardado en dic.txt")

