import numpy as np
import matplotlib.pyplot as plt
from math import factorial
from pathlib import Path

def metropolis_hastings_discreto(pi, Q, n_iteraciones, x0=None):
    #pi - distribucion estacionaria
    #Q - matriz de transicion
    #x0 - edo. inicial, alearotio si no se especifica
    
    n_estados = len(pi)
    cadena = np.zeros(n_iteraciones, dtype=int)
    # Estado inicial
    if x0 is None:
        cadena[0] = np.random.randint(0, n_estados)
    else:
        cadena[0] = x0
    
    aceptados = 0
    
    for t in range(1, n_iteraciones):
        # Estado actual
        i = cadena[t-1]
        
        # Generar candidato K usando la matriz Q
        k = np.random.choice(n_estados, p=Q[i])
        
        #se implementa la funcion acepracion α = min(1, (π_k × Q_ki) / (π_i × Q_ik))
        if Q[i, k] > 0:
            alpha = min(1, (pi[k] * Q[k, i]) / (pi[i] * Q[i, k]))
        else:
            alpha = 0
        
        #aceptar o rechazar
        u = np.random.uniform(0, 1)
        if u <= alpha:
            cadena[t] = k
            aceptados += 1
        else:
            cadena[t] = i
    
    tasa_aceptacion = aceptados / (n_iteraciones - 1)
    
    return cadena, tasa_aceptacion
    #cadena - estados generados
    #tasa_aceptados - proporcion propuestas aceptadas 


def crear_matriz_propuesta_simetrica(n_estados):
    Q = np.zeros((n_estados, n_estados))
    
    for i in range(n_estados):
        if i == 0:
            Q[i, i] = 0.5
            Q[i, i+1] = 0.5
        elif i == n_estados - 1:
            Q[i, i-1] = 0.5
            Q[i, i] = 0.5
        else:
            Q[i, i-1] = 0.5
            Q[i, i+1] = 0.5
    #print(Q)
    
    return Q #cadena de markov con matriz de transicion
    

def visualizar_resultados(cadena, pi, burn_in=1000):
    fig, axes = plt.subplots(2, 2, figsize=(14, 10))
    
    # Traza de la cadena
    axes[0, 0].plot(cadena, linewidth=0.5)
    axes[0, 0].set_xlabel('Iteración')
    axes[0, 0].set_ylabel('Estado')
    axes[0, 0].set_title('Traza de la Cadena de Markov')
    axes[0, 0].grid(True, alpha=0.3)
    
    # Histograma después del burn-in
    estados_unicos = np.arange(len(pi))
    frecuencias = np.bincount(cadena[burn_in:], minlength=len(pi))
    frecuencias = frecuencias / frecuencias.sum()
    
    x = np.arange(len(pi))
    width = 0.35
    axes[0, 1].bar(x - width/2, frecuencias, width, label='Empírica', alpha=0.7)
    axes[0, 1].bar(x + width/2, pi, width, label='Objetivo', alpha=0.7)
    axes[0, 1].set_xlabel('Estado')
    axes[0, 1].set_ylabel('Probabilidad')
    axes[0, 1].set_title(f'Distribución (después de {burn_in} iteraciones)')
    axes[0, 1].legend()
    axes[0, 1].grid(True, alpha=0.3)
    
    # Convergencia de la media
    medias_acumuladas = np.cumsum(cadena) / np.arange(1, len(cadena) + 1)
    media_teorica = np.sum(estados_unicos * pi)
    
    axes[1, 0].plot(medias_acumuladas)
    axes[1, 0].axhline(y=media_teorica, color='r', linestyle='--', 
                       label=f'Media teórica = {media_teorica:.3f}')
    axes[1, 0].set_xlabel('Iteración')
    axes[1, 0].set_ylabel('Media acumulada')
    axes[1, 0].set_title('Convergencia de la Media')
    axes[1, 0].legend()
    axes[1, 0].grid(True, alpha=0.3)
    
    #autocorrelacion
    max_lag = min(100, len(cadena) // 10)
    autocorr = np.correlate(cadena - cadena.mean(), cadena - cadena.mean(), 
                           mode='full')[len(cadena)-1:]
    autocorr = autocorr / autocorr[0]
    
    axes[1, 1].plot(autocorr[:max_lag])
    axes[1, 1].set_xlabel('Lag')
    axes[1, 1].set_ylabel('Autocorrelación')
    axes[1, 1].set_title('Función de Autocorrelación')
    axes[1, 1].grid(True, alpha=0.3)
    
    plt.tight_layout()
    return fig

#¡APROVECHO PARA ANUNCIAR QUE EL SERVIDOR MAS EPICO DE TODA LA UNIVERSIDAD HA LLEGADO!
# NO TE LO PIERDAS EL PROXIMO 06 DE NOVIEMBRE A LAS 6 p.m
# MAS INFORMACION EN: https://buap.pro/

if __name__ == "__main__":
    estados = np.arange(0, 10)
    lambda_param = 3
    pi = np.exp(-lambda_param) * (lambda_param ** estados) / np.array([factorial(k) for k in estados]) #poisson
    pi = pi / pi.sum()  #Normalizar
    
    print("Distribución objetivo:")
    for i, p in enumerate(pi):
        print(f"  Estado {i}: {p:.4f}")
    
    Q = crear_matriz_propuesta_simetrica(len(pi))
    
    #Ejecutar Metropolis-Hastings
    n_iteraciones = 10000000
    cadena, tasa_aceptacion = metropolis_hastings_discreto(pi, Q, n_iteraciones)
    
    print(f"\nNúmero de iteraciones: {n_iteraciones}")
    print(f"Tasa de aceptación: {tasa_aceptacion:.3f}")
    
    # Calcular distribución empírica después del burn-in
    burn_in = 1000
    frecuencias = np.bincount(cadena[burn_in:], minlength=len(pi))
    dist_empirica = frecuencias / frecuencias.sum()
    
    print("\nComparación de distribuciones (después del burn-in):")
    print("Estado | Objetivo | Empírica | Diferencia")
    print("-" * 50)
    for i in range(len(pi)):
        diff = abs(pi[i] - dist_empirica[i])
        print(f"  {i:2d}   |  {pi[i]:.4f}  |  {dist_empirica[i]:.4f}  |  {diff:.4f}")
    
    # visualizar resultados
    ruta = Path("resultados/metropolis_hastings_resultados.png")
    ruta.parent.mkdir(parents=True, exist_ok=True)
    fig = visualizar_resultados(cadena, pi, burn_in)
    plt.savefig(ruta, dpi=300, bbox_inches='tight')
    print(f"\nGráfica guardada en: {ruta.resolve()}")

    
    # Error cuadrático medio
    mse = np.mean((pi - dist_empirica)**2)
    print(f"\nError cuadrático medio: {mse:.6f}")
