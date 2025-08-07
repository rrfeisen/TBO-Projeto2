#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <regex>
#include <iomanip>

using namespace std;

// ==================== CLASSE KMP ====================
class KMP {
private:
    vector<int> calcularLPS(const string& padrao) {
        int tamanho = padrao.length();
        vector<int> lps(tamanho, 0);
        int comprimento = 0;
        int indice = 1;
        
        while (indice < tamanho) {
            if (padrao[indice] == padrao[comprimento]) {
                comprimento++;
                lps[indice] = comprimento;
                indice++;
            } else {
                if (comprimento != 0) {
                    comprimento = lps[comprimento - 1];
                } else {
                    lps[indice] = 0;
                    indice++;
                }
            }
        }
        return lps;
    }
    
public:
    vector<int> buscar(const string& texto, const string& padrao) {
        vector<int> ocorrencias;
        int tamanhoTexto = texto.length();
        int tamanhoPadrao = padrao.length();
        
        if (tamanhoPadrao == 0) return ocorrencias;
        
        vector<int> lps = calcularLPS(padrao);
        
        int i = 0; // índice para texto
        int j = 0; // índice para padrão
        
        while (i < tamanhoTexto) {
            if (padrao[j] == texto[i]) {
                i++;
                j++;
            }
            
            if (j == tamanhoPadrao) {
                ocorrencias.push_back(i - j);
                j = lps[j - 1];
            } else if (i < tamanhoTexto && padrao[j] != texto[i]) {
                if (j != 0) {
                    j = lps[j - 1];
                } else {
                    i++;
                }
            }
        }
        
        return ocorrencias;
    }
    
    void exibirResultados(const string& texto, const string& padrao, const vector<int>& ocorrencias) {
        cout << "\n=== Resultados da Busca KMP ===" << endl;
        cout << "Padrão buscado: \"" << padrao << "\"" << endl;
        cout << "Total de ocorrências: " << ocorrencias.size() << endl;
        
        for (size_t i = 0; i < ocorrencias.size(); i++) {
            int posicao = ocorrencias[i];
            cout << "Ocorrência " << (i + 1) << " na posição " << posicao << ": ";
            
            // Mostrar contexto
            int inicio = max(0, posicao - 10);
            int fim = min((int)texto.length(), posicao + (int)padrao.length() + 10);
            
            string contexto = texto.substr(inicio, fim - inicio);
            cout << "\"..." << contexto << "...\"" << endl;
        }
    }
};

// ==================== CLASSE CURINGA KMP ====================
class CuringaKMP {
private:
    bool coincideCaractere(char caractereTexto, char caracterePadrao) {
        return caracterePadrao == '*' || caractereTexto == caracterePadrao;
    }
    
public:
    vector<int> buscarComCuringa(const string& texto, const string& padrao) {
        vector<int> ocorrencias;
        int tamanhoTexto = texto.length();
        int tamanhoPadrao = padrao.length();
        
        for (int i = 0; i <= tamanhoTexto - tamanhoPadrao; i++) {
            bool coincide = true;
            for (int j = 0; j < tamanhoPadrao; j++) {
                if (!coincideCaractere(texto[i + j], padrao[j])) {
                    coincide = false;
                    break;
                }
            }
            if (coincide) {
                ocorrencias.push_back(i);
            }
        }
        
        return ocorrencias;
    }
    
    void exibirResultados(const string& texto, const string& padrao, const vector<int>& ocorrencias) {
        cout << "\n=== Resultados da Busca com Curinga ===" << endl;
        cout << "Padrão buscado: \"" << padrao << "\" (* = qualquer caractere)" << endl;
        cout << "Total de ocorrências: " << ocorrencias.size() << endl;
        
        for (size_t i = 0; i < ocorrencias.size(); i++) {
            int posicao = ocorrencias[i];
            cout << "Ocorrência " << (i + 1) << " na posição " << posicao << ": ";
            
            // Mostrar contexto
            int inicio = max(0, posicao - 10);
            int fim = min((int)texto.length(), posicao + (int)padrao.length() + 10);
            
            string contexto = texto.substr(inicio, fim - inicio);
            cout << "\"..." << contexto << "...\"" << endl;
        }
    }
};

