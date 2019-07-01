/**
 * Neste arquivo serão implementadas as estruturas que serão
 * utilizadas como base para o automato do interpretador léxico.
 * 
 * Classes implementadas aqui:
 *    Estados
 */
#include <string>
#include <exception>
#include "estruturas.hpp"

bool erro_comentario_nao_fechado = false,
     erro_caracter_nao_reconhecido = false,
     nova_linha = false,
     remove_caracter = false;

std::shared_ptr<std::map<std::string,Estado*>> lista_de_estados(new std::map<std::string,Estado *>());

std::ofstream tabela_lexica;
std::vector<std::string> pk,opr;

void monta_automato() {
    // Aqui serão instanciados todos os estados
    // std::string q = "q0";
    
    // Estado onde a verificação inicia
    (*lista_de_estados)["inicial"] = new Estado();
    (*lista_de_estados)["inicial"]->set_classificacao("inicial");
    // for(int i = 1; i <= 45;i++) {
    //     (*lista_de_estados)[q.replace(1,1,std::to_string(i))] = new Estado();    
    // }

    // Estado de comentario
    (*lista_de_estados)["comentarios"] = new Estado(); 
    (*lista_de_estados)["comentarios"]->set_classificacao("Comentarios");
    // Estado final do = 
    (*lista_de_estados)["operador_relacional#1"] = new Estado();
    (*lista_de_estados)["operador_relacional#1"]->set_estado(aceitacao);
    (*lista_de_estados)["operador_relacional#1"]->set_classificacao("Operador Relacional");

    // Estado final do < 
    (*lista_de_estados)["operador_relacional#2"] = new Estado();
    (*lista_de_estados)["operador_relacional#2"]->set_estado(aceitacao);
    (*lista_de_estados)["operador_relacional#2"]->set_classificacao("Operador Relacional");

    // Estado final do > 
    (*lista_de_estados)["operador_relacional#3"] = new Estado();
     (*lista_de_estados)["operador_relacional#3"]->set_estado(aceitacao);
     (*lista_de_estados)["operador_relacional#3"]->set_classificacao("Operador Relacional");

    // Estado final do + e -
    (*lista_de_estados)["operador_aditivo"] = new Estado(); 
    (*lista_de_estados)["operador_aditivo"]->set_estado(aceitacao);
    (*lista_de_estados)["operador_aditivo"]->set_classificacao("Operador Aditivo");

    // Estado final do * e /
    (*lista_de_estados)["operador_multiplicativo"] = new Estado();
    (*lista_de_estados)["operador_multiplicativo"]->set_estado(aceitacao);
    (*lista_de_estados)["operador_multiplicativo"]->set_classificacao("Operador Multiplicativo");

    // Estado final do = que vem depois do :
    (*lista_de_estados)["comando_de_atribuicao"] = new Estado(); 
    (*lista_de_estados)["comando_de_atribuicao"]->set_estado(aceitacao);
    (*lista_de_estados)["comando_de_atribuicao"]->set_classificacao("Comando de Atribuicao");

     // Estado final dos delimitadores .,;()
    (*lista_de_estados)["delimitador#1"] = new Estado();
    (*lista_de_estados)["delimitador#1"]->set_estado(aceitacao);
    (*lista_de_estados)["delimitador#1"]->set_classificacao("Delimitador");

     // Estado final do delimitador :
    (*lista_de_estados)["delimitador#2"] = new Estado();
    (*lista_de_estados)["delimitador#2"]->set_estado(aceitacao);
    (*lista_de_estados)["delimitador#2"]->set_classificacao("Delimitador");

    // Estado final dos numeros reais que são formado com numeros após o .
    (*lista_de_estados)["numeros_reais"] = new Estado();
    (*lista_de_estados)["numeros_reais"]->set_estado(aceitacao);
    (*lista_de_estados)["numeros_reais"]->set_classificacao("Numero real");

    // Estado final dos numeros inteiros 
    (*lista_de_estados)["numeros_inteiros"] = new Estado();
    (*lista_de_estados)["numeros_inteiros"]->set_estado(aceitacao);
    (*lista_de_estados)["numeros_inteiros"]->set_classificacao("Numero Inteiro");

    // Estado final para qualquer outra coisa que faça parte da linguagem
    // Palavras chave são tratadas como identificador até que se prove o contrário com uma
    // verificação na tabela. O mesmo vale para or (operador aditivo) e and (operador multiplicativo)
    (*lista_de_estados)["identificadores"] = new Estado();
    (*lista_de_estados)["identificadores"]->set_estado(aceitacao);
    (*lista_de_estados)["identificadores"]->set_classificacao("Identificador");

    /*****************************************************************
     * Lista de estas criada, agora serão instanciadas as transições *
     *****************************************************************/

    // Estados que não são finais não precisam ser classificados
    // A transição irá verificar apenas o seu estado para saber se ele é ou não final
    // Cada estado terá uma função de transição a qual também será pré-definida

    // Trasições do estado inicial
    (*lista_de_estados)["inicial"]->set_transicao([](char entrada,std::shared_ptr<std::vector<Estado*>> historico,std::shared_ptr<std::map<std::string,Estado*>> lista_de_estados)-> Estado *{
        // Um estado será retornado de acordo com a entrada inserida
        
        // entradas de "espaço" devem ser ignoradas
        if(entrada == '\n' || entrada == ' ' || entrada == '\t' || entrada == '\r') {
            if(entrada == '\n')
                nova_linha = true;
            remove_caracter = true;
            return (*lista_de_estados)["inicial"];
        }
        // Este caractere indica o início de comentários
        else if(entrada == '{') {
            erro_comentario_nao_fechado = true;
            return (*lista_de_estados)["comentarios"];
        }

        // Estes são operadores aditivos, o "or" é um operador com tratamento especial
        else if(entrada == '+' || entrada == '-')
            return (*lista_de_estados)["operador_aditivo"];

        // Estes são operadores multiplicativos, o "and" é um operador com tratamento especial
        else if(entrada == '*' || entrada == '/')
            return (*lista_de_estados)["operador_multiplicativo"];

        // Estes são delimitadores o ":" é um delimitador com outro possível uso e é tratado separadamente
        else if(entrada == '.' || entrada == ','  || entrada == ';' || entrada == '(' || entrada == ')')
            return (*lista_de_estados)["delimitador#1"];

        // Este é o delimitador ":", ele também pode ser utilizado para atribuição formandon ":="
        else if(entrada == ':')
            return (*lista_de_estados)["delimitador#2"];

        // Qualquer número é um numero inteiro. Se houver um "." ele vira real e pode receber nenhum
        // ou diversos outros números após o "."
        else if(entrada >= '0' && entrada <='9')
            return (*lista_de_estados)["numeros_inteiros"];
        
        // Todos os caracteres de letra serão tratados como variáveis, caso seja uma palavra chave
        // ou "or" ou "and" deve ser comparado na tabela para ser "promovido" ao seu real classificador
        else if(entrada >= 'A' && entrada <='Z' || entrada >= 'a' && entrada <='z' )
            return (*lista_de_estados)["identificadores"];
        
        // O "=" é um operador relacional
        else if(entrada == '=' )
            return (*lista_de_estados)["operador_relacional#1"];
        
        // O "<" é um operador relacional que pode ser combinado com ">" para formar "diferente de" e
        // com "=" para formar "menor ou igual a"
        else if(entrada == '<' )
            return (*lista_de_estados)["operador_relacional#2"];
            
        // O ">" é um operador relacional que pode ser combinado com "="  para formar "maior ou igual a"
        else if(entrada == '>' )
            return (*lista_de_estados)["operador_relacional#3"];
        
        // Caso nenhuma destas opções seja válida, é provavel que seja um caracter que não é aceito
        // já que este é o estado inicial. O NULL só indicará que nenhum dos caracteres é aceito
        // O avaliador irá verificar ser existe algum estado de aceitação no histórico
        return NULL;
        
    });

    // Os comentários aceitam qualquer caracter e são encerrados com }
    (*lista_de_estados)["comentarios"]->set_transicao([](char entrada,std::shared_ptr<std::vector<Estado*>> historico,std::shared_ptr<std::map<std::string,Estado*>> lista_de_estados)-> Estado *{
        // Se for o finalizador dos comentários ele volta pra o estado inicial, se não ele ignora o caracter
        if(entrada == '}') {
            erro_comentario_nao_fechado = false;
            
            return (*lista_de_estados)["inicial"];
        }
        else
            return (*lista_de_estados)["comentarios"];
    });

    // O delimitador#2 trata do ":", uma vez que esse pode ser parte de um comando de atribubição
    (*lista_de_estados)["delimitador#2"]->set_transicao([](char entrada,std::shared_ptr<std::vector<Estado*>> historico,std::shared_ptr<std::map<std::string,Estado*>> lista_de_estados)-> Estado *{
        // Se ele estiver recebendo "=" era realmente um comando de atribuição, caso ele receba outra
        // coisa este é um comando invalido e é necessário reiniciar a verificação
        if(entrada == '=')
            return (*lista_de_estados)["comando_de_atribuicao"];
        else
            return NULL;
    });

    // Transições dos números inteiros, eles podem ser transformados em reais com "."
    (*lista_de_estados)["numeros_inteiros"]->set_transicao([](char entrada,std::shared_ptr<std::vector<Estado*>> historico,std::shared_ptr<std::map<std::string,Estado*>> lista_de_estados)-> Estado *{
        if(entrada == '.')
            return (*lista_de_estados)["numeros_reais"];
        else if(entrada >= '0' && entrada <= '9')
            return (*lista_de_estados)["numeros_inteiros"];
        else
            return NULL;
    });

    // Após ser convertido de inteiro para real, ele só pode receber mais numeros
    (*lista_de_estados)["numeros_reais"]->set_transicao([](char entrada,std::shared_ptr<std::vector<Estado*>> historico,std::shared_ptr<std::map<std::string,Estado*>> lista_de_estados)-> Estado *{
        if(entrada >= '0' && entrada <= '9')
            return (*lista_de_estados)["numeros_reais"];
        else
            return NULL;
    });

    // Se for uma variável ele aceita letras, numeros ou "_"
    (*lista_de_estados)["identificadores"]->set_transicao([](char entrada,std::shared_ptr<std::vector<Estado*>> historico,std::shared_ptr<std::map<std::string,Estado*>> lista_de_estados)-> Estado *{
        if((entrada >= 'A' && entrada <='Z') || (entrada >= 'a' && entrada <='z') || entrada == '_' ||
           (entrada >= '0' && entrada <='9'))
            return (*lista_de_estados)["identificadores"];
        else
            return NULL;
    });

    // Para "<" pode haver um operador relacional de 2 caracteres com a concatenação de ">" ou "="
    (*lista_de_estados)["operador_relacional#2"]->set_transicao([](char entrada,std::shared_ptr<std::vector<Estado*>> historico,std::shared_ptr<std::map<std::string,Estado*>> lista_de_estados)-> Estado *{
        if(entrada == '=' || entrada == '>')
            return (*lista_de_estados)["operador_relacional#1"];
        else
            return NULL;
    });

    // Para ">" pode haver um outro operador relacional combinado com "="
    (*lista_de_estados)["operador_relacional#3"]->set_transicao([](char entrada,std::shared_ptr<std::vector<Estado*>> historico,std::shared_ptr<std::map<std::string,Estado*>> lista_de_estados)-> Estado *{
        if(entrada == '=')
            return (*lista_de_estados)["operador_relacional#1"];
    });
}


