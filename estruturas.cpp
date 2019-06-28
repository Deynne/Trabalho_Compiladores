/**
 * Neste arquivo serão implementadas as estruturas que serão
 * utilizadas como base para o automato do interpretador léxico.
 * 
 * Classes implementadas aqui:
 *    Estados
 */
#include <string>
#include "estruturas.hpp"

static bool erro_comentario_nao_fechado = false,
     erro_caracter_nao_reconhecido = false;

tipo_de_estado Estado::get_estado() {
    return Estado::estado;
}

std::function<Estado*(char,std::shared_ptr<std::vector<Estado*>>)> Estado::get_transicao() {
    return Estado::transicao
}

void Estado::set_estado(tipo_de_estado estado) {
    Estado::estado = estado;
}

void Estado::set_transicao(std::function<Estado*(char,std::shared_ptr<std::vector<Estado*>>)> funcao_de_transicao) {
    Estado::transicao = funcao_de_transicao;
}

Estado* avaliador_de_transicao(Estado * estado_atual,char entrada, std::shared_ptr<std::vector<Estado*>> historico_de_estados,int linha, std::string ultima_palavra) {
    // Passa para o histórico de estados o estado atual, uma vez que este vai ser analisado
    historico_de_estados->push_back(estado_atual);
    // Recupera a função de  transição do estado
    std::function<Estado*(char,std::shared_ptr<std::vector<Estado*>>)> funcao_de_transicao = estado_atual->get_transicao();
    // Executa a função de transição com a entrada, o que retornará um novo
    Estado * novo_estado = funcao_de_transicao(entrada,historico_de_estados);
    
    // Se o estado não mudar
    if(historico_de_estados->end == novo_estado) {
        // remove o estado da pilha
        historico_de_estados->pop_back();
        // Retorna o próprio estado
        return estado_atual;
    }

    // Se o estado retornado for nulo a entrada não é aceita neste estado. Neste caso dever-se-a
    // verificar se o estado atual é de aceitação. Se sim, o automato terminou "OK"
    // Caso não seja, deve-se registrar um erro Buscar um estado de aceitação dentro do histórico
    if(novo_estado == NULL) {
        // Caso onde a rejeição ocorre em um estado final
        if(estado_atual->get_estado() == aceitacao) {
            // Como a palavra terminou de ser lida não é necessário mais manter o histórico da
            // palavra
            while(!historico_de_estados->empty) {
                // Apaga o objeto
                delete historico_de_estados->end;
                // Remove referência do histórico
                historico_de_estados->pop_back();

                // Realiza a escrita no arquivo
                registra_classificacao(ultima_palavra,estado_atual->get_classificacao,linha);
            }
        }
        // Na situação de rejeição fora de um estado de satisfação
        else {
            while(!historico_de_estados->empty && (historico_de_estados->back)->get_estado() != aceitacao) {
                // Apaga o objeto
                delete historico_de_estados->end;
                // Remove referência do histórico
                historico_de_estados->pop_back();
            }
                erro_caracter_nao_reconhecido = true;
                // TODO throw caracter_nao_reconhecido_excessao("Erro de Sintaxe: Sequência de caracters não faz parte da linguagem.", linha);
            
        }
    }
    
    
    return novo_estado;
}

void lista_de_palavras_chave(char * entrada, int linha) {
    std::string s = entrada;
}

void registra_classificacao(std::string ultima_palavra, std::string classificacao,int linha) {

}