// ==================== CLASSE MINERAÇÃO DE DADOS ====================
class MineracaoDados {
private:
    struct PadraoDados {
        string nome;
        string padrao;
        string descricao;
    };
    
    vector<PadraoDados> padroes = {
        {"Data", R"(\b\d{1,2}[\/\-]\d{1,2}[\/\-]\d{2,4}\b)", "Datas no formato dd/mm/yyyy ou dd-mm-yyyy"},
        {"Email", R"(\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Z|a-z]{2,}\b)", "Endereços de email"},
        {"Telefone", R"(\b\(?[0-9]{2}\)?\s?[0-9]{4,5}-?[0-9]{4}\b)", "Números de telefone brasileiros"},
        {"CPF", R"(\b\d{3}\.?\d{3}\.?\d{3}-?\d{2}\b)", "Números de CPF"},
        {"CEP", R"(\b\d{5}-?\d{3}\b)", "Códigos postais (CEP)"},
        {"URL", R"(https?:\/\/(www\.)?[-a-zA-Z0-9@:%._\+~#=]{1,256}\.[a-zA-Z0-9()]{1,6}\b([-a-zA-Z0-9()@:%_\+.~#?&//=]*))", "URLs da web"}
    };
    
public:
    map<string, vector<string>> extrairDados(const string& texto) {
        map<string, vector<string>> resultados;
        
        for (const auto& padrao : padroes) {
            regex padraoRegex(padrao.padrao);
            sregex_iterator inicio(texto.begin(), texto.end(), padraoRegex);
            sregex_iterator fim;
            
            vector<string> coincidencias;
            for (sregex_iterator i = inicio; i != fim; ++i) {
                smatch coincidencia = *i;
                coincidencias.push_back(coincidencia.str());
            }
            
            if (!coincidencias.empty()) {
                resultados[padrao.nome] = coincidencias;
            }
        }
        
        return resultados;
    }
    
    void exibirResultados(const map<string, vector<string>>& resultados) {
        cout << "\n=== Resultados da Mineração de Dados ===" << endl;
        
        if (resultados.empty()) {
            cout << "Nenhum dado estruturado encontrado." << endl;
            return;
        }
        
        for (const auto& resultado : resultados) {
            cout << "\n" << resultado.first << " encontrados (" << resultado.second.size() << "):" << endl;
            for (size_t i = 0; i < resultado.second.size(); i++) {
                cout << "  " << (i + 1) << ". " << resultado.second[i] << endl;
            }
        }
    }
};

// ==================== CLASSE NUVEM DE PALAVRAS ====================
class NuvemPalavras {
private:
    struct FrequenciaPalavra {
        string palavra;
        int frequencia;
    };
    
    // Implementação de merge sort para ordenar por frequência
    void mergeSort(vector<FrequenciaPalavra>& arranjo, int esquerda, int direita) {
        if (esquerda < direita) {
            int meio = esquerda + (direita - esquerda) / 2;
            
            mergeSort(arranjo, esquerda, meio);
            mergeSort(arranjo, meio + 1, direita);
            merge(arranjo, esquerda, meio, direita);
        }
    }
    
    void merge(vector<FrequenciaPalavra>& arranjo, int esquerda, int meio, int direita) {
        int n1 = meio - esquerda + 1;
        int n2 = direita - meio;
        
        vector<FrequenciaPalavra> arranjoEsquerda(n1);
        vector<FrequenciaPalavra> arranjoDireita(n2);
        
        for (int i = 0; i < n1; i++)
            arranjoEsquerda[i] = arranjo[esquerda + i];
        for (int j = 0; j < n2; j++)
            arranjoDireita[j] = arranjo[meio + 1 + j];
        
        int i = 0, j = 0, k = esquerda;
        
        while (i < n1 && j < n2) {
            // Ordenar por frequência decrescente
            if (arranjoEsquerda[i].frequencia >= arranjoDireita[j].frequencia) {
                arranjo[k] = arranjoEsquerda[i];
                i++;
            } else {
                arranjo[k] = arranjoDireita[j];
                j++;
            }
            k++;
        }
        
        while (i < n1) {
            arranjo[k] = arranjoEsquerda[i];
            i++;
            k++;
        }
        
        while (j < n2) {
            arranjo[k] = arranjoDireita[j];
            j++;
            k++;
        }
    }
    
