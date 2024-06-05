#include <iostream>
#include <random>
#include <vector>

// constantes de controle
const int N_ITEMS = 40;         // quantidade de itens
const int SEED = 12453;         // seed da aleatorização
const int MIN_VALUE = 1;        // valor mínimo de cada item
const int MAX_VALUE = 50;       // valor máximo de cada item
const int MIN_WEIGHT = 1;       // peso mínimo de cada item
const int MAX_WEIGHT = 10;      // peso máximo de cada item
const int BAG_CAPACITY = 100;   // capacidade da mochila

int main() {

    // declaração dos vetores
    std::vector<int> item_ids(N_ITEMS);     // IDs dos items, será inicializado com números em ordem, só por questão de organização
    std::vector<int> item_values(N_ITEMS);  // valores dos itens, será inicializado com números aleatórios
    std::vector<int> item_weights(N_ITEMS); // pesos dos itens, será inicializado com números aleatórios

    // gerador de números aleatórios com a SEED definida nas constantes
    srand(SEED);

    // capacidade de mochila e header da apresentação dos valores
    std::cout << std::endl << "CAPACIDADE DA MOCHILA: " << BAG_CAPACITY << std::endl;
    std::cout << std::endl << "POSSÍVEIS ITENS:" << std::endl;
    std::cout << "Item\tValor\tPeso" << std::endl;

    // inicialização e apresentação dos vetores
    for (int i = 0; i < N_ITEMS; i++) {                                     // percorre elemento por elemento

        // inicialização dos vetores
        item_ids[i] = i;                                                    // o ID vai de 0 a N_ITEMS - 1, só serve para organização
        item_values[i] = (rand() + MIN_VALUE) % MAX_VALUE + MIN_VALUE;      // o valor é aleatório dentro do intervalo [MIN_VALUE, MAX_VALUE]
        item_weights[i] = (rand() + MIN_WEIGHT) % MAX_WEIGHT + MIN_WEIGHT;  // o pes é aleatório dentro do intervalo [MIN_WEIGHT, MAX_WEIGHT]

        // apresentação dos vetores
        std::cout << item_ids[i] << "\t";
        std::cout << item_values[i] << "\t";
        std::cout << item_weights[i] << std::endl;
    }
}