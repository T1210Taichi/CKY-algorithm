#include "Parser.h"
#include "Dictionary.h"

#include <bits/stdc++.h>
using namespace std;

//コンストラクタ
Parser::Parser(string fileName){
    setWordList(toWords(fileName));
}

//受理可能か不可能か調べる
void Parser::parser(){
    //単語辞書を読み込む
    bool b1 = readWordDictionary();
    if(b1 == false){
        return;
    }
    //文法辞書を読み込む
    bool b2 = readGrammerDictionary();
    if(b2 == false){
        return;
    }

    //CKY
    CKY();

    //結果を表示する
    //printWords(getWordList(),getWordClassList());
}

//ファイルから文章を単語ごとにリストに保存して返す
vector<string> Parser::toWords(string fileName){
    vector<string> list;
    ifstream file(fileName);
    string line="";
    string sumline="";
    //1行ずつ読み込む
    while(getline(file,line)){
        sumline += line;
    }
    list = Dictionary::split(sumline," ");

    return list;
}

//単語辞書を読み込む
bool Parser::readWordDictionary(){
    //単語辞書
    vector<string> list;
    //文章
    vector<string> list2 = getWordList();

    //文章に含まれる単語分繰り返す
    for(int i=0;i < list2.size();i++){
        //単語に対応する品詞を入れる
        string s = Dictionary::wordDictionary(list2[i]);
        if(s.empty()){//単語に対応する品詞がなければ
            cout << "文字「" + list2[i] + "」が受理不可能" << endl;
            return false;
        }else{
            //cout << s << endl;
            list.push_back(s);
        }
    }    

    //品詞の配列にセット
    setWordClassList(list);

    return true;
}

//文法辞書を読み込む
bool Parser::readGrammerDictionary(){
    setGrammerDictionaryList(Dictionary::grammerDictionary());
    return true;
}

//結果を表示する
void Parser::printWords(vector<string> words, vector<string> wordClasses){
    if(words.size() == 0){
        cout << "文章がありません" << endl;
        return;
    }

    //11文字区切りにする
    int n = 11;

    //単語を表示
    for(int i=0;i<n;i++){
        cout << " ";
    }
    for(int i=0;i<words.size();i++){
        cout << words[i];

        int x = n - (words[i].size());
        for(int j=0;j<x;j++){
            cout << " ";
        }
    }
    cout << "" << endl;
    //縦棒を表示
    for(int i=0;i<wordClasses.size();i++){
        cout << "|" ;
        for(int j=0;j<n-1;j++){
            cout << " ";
        }
    }
    cout << "" << endl;
    //品詞を表示
    for(int i=0;i<wordClasses.size();i++){
        string s = wordClasses[i];
        cout << s << "";

        int x = n - (wordClasses[i].size());
        for(int j = 0;j < x ; j++){
            cout << " ";
        }
    }
    cout <<""<<endl;
    cout << "受理可能" << endl;
}


//CKYアルゴリズム
void Parser::CKY(){
    //n文字からなる文字列を入力する
    vector<string> sentenceList = getWordList();
    //入力文に対する品詞
    vector<string> wordClassList = getWordClassList();
    //文法はr個の非終端記号からなる
    vector<vector<string>> grammerDictionaryList = getGrammerDictionaryList();

    int n = sentenceList.size();

    //P[n,n,r]をboolean型の配列とする。Pの全要素をfalseで初期化する
    //P[よこ、たかさ、生成規則]
    vector<vector<vector<string>>> P(n+1,vector<vector<string>>(n+1,vector<string>(grammerDictionaryList.size())));

    //入力文に対応する終端記号
    for(int i=1;i<=n;i++){
        //品詞を代入
        //P[i][i].push_back(wordClassList[i]);
        P[i][i][0] = wordClassList[i-1];

        for(vector<string> arry:grammerDictionaryList){
            if(P[i][i][0].compare(arry[1]) == 0 && arry.size() == 2){
                P[i][i][1] = arry[0];
            }
        }
    }

    for(int d=1;d<n;d++){//高さ
        for(int i=1;i<=n-d;i++){//横幅
            int j = i + d;
            for(int k=i;k<j;k++){//深さ
                for(vector<string> arry:grammerDictionaryList){//生成規則
                    //A -> BCのみ
                    if(arry.size() > 2){
                        //NOUN|NP  VERB|VPをふりわけ
                        if((P[i][k][0].compare(arry[1]) == 0) && (P[k+1][j][0].compare(arry[2]) == 0)){
                            P[i][j][0] = arry[0];
                        }else if(!P[i][k][1].empty() && P[k+1][j][1].empty()){
                            if((P[i][k][1].compare(arry[1]) == 0) && (P[k+1][j][0].compare(arry[2]) == 0))
                                P[i][j][0] = arry[0];
                        }else if(P[i][k][1].empty() && !P[k+1][j].empty()){
                            if((P[i][k][0].compare(arry[1]) == 0) && (P[k+1][j][1].compare(arry[2]) == 0))
                                P[i][j][0] = arry[0];
                        }else if(!P[i][k][1].empty() && !P[k+1][j].empty()){
                            if((P[i][k][1].compare(arry[1]) == 0) && (P[k+1][j][1].compare(arry[2]) == 0))
                                P[i][j][0] = arry[0];
                        }
                    }
                }
            }
        }
    }

    for(int i=1;i<=n;i++){
        for(int j=1;j<=n;j++){
            if(P[i][j][0].compare("") == 0){
                cout <<setw(9) << "";
            }else{
                cout << setw(9) <<P[i][j][0] + "   ";
            }
        }
        cout << "" << endl;
    }
    if(P[1][n][0].compare("SENTENCE") == 0)
        cout << "acceptance" << endl;
    else
        cout << "not acceptance" << endl;

    setCKYList(P);
    
}


//wordListのゲッター
vector<string> Parser::getWordList(){
    return this->wordList;
}
//wordListのセッター
void Parser::setWordList(vector<string> wList){
    this->wordList = wList;
}
//wordClassListのゲッター
vector<string> Parser::getWordClassList(){
    return this->wordClassList;
}
//wordClassListのセッター
void Parser::setWordClassList(vector<string> wClassList){
    this->wordClassList = wClassList;
}


//grammerDictionaryListのゲッター
vector<vector<string>> Parser::getGrammerDictionaryList(){
    return this->grammerDictionaryList;
}
//grammerDictionaryListのセッター
void Parser::setGrammerDictionaryList(vector<vector<string>> grammerDictionaryList){
    this->grammerDictionaryList = grammerDictionaryList;
}
//CKYListのゲッター
vector<vector<vector<string>>> Parser::getCKYList(){
    return this->CKYList;
}
//CKYListのセッター
void Parser::setCKYList(vector<vector<vector<string>>> CKYList){
    this->CKYList = CKYList;
}