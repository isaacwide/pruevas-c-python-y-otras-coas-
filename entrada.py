import ctypes
from numpy import ctypeslib as npct

LIB = "./main.dll"
#llamamos una funcion de la libreria
lib = npct.load_library(LIB, ".")

##primera llamada de una funcion
area = lib.area_circle
area.restype = ctypes.c_double
area.argtypes = [ctypes.c_double]

#segunda llmada de una funcion
concatenacion = lib.conca
concatenacion.restype = ctypes.c_char_p
concatenacion.argtypes = [ctypes.c_char_p, ctypes.c_char_p]



#ingresar radio
radio = float(input("Ingrese el radio del círculo: "))
#calcular area
area_circulo = area(radio)  
print(f"El área del círculo con radio {radio} es: {area_circulo}")



#ingresar cadenas
cadena1 = input("ingrersa tu nombre: ")
cadena2 = input("ingresa tu apellido: ")
#concatenar cadenas
resultado = concatenacion(cadena1.encode('utf-8'),cadena2.encode('utf-8'))
print(resultado)


#tercera funcion siendo llamada 
exist = lib.archivo_existe
exist.restype = ctypes.c_int
exist.argtypes = [ctypes.c_char_p]
if exist(b"principito.txt"):
    print("El archivo existe")
else:
    print("El archivo no existe")