    string paraMinusculo(const string& str) {
        string resultado = str;
        transform(resultado.begin(), resultado.end(), resultado.begin(), ::tolower);
        return resultado;
    }
    
    bool ehPalavraValida(const string& palavra) {
        if (palavra.length() < 2) return false;
        
        // Palavras muito comuns para filtrar
        vector<string> palavrasVazias = {"de", "da", "do", "das", "dos", "em", "na", "no", "nas", "nos",
                                        "com", "por", "para", "que", "um", "uma", "uns", "umas", "o", "a",
                                        "os", "as", "e", "ou", "mas", "se", "te", "me", "le", "la", "lo"};
        
        for (const string& vazia : palavrasVazias) {
            if (palavra == vazia) return false;
        }
        
        return true;
    }
    
public:
    map<string, int> analisarFrequencia(const string& texto) {
        map<string, int> contagemPalavras;
        string palavra = "";
        
        for (char c : texto) {
            if (isalpha(c)) {
                palavra += c;
            } else if (!palavra.empty()) {
                string palavraMinuscula = paraMinusculo(palavra);
                if (ehPalavraValida(palavraMinuscula)) {
                    contagemPalavras[palavraMinuscula]++;
                }
                palavra = "";
            }
        }
        
        // Processar última palavra se houver
        if (!palavra.empty()) {
            string palavraMinuscula = paraMinusculo(palavra);
            if (ehPalavraValida(palavraMinuscula)) {
                contagemPalavras[palavraMinuscula]++;
            }
        }
        
        return contagemPalavras;
    }
    
    void gerarNuvemPalavras(const map<string, int>& contagemPalavras, int maxPalavras = 20) {
        cout << "\n=== Nuvem de Palavras ===" << endl;
        
        if (contagemPalavras.empty()) {
            cout << "Nenhuma palavra encontrada." << endl;
            return;
        }
        
        // Converter map para vector para ordenação
        vector<FrequenciaPalavra> palavras;
        for (const auto& par : contagemPalavras) {
            palavras.push_back({par.first, par.second});
        }
        
        // Ordenar usando merge sort
        if (!palavras.empty()) {
            mergeSort(palavras, 0, palavras.size() - 1);
        }
        
        // Limitar número de palavras
        int quantidadeExibir = min(maxPalavras, (int)palavras.size());
        
        cout << "Top " << quantidadeExibir << " palavras mais frequentes:" << endl;
        cout << string(50, '=') << endl;
        
        for (int i = 0; i < quantidadeExibir; i++) {
            // Criar representação visual baseada na frequência
            cout << setw(15) << left << palavras[i].palavra 
                 << " [" << palavras[i].frequencia << "] " << endl;
        }
        
        // Estatísticas
        cout << "\nEstatísticas:" << endl;
        cout << "Total de palavras únicas: " << palavras.size() << endl;
        cout << "Palavra mais frequente: " << palavras[0].palavra << " (" << palavras[0].frequencia << " vezes)" << endl;
        
        if (palavras.size() > 1) {
            cout << "Segunda mais frequente: " << palavras[1].palavra << " (" << palavras[1].frequencia << " vezes)" << endl;
        }
    }
};

// ==================== CLASSE PRINCIPAL ====================
class SistemaAnaliseTexto {
private:
    KMP kmp;
    CuringaKMP curingaKmp;
    MineracaoDados mineracaoDados;
    NuvemPalavras nuvemPalavras;
    string textoAtual;
    
