#include <iostream>
#include <random>
#include <vector>

// constantes de controle geral
const int GENERATIONS = 100;    // quantidade de vezes para rodar o algoritmo

// constantes de controle dos itens
const int N_ITEMS = 40;         // quantidade de itens
const int MIN_VALUE = 1;        // valor mínimo de cada item
const int MAX_VALUE = 40;       // valor máximo de cada item
const int MIN_WEIGHT = 1;       // peso mínimo de cada item
const int MAX_WEIGHT = 10;      // peso máximo de cada item

// constantes de controle da mochila
const int BAG_CAPACITY = 100;   // capacidade da mochila

// constantes de controle do crossover
const int CROSSOVER_POINT = 20;     // índice do gene até o qual incluir genes do pai 1 (exclusive) e a partir do qual (inclusive) incluir genes do pai 2

// variáveis de entrada (são passadas como argumentos para o programa)
int SEED;               // seed do gerador de números pseudoaleatórios
int POP_SIZE;           // soluções por geração
int N_PARENTS;          // quantas soluções são passadas adiante por iteração
double CROSSOVER_RATE;  // probabilidade de ocorrer crossover para cada par de pais
double MUTATION_RATE;   // probabilidade de ocorrer uma mutação em um filho

// identidades das funções
int parse_arguments(int argc, char* argv[]);
int verify_constants();                                                                                 // verifica a validade das constantes
void print_constants();                                                                                 // imprime todas as constantes
int calculate_fitness(std::vector<int> values, std::vector<int> weights, std::vector<int> solution);    // calcula o fitness de uma solução
std::vector<int> make_selection(std::vector<int> fitnesses);                                            // seleciona as N_PARENTS soluções com maior fitness
std::vector<int> make_crossover(std::vector<int> parent_1, std::vector<int> parent_2);                  // faz invariavelmente o crossover de parent 1 com parent 2
std::vector<int> make_mutation(std::vector<int> solution);                                              // faz invariavelmente uma mutação na solution
std::vector<int> optimize(                                                                              // função de rodar o algoritmo genético
    std::vector<std::vector<int>> initial_population,
    std::vector<int> item_values,
    std::vector<int> item_weights
);

