'''
Script que roda o algoritmo genético múltiplas vezes com diferentes parâmetros,
afim de reconhecer o efeito de cada parâmetro sobre o resultado final.

Os parâmetros que serão manipulados são:
- N_PARENTS: número de soluções que sobrevivem a cada geração
- CROSSOVER_RATE: chance de ocorrer o crossover entre cada par consecutivo de soluções
- MUTATION_RATE: chance de ocorrer uma mutação em um único gene aleatório de cada nova solução

O script gera gráficos dentro do diretório outputs.

'''

import os
import random
import subprocess
import numpy as np

from datetime import datetime
from matplotlib import pyplot as plt

# constantes de controle
N_SEEDS      = 100    # número de seeds diferentes para testar por parâmetro
N_VARIATIONS = 50       # quantidade de valores para testar na taxa de crossover e de mutação
GENERATIONS  = 30       # número de gerações por execução
POP_SIZE     = 16       # quantidade de soluções por geração

# constantes extra
SOURCE_CODE_LOCATION = "program.cpp"    # localização do código fonte
EXECUTABLE_NAME      = "program"        # nome do executável do algoritmo
N_PARENTS_NAME       = "N_PARENTS"      # nome do parâmetro N_PARENTS
MUTATION_RATE_NAME   = "MUTATION_RATE"  # nome do parâmetro MUTATION_RATE
CROSSOVER_RATE_NAME  = "CROSSOVER_RATE" # nome do parâmetro CROSSOVER_RATE

def main():

    # registra o horário inicial e formata o nome da execução
    start_time = datetime.now()
    start_time_string = start_time.strftime("%Y%m%d%H%M%S")
    execution_name = "%d_%s" % (N_SEEDS, start_time_string)

    # print de verbose
    print("\nINICIANDO SIMULAÇÃO %s" % execution_name)

    # compila o programa
    os.system("g++ -o %s %s" % (EXECUTABLE_NAME, SOURCE_CODE_LOCATION))

    # cria os diretórios de output
    try: os.chdir("outputs"); os.chdir("..")                # verifica se o diretório outputs existe
    except FileNotFoundError: os.system("mkdir outputs")    # se não, o cria
    os.system("mkdir outputs/%s" % execution_name)          # cria o diretório da execução
    
    # argumentos dos parâmetros
    seeds_list = list(range(0, N_SEEDS * 1000))                                     # lista base das seeds, será embaralhada
    n_parents_list = list(range(1, POP_SIZE + 1))                                   # varia de 1 até POP_SIZE, não pode ser menor nem maior
    mutation_rate_list = list(np.linspace(0, 1, N_VARIATIONS))                      # varia de 0 até 1 com N_VARIATIONS elementos
    crossover_rate_list = list(np.linspace(1 / N_VARIATIONS, 1, N_VARIATIONS - 1))  # varia de pouco mais de 0 até 1 com N_VARIATIONS - 1 elementos

    # argumentos neutros de cada parâmetro
    n_parents_neutral = n_parents_list[POP_SIZE // 2]                               # 50% morre 50% sobrevive
    mutation_rate_neutral = mutation_rate_list[len(mutation_rate_list) // 2 - 1]    # pega o valor do meio
    crossover_rate_neutral = crossover_rate_list[len(crossover_rate_list) // 2 - 1] # pega o valor do meio

    # dicionário dos argumentos para deixar o código menos repetitivo
    arguments = {
        N_PARENTS_NAME      : (n_parents_list, [str(GENERATIONS), None, str(POP_SIZE), None, str(crossover_rate_neutral), str(mutation_rate_neutral)], 1, 3, "g"),
        MUTATION_RATE_NAME  : (mutation_rate_list, [str(GENERATIONS), None, str(POP_SIZE), str(n_parents_neutral), str(crossover_rate_neutral), None], 1, 5, "r"),
        CROSSOVER_RATE_NAME : (crossover_rate_list, [str(GENERATIONS), None, str(POP_SIZE), str(n_parents_neutral), None, str(mutation_rate_neutral)], 1, 4, "b")
    }

    # embaralha a lista de seeds e reduz o tamanho
    random.shuffle(seeds_list)
    seeds_list = seeds_list[:N_SEEDS]

    # dicionário dos resultados e print de espaçamento
    results = dict()
    print()

    for key in arguments.keys():

        # print de feedback de execução, inicialização dos resultados do parâmetro e variável de contagem do loop externo
        print("\t" + key + "\n")
        results[key] = list()
        count_out = 0

        # loop de variação da quantidade de soluções sobreviventes por geração
        for argument in arguments[key][0]:

            # verbose para feedback de execução
            print("\t\t" + key + ": %d/%d" % (count_out, len(arguments[key][0])))
            print("\t\t\tSEED: 0/%d" % N_SEEDS)

            # variáveis para o loop de variação da seed
            max_fitnesses = list()  # lista de valores máximos (finais) de fitness por seed
            count_in = 0            # contagem de iterações do loop de seeds para verbose de feedback de execução

            # loop de variação da seed
            for seed in seeds_list:

                # verbose de feedback de execução
                if (count_in + 1) % 1000 == 0:                          # a cada 1000 iterações
                    print("\t\t\tSEED: %d/%d" % (count_in + 1, N_SEEDS))    # da sinal de vida

                # define os argumentos do subprocess (de chamada do algoritmo)
                seed_index = arguments[key][2]                       # pega o índice do argumento da seed
                argument_index = arguments[key][3]                   # pega o índice do argumento em questão
                arguments[key][1][seed_index] = str(seed)            # inclui a seed nos argumentos
                arguments[key][1][argument_index] = str(argument)    # inclui o argumento em questão nos argumentos

                # cria um subprocesso de chamada do algoritmo genético
                subprocess_results = subprocess.run(["./" + EXECUTABLE_NAME] + arguments[key][1], capture_output=True, text=True)

                # extrai a fitness máxima (final) da iteração e incrementa a contagem interna
                max_fitness = int(subprocess_results.stdout.split()[-1])    # extrai a fitness máxima (final) da iteração
                max_fitnesses.append(max_fitness)                           # adiciona a fitness máxima da iteração à lista de fitness máximas da seed
                count_in += 1                                               # incrementa a contagem do loop de seeds

            # print de feedback de execução
            print("\t\t\tSEED: %d/%d" % (N_SEEDS, N_SEEDS))

            # calcula a média das fitness máximas para a seed e armazena no dicionário dos resultados e incrementa a contagem externa
            average_fitness = np.mean(max_fitnesses)    # calcula a média
            results[key].append(average_fitness)        # adiciona ao dicionário de resultados
            count_out += 1                              # incrementa a contage externa (do loop do argumento)

        # verbose para feedback de execução
        print("\t\t" + key + ": %d/%d\n" % (len(arguments[key][0]), len(arguments[key][0])))
        print("\t\t" + key, "finalizado!\n")

        # plota o gráfico do fitness máximo por quantidade de sobreviventes por geração
        plt.plot(arguments[key][0], results[key], c=arguments[key][4])  # plota o gráfico de cada parâmetro vs fitness máximos
        plt.savefig("outputs/%s/%s.png" % (execution_name, key))        # salva em um arquivo com o nome da key no diretório da execução dentro do diretório outputs
        plt.close()                                                     # finaliza a plotagem

    # remove o executável
    os.system("rm program")

    # print de finzalização
    print("SIMULAÇÃO FINALIZADA %s\n" % execution_name)

if __name__  == "__main__":
    main()