Estado::Estado() {
    Estado::classificacao = "";
    Estado::estado = tipo_de_estado::transicao;
    Estado::transicao = [](char entrada,std::shared_ptr<std::vector<Estado*>> historico,std::shared_ptr<std::map<std::string,Estado*>> lista_de_estados)->Estado*{ return NULL;};

}

Estado::Estado(tipo_de_estado estado,std::string classificacao, std::function<Estado*(char,std::shared_ptr<std::vector<Estado*>>,std::shared_ptr<std::map<std::string,Estado*>>)> funcao_transicao) {
    Estado::estado = estado;
    Estado::classificacao = classificacao;
    Estado::transicao = funcao_transicao;
}

tipo_de_estado Estado::get_estado() {
    return Estado::estado;
}

std::function<Estado*(char,std::shared_ptr<std::vector<Estado*>>,std::shared_ptr<std::map<std::string,Estado*>>)> Estado::get_transicao() {
    return Estado::transicao;
}

std::string Estado::get_classificacao() {
    return Estado::classificacao;
}

void Estado::set_classificacao(std::string classificacao) {
    Estado::classificacao = classificacao;
}

void Estado::set_estado(tipo_de_estado estado) {
    Estado::estado = estado;
}

void Estado::set_transicao(std::function<Estado*(char,std::shared_ptr<std::vector<Estado*>>,std::shared_ptr<std::map<std::string,Estado*>>)> funcao_de_transicao) {
    Estado::transicao = funcao_de_transicao;
}

