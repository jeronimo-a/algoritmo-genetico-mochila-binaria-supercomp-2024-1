'''
Script que roda o algoritmo genético múltiplas vezes com diferentes parâmetros,
afim de reconhecer o efeito de cada parâmetro sobre o resultado final.

Os parâmetros que serão manipulados são:
- N_PARENTS: número de soluções que sobrevivem a cada geração
- CROSSOVER_RATE: chance de ocorrer o crossover entre cada par consecutivo de soluções
- MUTATION_RATE: chance de ocorrer uma mutação em um único gene aleatório de cada nova solução 

'''

import os
import random
import subprocess
import numpy as np

# constantes de controle
N_SEEDS = 100       # número de seeds diferentes para testar por parâmetro
N_VARIATIONS = 100  # quantidade de valores para testar na taxa de crossover e de mutação
POP_SIZE = 16       # quantidade de soluções por geração

def main():

    # compila o programa e cria o diretório dos outputs
    os.system("g++ -o program program.cpp")
    os.system("mkdir outputs")
    
    # argumentos dos parâmetros
    seeds_list = list(range(0, N_SEEDS * 1000))                 # lista base das seeds, será embaralhada
    n_parents_list = list(range(0, POP_SIZE + 1))               # varia de 0 até POP_SIZE, não pode ser menor nem maior
    mutation_rate_list = list(np.arange(0, 1, N_VARIATIONS))    # varia de 0 até 1 com N_VARIATIONS elementos
    crossover_rate_list = list(np.arange(0, 1, N_VARIATIONS))   # varia de 0 até 1 com N_VARIATIONS elementos

    # argumentos neutros de cada parâmetro
    n_parents_neutral = n_parents_list[len(n_parents_list) // 2]                # pega o valor do meio
    mutation_rate_neutral = mutation_rate_list[len(mutation_rate_list) // 2]    # pega o valor do meio
    crossover_rate_neutral = crossover_rate_list[len(crossover_rate_list) // 2] # pega o valor do meio

    # embaralha a lista de seeds e reduz o tamanho
    random.shuffle(seeds_list)
    seeds_list = seeds_list[:N_SEEDS]


if __name__  == "__main__":
    main()