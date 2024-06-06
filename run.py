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

from datetime import datetime
from matplotlib import pyplot as plt

# constantes de controle
N_SEEDS = 10000             # número de seeds diferentes para testar por parâmetro
N_VARIATIONS = 100          # quantidade de valores para testar na taxa de crossover e de mutação
GENERATIONS = 30            # número de gerações por execução
POP_SIZE = 8                # quantidade de soluções por geração
EXECUTABLE_NAME = "program" # nome do executável do algoritmo

def main():

    # registra o horário inicial
    start_time = datetime.now()
    start_time_string = start_time.strftime("%Y%m%d%H%M%S")

    # compila o programa
    os.system("g++ -o program program.cpp")
    
    # argumentos dos parâmetros
    seeds_list = list(range(0, N_SEEDS * 1000))                 # lista base das seeds, será embaralhada
    n_parents_list = list(range(1, POP_SIZE + 1))               # varia de 1 até POP_SIZE, não pode ser menor nem maior
    mutation_rate_list = list(np.linspace(0, 1, N_VARIATIONS))  # varia de 0 até 1 com N_VARIATIONS elementos
    crossover_rate_list = list(np.linspace(0, 1, N_VARIATIONS)) # varia de 0 até 1 com N_VARIATIONS elementos

    # argumentos neutros de cada parâmetro
    n_parents_neutral = n_parents_list[POP_SIZE // 2]                               # 50% morre 50% sobrevive
    mutation_rate_neutral = mutation_rate_list[len(mutation_rate_list) // 2 - 1]    # pega o valor do meio
    crossover_rate_neutral = crossover_rate_list[len(crossover_rate_list) // 2 - 1] # pega o valor do meio

    # dicionário dos argumentos para deixar o código menos repetitivo
    arguments = {
        "N_PARENTS": n_parents_list
    }

    # dicionário dos argumentos de chamada do script formatados
    subprocess_arguments = {
        "N_PARENTS": ([str(GENERATIONS), None, str(POP_SIZE), None, str(crossover_rate_neutral), str(mutation_rate_neutral)], 1, 3)
    }

    # embaralha a lista de seeds e reduz o tamanho
    random.shuffle(seeds_list)
    seeds_list = seeds_list[:N_SEEDS]

    # dicionário dos resultados
    results = dict()

    for key in arguments.keys():

        # inicializa o par chave valor do tipo de argumento nos resultados
        results[key] = list()

        # loop de variação da quantidade de soluções sobreviventes por geração
        for argument in arguments[key]:

            # verbose para feedback de execução
            print(key + ": %d/%d" % (argument, POP_SIZE))

            # variáveis para o loop de variação da seed
            max_fitnesses = list()  # lista de valores máximos (finais) de fitness por seed
            count = 0               # contagem de iterações para verbose de feedback de execução

            # loop de variação da seed
            for seed in seeds_list:

                # verbose de feedback de execução
                if (count + 1) % 1000 == 0:                                # a cada 1000 iterações
                    print("\tSEED: %d/%d" % (count + 1, len(seeds_list)))   # da sinal de vida

                # define os argumentos do subprocess (de chamada do algoritmo)
                seed_index = subprocess_arguments[key][1]                       # pega o índice do argumento da seed
                argument_index = subprocess_arguments[key][2]                   # pega o índice do argumento em questão
                subprocess_arguments[key][0][seed_index] = str(seed)            # inclui a seed nos argumentos
                subprocess_arguments[key][0][argument_index] = str(argument)    # inclui o argumento em questão nos argumentos

                # cria um subprocesso de chamada do algoritmo genético
                subprocess_results = subprocess.run(["./" + EXECUTABLE_NAME] + subprocess_arguments[key][0], capture_output=True, text=True)

                # extrai a fitness máxima (final) da iteração e incrementa a contagem
                max_fitness = int(subprocess_results.stdout.split()[-1])    # extrai a fitness máxima (final) da iteração
                max_fitnesses.append(max_fitness)                           # adiciona a fitness máxima da iteração à lista de fitness máximas da seed
                count += 1                                                  # incrementa a contagem

            # calcula a média das fitness máximas para a seed e armazena no dicionário dos resultados
            average_fitness = np.mean(max_fitnesses)        # calcula a média
            results[key].append(average_fitness)            # adiciona ao dicionário de resultados

        # plota o gráfico do fitness máximo por quantidade de sobreviventes por geração
        plt.plot(n_parents_list, results[key])                                      # plota o gráfico de cada parâmetro vs fitness máximos
        while True:                                                                 # salva o gráfico nos outputs
            try: plt.savefig("outputs/%s_%s.png" % (key, start_time_string)); break # salva em um arquivo com a data de início do processo na pasta outputs
            except FileNotFoundError: os.system("mkdir outputs"); continue          # se a pasta não existir, a cria
        plt.close()                                                                 # finaliza a plotagem

    # remove o executável
    os.system("rm program")

if __name__  == "__main__":
    main()