Estado* avaliador_de_transicao(Estado * estado_atual,char entrada, std::shared_ptr<std::vector<Estado*>> historico_de_estados,int linha, std::string *ultima_palavra) {
    // Passa para o histórico de estados o estado atual, uma vez que este vai ser analisado
    historico_de_estados->push_back(estado_atual);

    // Adiciona o caracter ao fim de ultima_palavra
    //if(!erro_comentario_nao_fechado)
        ultima_palavra->append(1,entrada);

    // Recupera a função de  transição do estado
    std::function<Estado*(char,std::shared_ptr<std::vector<Estado*>>,std::shared_ptr<std::map<std::string,Estado*>>)> funcao_de_transicao = estado_atual->get_transicao();

    // Executa a função de transição com a entrada, o que retornará um novo estado    
    Estado * novo_estado = funcao_de_transicao(entrada,historico_de_estados,lista_de_estados);
    
    // Se o caracter for um dos ignorados, ou estiver em um estado de comentarios
        // o caracter é removido da string final pois não será considerado
        if(erro_comentario_nao_fechado || entrada == '}')
            ultima_palavra->erase(ultima_palavra->size()-1);
    
    // Se o estado não mudar
    if(historico_de_estados->back() == novo_estado) {
        // remove o estado da pilha
        historico_de_estados->pop_back();

        // if(entrada == '\t' || entrada == '\n' || entrada == ' ' && !erro_comentario_nao_fechado)
        //     ultima_palavra->erase(ultima_palavra->size()-1);

        // Retorna o próprio estado
        return estado_atual;
    }

    // Se o estado retornado for nulo a entrada não é aceita neste estado. Neste caso dever-se-a
    // verificar se o estado atual é de aceitação. Se sim, o automato terminou "OK"
    // Caso não seja, deve-se registrar um erro Buscar o estado de aceitação mais próximo
    //  dentro do histórico. Caso não se encontre nenhum. O caracter não é aceito
    if(novo_estado == NULL) {
        // Caso onde a rejeição ocorre em um estado final
        if(estado_atual->get_estado() == aceitacao) {
            // Como a palavra terminou de ser lida não é necessário mais manter o histórico da
            // palavra
            while(!historico_de_estados->empty()) {
                // Apaga o objeto
                //delete historico_de_estados->back();
                // Remove referência do histórico
                historico_de_estados->pop_back();
            }

            //  Como houve um caracter não reconhecido, liga-se a flag
            erro_caracter_nao_reconhecido = true;

            // Como o caracter não foi reconhecido ele não faz parte da palavra e deve ser removido
            ultima_palavra->erase(ultima_palavra->size()-1);

            // Realiza a escrita no arquivo
            registra_classificacao(*ultima_palavra,estado_atual->get_classificacao(),linha);

            // Retorna NULL para informar que a leitura da palavra finalizou
            return NULL;
        }

        // Na situação de rejeição fora de um estado de satisfação
        else {
            while(!historico_de_estados->empty() && (historico_de_estados->back())->get_estado() != aceitacao) {
                // Apaga o objeto
                // delete historico_de_estados->back();

                // Remove o caracter da ultima palavra pois ele não faz parte dela
                if(!ultima_palavra->empty())
                    ultima_palavra->erase(ultima_palavra->size()-1);
                
                // Remove referência do histórico
                historico_de_estados->pop_back();
            }
            
            // Se a lista foi esvaziada sem se encontrar um estado final, este caracter não pertence
            // a linguagem
            if(historico_de_estados->empty()) {
                std::cout << "O caracter \'" << entrada << "\' em linha: " << linha << " é invalido" << std:: endl;
                std::exit(1);
            }
                // TODO throw caracter_nao_reconhecido_excessao("Erro de Sintaxe: Sequência de caracters não faz parte da linguagem.", linha);
            // Caso contrário, um estado final foi encontrado há, dentro dessa string uma palavra válida
            else {
                // Registra a palavra
                registra_classificacao(*ultima_palavra,(historico_de_estados->back())->get_classificacao(),linha);

                // Marca que houve ao menos um caracter que não foi lido corretamente
                erro_caracter_nao_reconhecido = true;
                
                // Como houve uma escrita retorna NULL para avisar o sucesso
                return NULL;
            }

        }
    }


    // Caso não tenha retornado NULL, retorna o novo estado
    return novo_estado;
}

std::string lista_de_palavras_chave(std::string entrada , std::string classificacao) {
    for(int i = 0; i < pk.size();i++){
        // std::cout << pk[i] << "#" <<std::endl;
        if(entrada.compare(pk[i]) == 0)
            return "Palavra-Chave";
    }
    for(int i = 0; i < opr.size();i++){
        // std::cout << opr[i] << "#" <<std::endl;
       if(entrada.compare(opr[i]) == 0) {
            if(entrada.compare("or") == 0)
                return "Operador Aditivo";
            if(entrada.compare("and") == 0)
                return "Operador Multiplicativo";
       }
    }
    return classificacao;
}

void registra_classificacao(std::string ultima_palavra, std::string classificacao,int linha) {
    classificacao = lista_de_palavras_chave(ultima_palavra,classificacao);
    
    tabela_lexica << ultima_palavra << "#" << classificacao << "#" << std::to_string(linha) << std::endl;
    // std::cout << std::endl;
}