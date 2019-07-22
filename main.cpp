#include "estruturas.hpp"
#include <ios>



int main(int argc,char* argv[]) {
    std::ifstream palavras_chave,operadores, codigo;

    tabela_lexica.open("tabela_lexica",std::ios_base::binary|std::ios_base::out);
    operadores.open("operadores",std::ios_base::binary|std::ios_base::in);
    std::string s = "";
    char ch;
    while(!operadores.eof()) {
        ch = operadores.get();
        // if (ch == '\0')
        //     break;
        if(ch == '\r')
            continue;
        if(ch == '\n') {
            opr.push_back(s);
            s.clear();
            continue;
        }
        s.append(1,ch); 
    }
    opr.push_back(s.erase(s.size()-1));
    s.clear();
    

    palavras_chave.open("palavras_chave.txt",std::ios_base::binary|std::ios_base::in);
    while(!palavras_chave.eof()) {
        ch = palavras_chave.get();
        // if (ch == '\0')
        //     break;
        if(ch == '\r')
            continue;
        if(ch == '\n') {
            pk.push_back(s);
            s.clear();
            continue;
        }
        s.append(1,ch);
    }
    pk.push_back(s.erase(s.size()-1));
    s.clear();

    codigo.open("codigo",std::ios_base::binary|std::ios_base::in);

    monta_automato();
    std::string ultima_palavra = "";
    int linha = 1;
    std::shared_ptr<std::vector<Estado*>> historico_de_estados(new std::vector<Estado*>());
    Estado * novo = (*lista_de_estados)["inicial"]; // = avaliador_de_transicao((*lista_de_estados)["inicial"],'b',historico_de_estados,linha,&ultima_palavra);
    // // std::cout << novo->get_classificacao() << std::endl;

    std::string str = "",//"program teste;{programa exemplo}\n           \t        var\nvalor1: integer;\nvalor2: real;\nbegin\nvalor1 := 10;\nend.",
                buffer = "";

    while(!codigo.eof()) {
        str.append(1,codigo.get());
    }
    // std::cout << str << std::endl;
    int posicao = 0;
    char c;
    while(posicao < str.size()) {
        c = str.at(posicao);
        buffer.append(1,c);
        if (erro_comentario_nao_fechado){
                buffer.clear();
        }
        
        if(nova_linha) {
            linha++;
            nova_linha = false;
        }
        
    //    std::cout << c;
        novo = avaliador_de_transicao(novo,c,historico_de_estados,linha,&ultima_palavra);

        if(remove_caracter) {
            buffer.erase(buffer.size() - 1);
            ultima_palavra.erase(ultima_palavra.size()-1);
            remove_caracter = false;
        }

        if(novo == NULL) {
            // if (c == '\n') 
            //     linha --;
            if(erro_caracter_nao_reconhecido)
                posicao = posicao - (buffer.size() - ultima_palavra.size());
            // std::cout << buffer << "#" << buffer.length() << "#" << ultima_palavra << "#" <<ultima_palavra.length() << std::endl;
            buffer.clear();
            ultima_palavra.clear();
            novo = (*lista_de_estados)["inicial"];
        }
        posicao++;     
    }
    if(erro_comentario_nao_fechado) {
        std::cout << "O comentario foi aberto mas não foi fechado." << std:: endl;
        std::exit(1);
    }

    if (novo != NULL && novo->get_estado() == aceitacao) {
        registra_classificacao(ultima_palavra,novo->get_classificacao(),linha);
    }

    
    // std::cout << (int) '0' << " " << (int) '9';
    return 0;
}
