import multiprocessing
import time

def proceso_1():
    """Primer proceso"""
    for i in range(5):
        print(f"Proceso 1 - Iteración {i}")
        time.sleep(1)

def proceso_2():
    """Segundo proceso"""
    for i in range(5):
        print(f"Proceso 2 - Iteración {i}")
        time.sleep(1)

if __name__ == "__main__":
    # Crear los procesos
    p1 = multiprocessing.Process(target=proceso_1)
    p2 = multiprocessing.Process(target=proceso_2)
    
    # Iniciar ambos procesos
    p1.start()
    p2.start()
    
    # Esperar a que terminen
    p1.join()
    p2.join()
    
    print("Ambos procesos han terminado")