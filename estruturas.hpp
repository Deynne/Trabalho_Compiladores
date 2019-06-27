#ifndef _ESTRUTURAS_HPP
#define _ESTRUTURAS_HPP
/**
 * Neste arquivo serão definidas as estruturas que serão utilizadas
 * como base para o automato do interpretador léxico, assim como
 * funções para interpretação destas estruturas
 * 
 * Classes descritas aqui:
 *    Estados
 * 
 * Funções descritas aqui:
 *    avaliador_de_transição
 *    leitor_de_arquivo
 */
#include <vector>
#include<memory>
#include <functional>


enum tipo_de_estado {transicao,aceitacao};

/**
 * classe: Estado
 * Descrição: Classe representando um estado do automato
 */
class Estado {
    public:
        Estado(int estado,void * funcao_transicao); // construtor da classe
        ~Estado(); // Destrutor da classe
        void set_estado(tipo_de_estado); // '
        void set_transicao(std::function<Estado(char,std::shared_ptr<std::vector<Estado>>)>);
        std::function<Estado(char,std::shared_ptr<std::vector<Estado>>)> get_transicao();
        int get_estado();
    private:
        int estado;
        std::function<Estado(char,std::shared_ptr<std::vector<Estado>>)> transicao;
};

/**
 * função: avaliador_de_transicao
 * descrição: A partir de um estado e uma entrada decide para qual
 *            estado o programa será transferido segundo a função
 *            de transição daquele estado e a entrada recebida.
 * 
 * parametros: estado_atual - Estado sendo analisado pela função
 *             
 *             historico_de_estados - Array de estados com uma lista
 *                                    de todos os estados visitados
 *                                    até o momento.
 * 
 * retorno: O próximo Estado a ser analisado.
 */
extern Estado avaliador_de_transicao(Estado estado_atual,char entrada, Estado historico_de_estados []);

/**
 * função: lista_de_palavras_chave
 * descrição: Realiza uma busca na lista de palavras chave da
 *            linguagem de modo a descobrir se a entrada faz parte
 *            da palavra chave. Caso seja uma palavra chave ela
 *            é escrita diretamente no arquivo.
 * 
 * parametros: entrada - A string com a palavra retornada do
 *                       arquivo
 * 
 */
extern void lista_de_palavras_chave(char * entrada);

#endif