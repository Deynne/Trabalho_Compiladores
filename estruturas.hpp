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
#include <string>
#include <map>
#include <iostream>
#include <functional>
#include <fstream>


enum tipo_de_estado {transicao,aceitacao};

extern bool erro_comentario_nao_fechado,
     erro_caracter_nao_reconhecido,
     nova_linha,
     remove_caracter;

extern std::ofstream tabela_lexica;
extern std::vector<std::string> pk,opr;

/**
 * classe: Estado
 * Descrição: Classe representando um estado do automato
 */
class Estado {
    public:
        Estado(tipo_de_estado estado,std::string classificacao, std::function<Estado*(char,std::shared_ptr<std::vector<Estado*>>,std::shared_ptr<std::map<std::string,Estado*>>)> funcao_transicao); // construtor da classe
        Estado();
        void set_estado(tipo_de_estado);
        void set_classificacao(std::string);
        void set_transicao(std::function<Estado*(char,std::shared_ptr<std::vector<Estado*>>,std::shared_ptr<std::map<std::string,Estado*>>)>);
        std::function<Estado*(char,std::shared_ptr<std::vector<Estado*>>,std::shared_ptr<std::map<std::string,Estado*>>)> get_transicao();
        tipo_de_estado get_estado();
        std::string get_classificacao();
    private:
        tipo_de_estado estado;
        std::string classificacao;
        std::function<Estado*(char,std::shared_ptr<std::vector<Estado*>>,std::shared_ptr<std::map<std::string,Estado*>>)> transicao;
};

extern std::shared_ptr<std::map<std::string,Estado*>> lista_de_estados;

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
extern Estado* avaliador_de_transicao(Estado * estado_atual,char entrada, std::shared_ptr<std::vector<Estado*>> historico_de_estados,int linha, std::string *ultima_palavra);

/**
 * função: lista_de_palavras_chave
 * descrição: Realiza uma busca na lista de palavras chave da
 *            linguagem de modo a descobrir se a entrada faz parte
 *            da palavra chave. Caso seja uma palavra chave ela
 *            é escrita diretamente no arquivo.
 * 
 *            A busca será feita através de busca binária o que
 *            demanda uma lista ordenada.
 * 
 * parametros: entrada - A string com a palavra retornada do
 *                       arquivo
 *  
 *             linha - Linha onde a palavra se encontra
 */
extern std::string lista_de_palavras_chave(std::string entrada, std::string classificacao);


/**
 * funcao: registra_classificacao
 * descrição: Realiza a escrita do arquivo de tabela com a classificação das palavras
 *            do programa. O formato utilizado no armazenamento é:
 *            PALAVRA ; CLASIFICAÇÃO ; LINHA \n
 * 
 * parametros: ultima_palavra - Uma string contendo a ultima palavra lida para armazenamento
 *
 *             classificacao - A classificação da ultima palavra lida. Esta classificação é
 *                             atribuída pelo estado de aceitação onde a palavra finalizou.
 * 
 *             linha - A linha onde a palavra se encontra           
 */
extern void registra_classificacao(std::string ultima_palavra, std::string classificacao,int linha);
extern void monta_automato();
#endif