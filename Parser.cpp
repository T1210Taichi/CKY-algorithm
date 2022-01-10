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

    //作業しているブロックをカウント
    int countNum = 1;
    //ex) S8 -> NP1 VP2
    //[ブロックの番号,P[][][1],ブロック番号,P[][][1]]
    //generationIntList[8] = [1,1,2,1]
    //ex) VP15 -> VERB2 NP10
    //[ブロックの番号,P[][][0],ブロック番号,P[][][0]]
    //generationIntList[15] = [2,0,10,0]
    vector<vector<int>> generationIntList;
    //geterationStringList[8] = ["SENTENCE8","NP1","VP2"];
    vector<vector<string>> generationStringList;

    //入力文に対応する終端記号
    for(int i=1;i<=n;i++){
        //品詞を代入
        P[i][i][0] = wordClassList[i-1];

        //作業ブロックをカウント
        generationIntList.push_back({0,0,0,0});
        generationStringList.push_back({P[i][i][0] + to_string(i)});
        countNum++;

        for(vector<string> arry:grammerDictionaryList){
            if(P[i][i][0].compare(arry[1]) == 0 && arry.size() == 2){
                P[i][i][1] = arry[0];
                generationStringList[i-1].push_back({P[i][i][1] + to_string(i)});
            }
        }
    }

    //A->BCのBとCのブロックの数字
    int B=0,C=0;
    int tempN = n;
    int Num=0;

    for(int d=1;d<n;d++){//高さ
        for(int i=1;i<=n-d;i++){//横幅
            int j = i + d;

            //変数の初期化
            Num = n - d;
            tempN = n;
            //左
            B = i;
            //一つ下
            C = countNum - Num;
            for(int k=i;k<j;k++){//深さ
                for(vector<string> arry:grammerDictionaryList){//生成規則
                    //A -> BCのみ
                    if(arry.size() > 2){
                        //NOUN|NP  VERB|VPをふりわけ
                        if((P[i][k][0].compare(arry[1]) == 0) && (P[k+1][j][0].compare(arry[2]) == 0)){
                            P[i][j][0] = arry[0];
                            //Listに記入
                            generationIntList.push_back({B,0,C,0});
                            generationStringList.push_back({P[i][j][0] + to_string(countNum), P[i][k][0] + to_string(B), P[k+1][j][0] + to_string(C)});

                            //cout << arry[0] + to_string(countNum) << endl;//debug
                        }else if(!P[i][k][1].empty() && P[k+1][j][1].empty()){
                            if((P[i][k][1].compare(arry[1]) == 0) && (P[k+1][j][0].compare(arry[2]) == 0)){
                                P[i][j][0] = arry[0];
                                //Listに記入
                                generationIntList.push_back({B,1,C,0});
                                generationStringList.push_back({P[i][j][0] + to_string(countNum), P[i][k][1] + to_string(B), P[k+1][j][0] + to_string(C)});


                                //cout << arry[0] + to_string(countNum) << endl;//debug
                            }
                        }else if(P[i][k][1].empty() && !P[k+1][j].empty()){
                            if((P[i][k][0].compare(arry[1]) == 0) && (P[k+1][j][1].compare(arry[2]) == 0)){
                                P[i][j][0] = arry[0];
                                //Listに記入
                                generationIntList.push_back({B,0,C,1});
                                generationStringList.push_back({P[i][j][0] + to_string(countNum), P[i][k][0] + to_string(B), P[k+1][j][1] + to_string(C)});


                                //cout << arry[0] + to_string(countNum) << endl;//debug
                            }
                        }else if(!P[i][k][1].empty() && !P[k+1][j].empty()){
                            if((P[i][k][1].compare(arry[1]) == 0) && (P[k+1][j][1].compare(arry[2]) == 0)){
                                P[i][j][0] = arry[0];
                                //Listに記入
                                generationIntList.push_back({B,1,C,1});
                                generationStringList.push_back({P[i][j][0] + to_string(countNum), P[i][k][1] + to_string(B), P[k+1][j][1] + to_string(C)});


                                //cout << arry[0] + to_string(countNum) << endl;//debug
                            }
                        }
                    }

                }
                //ブロックの移動
                B += tempN--;
                Num++;
            }
            //このブロックで生成されなければ
            if(generationIntList.size() < countNum){
                generationIntList.push_back({-1,-1,-1,-1});
                generationStringList.push_back({"-----","-----","-----"});
            }
            //ブロックを移動
            countNum++;
        }
    }

    int addNum = n;
    //三角行列を表示
    for(int i=1;i<=n;i++){
        for(int j=1;j<=n;j++){
            if(i == j){
                countNum = i;
                addNum = n;   
            }
            if(P[i][j][1].compare("") == 0){
                if(P[i][j][0].compare("") == 0){
                    cout <<setw(13) << "";
                }else{
                    cout << setw(13) <<P[i][j][0] + to_string(countNum) + "   ";
                }
            }else{
                if(P[i][j][0].compare("") == 0){
                    cout <<setw(13) << "";
                }else{
                    cout << setw(13) <<P[i][j][0] + to_string(countNum) + "(" + P[i][j][1] + to_string(countNum) + ")" + "   ";
                }                
            }

            countNum += addNum;
            addNum--;
        }
        cout << "" << endl;
    }

    //受理か非受理か
    if(P[1][n][0].compare("SENTENCE") == 0)
        cout << "acceptance" << endl;
    else
        cout << "not acceptance" << endl;

    cout << setw(3) << "Num";
    cout << setw(5) << "left";
    cout << setw(5) << "no.";
    cout << setw(5) << "bottom";
    cout << setw(5) << "no.";
    cout << setw(12) << "result";
    cout << setw(12) << "left";
    cout << setw(12) << "bottom" << endl;;


    for(int i=0;i<generationIntList.size();i++){
        cout << setw(3) << i+1;
        for(int a:generationIntList[i]){
            cout << setw(5) << a;
        }
        for(string a:generationStringList[i]){
            cout << setw(12) << a;
        }
        cout << "" << endl;
    }

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