    void carregarTextoExemplo() {
        textoAtual = "João Silva mora na Rua das Flores, 123, CEP 12345-678. "
                     "Seu telefone é (11) 98765-4321 e email joao.silva@email.com. "
                     "Nasceu em 15/03/1985 e trabalha na empresa XYZ desde 10-01-2020. "
                     "Seu CPF é 123.456.789-10. Acesse o site www.exemplo.com.br "
                     "ou https://github.com/usuario para mais informações. "
                     "Contato alternativo: maria.santos@empresa.com.br, telefone (21) 5555-1234. "
                     "Data de cadastro: 25/12/2023. CEP da empresa: 54321-987. "
                     "Os dados dos funcionários incluem nomes, endereços, telefones e emails. "
                     "Todos os documentos são importantes para a empresa. "
                     "A análise de texto pode revelar padrões interessantes nos dados. "
                     "Palavras repetidas aparecem com maior frequência na análise. "
                     "Este é um exemplo de texto para demonstrar as funcionalidades do sistema.";
    }
    
public:
    SistemaAnaliseTexto() {
        carregarTextoExemplo();
    }
    
    void exibirMenu() {
        cout << "\n" << string(60, '=') << endl;
        cout << "    SISTEMA DE ANÁLISE DE TEXTO" << endl;
        cout << string(60, '=') << endl;
        cout << "1. Busca KMP (Tarefa 1)" << endl;
        cout << "2. Busca com Curinga (Tarefa 2)" << endl;
        cout << "3. Mineração de Dados (Tarefa 7)" << endl;
        cout << "4. Nuvem de Palavras (Tarefa 9)" << endl;
        cout << "5. Carregar texto de arquivo" << endl;
        cout << "0. Sair" << endl;
        cout << string(60, '=') << endl;
        cout << "Escolha uma opção: ";
    }
    
    void carregarTextoDoArquivo() {
        string nomeArquivo;
        cout << "Digite o nome do arquivo: ";
        cin.ignore();
        getline(cin, nomeArquivo);
        
        ifstream arquivo(nomeArquivo);
        if (arquivo.is_open()) {
            string linha;
            textoAtual = "";
            while (getline(arquivo, linha)) {
                textoAtual += linha + " ";
            }
            arquivo.close();
            cout << "Texto carregado com sucesso!" << endl;
        } else {
            cout << "Erro ao abrir o arquivo!" << endl;
        }
    }
    
    void executarBuscaKMP() {
        string padrao;
        cout << "Digite a palavra a ser buscada: ";
        cin >> padrao;
        
        vector<int> resultados = kmp.buscar(textoAtual, padrao);
        kmp.exibirResultados(textoAtual, padrao, resultados);
    }
    
    void executarBuscaCuringa() {
        string padrao;
        cout << "Digite o padrão com curingas (* = qualquer caractere): ";
        cin >> padrao;
        
        vector<int> resultados = curingaKmp.buscarComCuringa(textoAtual, padrao);
        curingaKmp.exibirResultados(textoAtual, padrao, resultados);
    }
    
    void executarMineracaoDados() {
        auto resultados = mineracaoDados.extrairDados(textoAtual);
        mineracaoDados.exibirResultados(resultados);
    }
    
    void executarNuvemPalavras() {
        auto frequencia = nuvemPalavras.analisarFrequencia(textoAtual);
        
        int maxPalavras;
        cout << "Quantas palavras exibir na nuvem? (padrão: 20): ";
        cin >> maxPalavras;
        if (maxPalavras <= 0) maxPalavras = 20;
        
        nuvemPalavras.gerarNuvemPalavras(frequencia, maxPalavras);
    }
    
    void executar() {
        int escolha;
        do {
            exibirMenu();
            cin >> escolha;
            
            switch (escolha) {
                case 1:
                    executarBuscaKMP();
                    break;
                case 2:
                    executarBuscaCuringa();
                    break;
                case 3:
                    executarMineracaoDados();
                    break;
                case 4:
                    executarNuvemPalavras();
                    break;
                case 5:
                    carregarTextoDoArquivo();
                    break;
                case 0:
                    cout << "Encerrando o programa..." << endl;
                    break;
                default:
                    cout << "Opção inválida!" << endl;
            }
            
            if (escolha != 0) {
                cout << "\nPressione Enter para continuar...";
                cin.ignore();
                cin.get();
            }
            
        } while (escolha != 0);
    }
};

// ==================== FUNÇÃO MAIN ====================
int main() {
    SistemaAnaliseTexto sistema;
    sistema.executar();
    return 0;
}
