#include <iostream>
#include <random>
#include <vector>

// constantes de controle
const int N_ITEMS = 10;         // quantidade de itens
const int SEED = 12453;         // seed da aleatorização
const int MIN_VALUE = 1;        // valor mínimo de cada item
const int MAX_VALUE = 50;       // valor máximo de cada item
const int MIN_WEIGHT = 1;       // peso mínimo de cada item
const int MAX_WEIGHT = 10;      // peso máximo de cada item
const int BAG_CAPACITY = 30;    // capacidade da mochila
const int POP_SIZE = 16;        // soluções por população

// funções
int fitness(std::vector<int> values, std::vector<int> weights, std::vector<int> solution);  // calcula o fitness de uma solução

int main() {

    // gerador de números aleatórios com a SEED definida nas constantes
    srand(SEED);

    //== INICIALIZAÇÃO DOS ITENS == === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === ===

    // declaração dos vetores dos itens
    std::vector<int> item_ids(N_ITEMS);     // IDs dos items, será inicializado com números em ordem, só por questão de organização
    std::vector<int> item_values(N_ITEMS);  // valores dos itens, será inicializado com números aleatórios
    std::vector<int> item_weights(N_ITEMS); // pesos dos itens, será inicializado com números aleatórios

    // apresenta a capacidade de mochila e o header da apresentação dos itens
    std::cout << std::endl << "CAPACIDADE DA MOCHILA: " << BAG_CAPACITY << std::endl;
    std::cout << std::endl << "POSSÍVEIS ITENS" << std::endl;
    std::cout << "Item\tValor\tPeso" << std::endl;

    // inicialização e apresentação dos itens
    for (int i = 0; i < N_ITEMS; i++) {                                     // percorre elemento por elemento

        // inicialização dos itens
        item_ids[i] = i;                                                    // o ID vai de 0 a N_ITEMS - 1, só serve para organização
        item_values[i] = (rand() + MIN_VALUE) % MAX_VALUE + MIN_VALUE;      // o valor é aleatório dentro do intervalo [MIN_VALUE, MAX_VALUE]
        item_weights[i] = (rand() + MIN_WEIGHT) % MAX_WEIGHT + MIN_WEIGHT;  // o pes é aleatório dentro do intervalo [MIN_WEIGHT, MAX_WEIGHT]

        // apresentação dos itens
        std::cout << item_ids[i] << "\t";
        std::cout << item_values[i] << "\t";
        std::cout << item_weights[i] << std::endl;
    }

    //== INICIALIAZAÇÃO DA POPULAÇÃO INICIAL == === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === ===

    // declaração da matriz das populações
    std::vector<std::vector<int>> population(POP_SIZE, std::vector<int>(N_ITEMS));

    // apresenta as dimensões de cada população e o header da apresentação da população em si
    std::cout << std::endl << "POPULAÇÃO" << std::endl;
    std::cout << "Tamanho: " << POP_SIZE << std::endl;
    std::cout << "Itens: " << N_ITEMS << std::endl;
    std::cout << std::endl << "POPULAÇÃO INICIAL" << std::endl;
    std::cout << "Solução\tGenes" << std::endl;

    // inicializaçao e apresentação da população inicial
    for (int i = 0; i < POP_SIZE; i++) {    // percorre todas as soluções da população

        // apresenta o número da solução
        std::cout << i << "\t";

        // preenche e apresenta os genes da solução
        for (int j = 0; j < N_ITEMS; j++) { // percorre todos os itens da solução
            int gene_value = rand() % 2;    // 50% de chance de 0, 50% de chance de 1
            population[i][j] = gene_value;  // insere o valor obtido na matriz da população
            std::cout << gene_value << " "; // apresenta o valor do gene
        }

        // quebra de linha
        std::cout << std::endl;
    }
}

int fitness(std::vector<int> values, std::vector<int> weights, std::vector<int> solution) {
    // calcula o valor do fitness de uma solução
    // se o peso total da solução for maior que a capacidade da mochila, retorn 0
    // se o peso total da solução for menor ou igual à capacidade da mochila, retorna a soma dos valores dos itens da solução

    // calcula o peso combinado dos itens
    int solution_weight = 0;                                // acumulador do peso da solução
    for (int i = 0; i < N_ITEMS; i++) {                     // percorre todos os genes da solução
        solution_weight += weights[i] * solution[i];        // soma o peso do item ao acumulador com base no valor do gene
        if (solution_weight > BAG_CAPACITY) { return 0; }   // se o peso ultrapassar o limite da mochila, já retorna 0
    }

    // calcula o valor da mochila
    int solution_value = 0;                                 // acumulador do valor da solução
    for (int i = 0; i < N_ITEMS; i++) {                     // percorre todos os genes da solução
        solution_value += values[i] * solution[i];          // soma o valor do item ao acumulador com base no valor do gene
    }

    return solution_value;
}