int main(int argc, char* argv[]) {

    // faz o parsing dos argumentos e verifica se a sua combinação com as constantes é válida
    if (parse_arguments(argc, argv)) { return 1; }
    if (verify_constants()) { return 1; }

    // imprime todas as constantes
    print_constants();

    // gerador de números aleatórios com a SEED definida nas constantes
    srand(SEED);

    //== INICIALIZAÇÃO DOS ITENS == === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === ===

    // declaração dos vetores dos itens
    std::vector<int> item_ids(N_ITEMS);     // IDs dos items, será inicializado com números em ordem, só por questão de organização
    std::vector<int> item_values(N_ITEMS);  // valores dos itens, será inicializado com números aleatórios
    std::vector<int> item_weights(N_ITEMS); // pesos dos itens, será inicializado com números aleatórios

    // imprime o header da apresentação dos itens
    std::cout << std::endl << "ITEMS" << std::endl;
    std::cout << "ID\tVALUE\tWEIGHT" << std::endl;

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

    // inicializaçao e apresentação da população inicial
    for (int i = 0; i < POP_SIZE; i++) {    // percorre todas as soluções da população

        // preenche os genes da população inicial
        for (int j = 0; j < N_ITEMS; j++) { // percorre todos os itens da solução
            int gene_value = rand() % 2;    // 50% de chance de 0, 50% de chance de 1
            population[i][j] = gene_value;  // insere o valor obtido na matriz da população
        }
    }

    // chama a função de execução do algoritmo genético
    std::vector<int> results = optimize(population, item_values, item_weights);

    // imprime os resultados
    std::cout << std::endl << "RESULTS" << std::endl;
    for (int i = 0; i < GENERATIONS; i++) {
        std::cout << results[i] << std::endl;
    }

    return 0;
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

std::vector<int> make_crossover(std::vector<int> parent_1, std::vector<int> parent_2) {
    // faz o crossover de dois parents
    // 
    // recebe:
    // - parent_1: vetor dos genes do parent 1
    // - parent_2: vetor dos genes do parent 2
    //
    // retorna: vetor dos genes do filho

    // declara o vetor do filho
    std::vector<int> offspring(N_ITEMS);

    // preenche parte do vetor do filho com os genes do pai 1
    for (int i = 0; i < CROSSOVER_POINT; i++) {
        offspring[i] = parent_1[i];
    }

    // preenche parte do vetor do filho com os genes do pai 2
    for (int i = CROSSOVER_POINT; i < N_ITEMS; i++) {
        offspring[i] = parent_2[i];
    }

    return offspring;
}

std::vector<int> make_mutation(std::vector<int> solution) {
    // faz invariavelmente uma mutação no solution passado como argumento
    // escolhe um gene aleatório da solução para inverter
    //
    // recebe:
    // - solution: vetor dos genes da solução a ser mutada
    //
    // retorna: vetor dos genes da solução mutados

    // declara o vetor da solução com mutação e gera um índice aleatório
    std::vector<int> result(N_ITEMS);
    int random_index = rand() % N_ITEMS;

    // preenche o vetor com os valores de entrada, mas altera o do índice aleatório
    for (int i = 0; i < N_ITEMS; i++) {
        result[i] = solution[i];
        if (i == random_index) {
            result[i] = (result[i] + 1) % 2;
        }
    }

    return result;
}

std::vector<int> optimize(
    std::vector<std::vector<int>> initial_population,
    std::vector<int> values,
    std::vector<int> weights
) {
    // executa o algoritmo genético
    //
    // recebe:
    // - initial_population: matriz da população inicial
    // - values: vetor dos valores dos itens
    // - weights: vetor dos pesos dos itens
    //
    // retorna: vetor das maiores fitness de cada geração
    //
    // etapas:
    // - 0: criação do vetor dos resultados
    // - 1: criação da matriz da última geração
    // - 2: loop de evolução
    //      - 2.1: calculo do fitness e inclusão no vetor dos resultados
    //      - 2.2: seleção
    //      - 2.3: crossover
    //      - 2.4: mutação
    //      - 2.5: cópia do vetor da nova geração para o da última geração

    //== ETAPA 0 === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === optimize 0

    // criação do vetor dos resultados
    std::vector<int> results(GENERATIONS);

    //== ETAPA 1 === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === optimize 1
    
    // copia a matriz das soluçõs inicias para a matriz da última geração
    std::vector<std::vector<int>> latest_generation(POP_SIZE, std::vector<int>(N_ITEMS));
    for (int i = 0; i < POP_SIZE; i++) {                        // para toda solução na população
        for (int j = 0; j < N_ITEMS; j++) {                     // para todo gene na solução
            latest_generation[i][j] = initial_population[i][j]; // copia o gene da solução da população inicial
        }
    }

    //== ETAPA 2 === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === optimize 2
    for (int generation = 0; generation < GENERATIONS; generation++) {
        
        // ETAPA 2.1 === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === optimize 2.1

        // declara o vetor dos valores de fitness das soluções da população atual
        std::vector<int> fitnesses(POP_SIZE);

        // cálculo do fitness e do valor mais alto
        int max_fitness = -1;                                                           // fitness mais alto da população
        for (int i = 0; i < POP_SIZE; i++) {                                            // percorre todas as soluções atuais
            fitnesses[i] = calculate_fitness(values, weights, latest_generation[i]);    // calcula o fitness de cada uma
            if (fitnesses[i] > max_fitness) { max_fitness = fitnesses[i]; }             // se for maior que o máximo até agora, atualiza o valor do máximo
        }

        // adiciona o fitness mais alto ao vetor dos resultados
        results[generation] = max_fitness;

        // ETAPA 2.2 === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === optimize 2.2
        
        // faz a seleção e cria uma nova matriz com apenas os sobreviventes
        std::vector<int> survivors = make_selection(fitnesses);                             // faz a seleção com base nos valores de fitness
        std::vector<std::vector<int>> new_generation(POP_SIZE, std::vector<int>(N_ITEMS));  // declara a matriz da nova geração

        // preenche a matriz nova com os sobreviventes no começo dela
        int survivors_included = 0;                                                 // quantidade de sobreviventes que já foram inclusos na nova matriz
        for (int i = 0; i < POP_SIZE; i++) {                                        // percorre todas as soluções
            if (survivors[i] == 0) { continue; }                                    // se não tiver sobrevivido, pula
            for (int j = 0; j < N_ITEMS; j++) {                                     // percorre todos os genes da solução
                new_generation[survivors_included][j] = latest_generation[i][j];    // copia para a primeira linha disponível da nova matriz
            }
            survivors_included++;       // incrementa a quantidade de sobreviventes que já foram inclusos na nova matriz
        }

        // ETAPA 2.3 === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === optimize 2.3

        // variáveis auxiliares do loop
        int i = 0;                  // variável de contagem, incrementada a cada loop, usada para determinar os índices dos pais
        int offsprings_added = 0;   // quantidade de filhos já adicionados à matriz da nova geração

        // loop de crossover
        while (offsprings_added < POP_SIZE - N_PARENTS) {
            
            // gera um número aleatório que determina se o crossover será realizado com esses pais
            int random_number = rand() % 100;

            // realiza o crossover caso o valor aleatório seja menor que a chance de ocorrer o crossover
            if (random_number < CROSSOVER_RATE * 100) {

                // índice dos pais
                int parent_1_index = i % N_PARENTS;         // índice do pai da esquerda, varia de 0 a N_PARENTS - 1
                int parent_2_index = (i + 1) % N_PARENTS;   // índice do pai da direita, sempre 1 a mais que o da esquerda, ou 0 caso o da esquerda seja N_PARENTS - 1
                
                // adiciona a nova solução à matriz da nova geração e incrementa a contagem de filhos adicionados
                new_generation[N_PARENTS + offsprings_added] = make_crossover(new_generation[parent_1_index], new_generation[parent_2_index]);
                offsprings_added++;
            }
            i++;    // incrementa a contagem independentemente do crossover ter, ou não, ocorrido
        }

        // ETAPA 2.4 === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === optimize 2.4

        // loop de mutação das novas soluções
        for (int i = N_PARENTS; i < POP_SIZE; i++) {                // percorre todas as novas soluções
            int random_number = rand() % 100;                       // gera um número aleatório para decidir se gera ou não mutação
            if (random_number >= MUTATION_RATE * 100) { continue; } // se não for pra gerar, pula essa solução
            new_generation[i] = make_mutation(new_generation[i]);   // se for, gera uma mutação em um gene aleatório
        }

        // ETAPA 2.5 === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === === optimize 2.5

        // copia a nova geração para a última geração
        for (int i = 0; i < POP_SIZE; i++) {
            for (int j = 0; j < N_ITEMS; j++) {
                latest_generation[i][j] = new_generation[i][j];
            }
        }
    }

    return results;
}

void input_error(char *argv0) {
    std::cerr << "Erro: utilização: " << argv0 << " <SEED:int>";
    std::cerr << " <POP_SIZE:int>" << " <N_PARENTS:integer> <CROSSOVER_RATE:double> ";
    std::cerr << "<MUTATION_RATE:double>" << std::endl;
}

int parse_arguments(int argc, char* argv[]) {
    // verifica se todos os argumentos necessários foram fornecidos
    // atualiza as variáveis com os argumentos

    // gera um erro se a quantidade suficiente de argumentos não forem fornecidos
    if (argc != 6) {
        input_error(argv[0]);
        return 1;
    }

    // coleta os argumentos
    try {
        SEED = std::stoi(argv[1]);              // argumento da seed do gerado de números aleatórios
        POP_SIZE = std::stoi(argv[2]);          // argumento da quantidade de soluções por geração
        N_PARENTS = std::stoi(argv[3]);         // argumento do número de parents
        CROSSOVER_RATE = std::stod(argv[4]);    // argumento da taxa de crossover
        MUTATION_RATE = std::stod(argv[5]);     // argumento da taxa de mutação
    }
    
    // captura erros de argumentos de tipo inválido
    catch (const std::invalid_argument& e) {
        input_error(argv[0]);
        return 1;
    }
    
    // captura erros de argumentos grandes demais
    catch (const std::out_of_range& e) {
        std::cerr << "Erro: argumentos grandes demais" << std::endl;
        return 1;
    }

    return 0;
}

int verify_constants() {
    // verifica as constantes e retorna 1 se algum dos valores for inválido
    // também gera mensagens de erro explicativas

    // bool, 1 se encontrou alguma constante inválida, 0 se não
    int is_any_invalid = 0;

    // gera erro no caso de N_PARENTS maior que POP_SIZE
    if (N_PARENTS > POP_SIZE) {
        std::cerr << "Erro: N_PARENTS > POP_SIZE" << std::endl;
        is_any_invalid = 1;
    }

    // gera erro no caso de N_PARENTS menor que zero
    if (N_PARENTS < 0) {
        std::cerr << "Erro: N_PARENTS < 0" << std::endl;
        is_any_invalid = 1;
    }

    // gera erro no caso de CROSSOVER_POINT maior ou igual a N_ITEMS
    if (CROSSOVER_POINT >= N_ITEMS) {
        std::cerr << "Erro: CROSSOVER_POINT >= " << N_ITEMS << " (N_ITEMS)" << std::endl;
        is_any_invalid = 1;
    }

    // gera erro no caso de MUTATION_RATE não estar entre 0 e 1
    if (MUTATION_RATE < 0 || MUTATION_RATE > 1) {
        std::cerr << "Erro: MUTATION_RATE tem que estar entre 0 e 1" << std::endl;
        is_any_invalid = 1;
    }

    // gera erro no caso de CROSSOVER_RATE não estar entre 0 e 1
    if (CROSSOVER_RATE < 0 || CROSSOVER_RATE > 1) {
        std::cerr << "Erro: CROSSOVER_RATE tem que estar entre 0 e 1" << std::endl;
        is_any_invalid = 1;
    }


    return is_any_invalid;
}

void print_constants() {
    // imprime todas as constantes

    std::cout << "SEED\t\t" << SEED << std::endl;
    std::cout << "GENERATIONS\t" << GENERATIONS << std::endl;
    std::cout << "N_ITEMS\t\t" << N_ITEMS << std::endl;
    std::cout << "MIN_VALUE\t" << MIN_VALUE << std::endl;
    std::cout << "MAX_VALUE\t" << MAX_VALUE << std::endl;
    std::cout << "MIN_WEIGHT\t" << MIN_WEIGHT << std::endl;
    std::cout << "MAX_WEIGHT\t" << MAX_WEIGHT << std::endl;
    std::cout << "BAG_CAPACITY\t" << BAG_CAPACITY << std::endl;
    std::cout << "POP_SIZE\t" << POP_SIZE << std::endl;
    std::cout << "N_PARENTS\t" << N_PARENTS << std::endl;
    std::cout << "CROSSOVER_RATE\t" << CROSSOVER_RATE << std::endl;
    std::cout << "CROSSOVER_POINT\t" << CROSSOVER_POINT << std::endl;
    std::cout << "MUTATION_RATE\t" << MUTATION_RATE << std::endl;
}