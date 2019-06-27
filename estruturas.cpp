/**
 * Neste arquivo serão implementadas as estruturas que serão
 * utilizadas como base para o automato do interpretador léxico.
 * 
 * Classes implementadas aqui:
 *    Estados
 */
#include <string>
#include "estruturas.hpp"

int Estado::get_estado() {
    return Estado::estado;
}

std::function<Estado(char,std::shared_ptr<std::vector<Estado>>)> Estado::get_transicao() {
    return Estado::transicao
}

void Estado::set_estado(tipo_de_estado estado) {
    Estado::estado = estado;
}

void Estado::set_transicao(std::function<Estado(char,std::shared_ptr<std::vector<Estado>>)> funcao_de_transicao) {
    Estado::transicao = funcao_de_transicao;
}

Estado avaliador_de_transicao(Estado estado_atual,char entrada, std::shared_ptr<std::vector<Estado>> historico_de_estados) {
    // Passa para o histórico de estados o estado atual, uma vez que este vai ser analisado
    historico_de_estados->push_back(estado_atual);

    std::function<Estado(char,std::shared_ptr<std::vector<Estado>>)> funcao_de_transicao;// = estado_atual.get_transicao(); // Recupera a função de transição do nó
    funcao_de_transicao(entrada,historico_de_estados);
}

void lista_de_palavras_chave(char * entrada) {
    std::string s = entrada;
}