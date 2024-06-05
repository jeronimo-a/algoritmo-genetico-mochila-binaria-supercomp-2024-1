#include <iostream>
#include <random>
#include <vector>

// constantes de controle da aleatoriedade
const int SEED = 525072;        // seed do gerador de números pseudoaleatórios

// constantes de controle dos itens
const int N_ITEMS = 10;         // quantidade de itens
const int MIN_VALUE = 1;        // valor mínimo de cada item
const int MAX_VALUE = 40;       // valor máximo de cada item
const int MIN_WEIGHT = 1;       // peso mínimo de cada item
const int MAX_WEIGHT = 10;      // peso máximo de cada item

// constantes de controle da mochila
const int BAG_CAPACITY = 40;    // capacidade da mochila

// constantes de controle da população
const int POP_SIZE = 16;        // soluções por população

// constantes de controle da seleção
const int N_PARENTS = 8;        // quantas soluções são passadas adiante por iteração

// funções
int verify_constants();                                                                                 // verifica a validade das constantes
int calculate_fitness(std::vector<int> values, std::vector<int> weights, std::vector<int> solution);    // calcula o fitness de uma solução
std::vector<int> make_selection(std::vector<int> fitnesses);                                            // seleciona as N_PARENTS soluções com maior fitness

int main() {

    // verifica se as constantes são válidas
    if (verify_constants()) { return 1; }

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

int verify_constants() {
    // verifica as constantes e retorna 1 se algum dos valores for inválido
    // também gera mensagens de erro explicativas

    // bool, 1 se encontrou alguma constante inválida, 0 se não
    int is_any_invalid = 0;

    // gera erro no caso de N_PARENTS maior que POP_SIZE
    if (N_PARENTS > POP_SIZE) {
        std::cerr << "O número de pais não pode ser maior que o da população." << std::endl;
        is_any_invalid = 1;
    }

    return is_any_invalid;
}

int calculate_fitness(std::vector<int> values, std::vector<int> weights, std::vector<int> solution) {
    // calcula o valor do fitness de uma solução
    // se o peso total da solução for maior que a capacidade da mochila, retorn 0
    // se o peso total da solução for menor ou igual à capacidade da mochila, retorna a soma dos valores dos itens da solução
    //
    // recebe:
    // - values:    vetor dos valores dos itens
    // - weights:   vetor dos pesos dos itens
    // - solution:  vetor dos genes de uma única solução
    //
    // retorna: o fitness dessa única solução

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

std::vector<int> make_selection(std::vector<int> fitnesses) {
    // seleciona os N_PARENTS soluções com o fitness mais alto
    //
    // recebe
    // - fitnesses: vetor com os valores dos fitness da população atual em ordem
    //
    // retorna: um vetor em ordem com as soluções mais adequadas (0 para inadequada, 1 para adequada)

    // vetor dos resultados e variável de referência
    std::vector<int> results(POP_SIZE, 0);  // vetor dos resultados inicializado com zeros
    int n_selected = 0;                     // quantidade de parents selecionados

    // loop de seleção
    while (n_selected < N_PARENTS) {    // até que soluções o suficiente sejam escolhidas

        // variáveis auxiliares para encontrar o índice do fitness mais alto
        int max_fitness = -1;           // valor de fitness mais alto
        int max_fitness_index = -1;     // índice do valor de fitness mais alto

        // loop de seleção da solução com o fitness mais alto ainda não inclusa nos resultados
        for (int i = 0; i < POP_SIZE; i++) {                        // percorre o fitness de todas as soluções
            if (fitnesses[i] > max_fitness && results[i] == 0) {    // se for o maior fitness até então e não tiver sido incluso nos resultados
                max_fitness = fitnesses[i];                         // atualiza qual o valor mais alto encontrado até então
                max_fitness_index = i;                              // atualiza o índice do valor mais alto encontrado até então
            }
        }

        // atualiza os resultados e a variável de contagem 
        results[max_fitness_index] = 1; // adiciona a solução mais adequada do loop aos resultados
        n_selected++;
    }

    return results;
}