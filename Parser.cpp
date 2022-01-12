#include "Parser.h"
#include "Dictionary.h"

#include <bits/stdc++.h>
using namespace std;

//コンストラクタ
Parser::Parser(string sentenceFileName,string wordDictionaryFileName,string grammerDictionaryFileName){
    setWordList(toWords(sentenceFileName));
    setWordDictionaryFileName(wordDictionaryFileName);
    setGrammerDictionaryFileName(grammerDictionaryFileName);
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
    printWords();
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
        string s = Dictionary::wordDictionary(list2[i],getWordDictionaryFileName());
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
    setGrammerDictionaryList(Dictionary::grammerDictionary(getGrammerDictionaryFileName()));
    return true;
}

//結果を表示する
void Parser::printWords(){
    int n = getWordList().size();

    vector<vector<int>> generationIntList = getGenerationIntList();
    vector<vector<string>> generationStringList = getGenerationStringList();
    vector<vector<vector<string>>> P = getCKYList();
    //作業しているブロックをカウント
    int countNum = 1;

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

    //リストを表示

    cout << setw(3) << "Num";
    cout << setw(5) << "left";
    cout << setw(5) << "no.";
    cout << setw(5) << "bottom";
    cout << setw(5) << "no.";
    cout << setw(12) << "result";
    cout << setw(12) << "left";
    cout << setw(12) << "bottom" << endl;
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

    //S文を表示
    string S = makeSSentence(generationIntList,generationStringList,generationIntList.size());
    cout << S << endl;
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
    //generationStringList[8] = ["SENTENCE8","NP1","VP2"];
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
                //探索ブロックの深さを移動
                B += tempN--;
                C -= ++Num;
            }
            //このブロックで生成されなければ
            if(generationIntList.size() < countNum){
                generationIntList.push_back({-1,-1,-1,-1});
                generationStringList.push_back({"-----","-----","-----"});
            }
            //ブロックを横に移動
            countNum++;
        }
    }

    setCKYList(P);
    setGenerationIntList(generationIntList);
    setGenerationStringList(generationStringList);
    
}

//S文の作成
//(generationIntList,generationStringList,itr)
//itr:表示するリストの番号
string Parser::makeSSentence(vector<vector<int>> generationIntList,vector<vector<string>> generationStringList,int itr){
    //入力文以下のサイズになったら品詞を返す
    if(itr <= getWordList().size()){
        return "(" + generationStringList[itr-1][0] + " \"" + getWordList()[itr-1] + "\")";
    }else if(itr < 0){
        cout << "error" << endl;
    }

    string A = generationStringList[itr-1][0];
    string B="",C="";

    B = makeSSentence(generationIntList,generationStringList,generationIntList[itr-1][0]);
    C = makeSSentence(generationIntList,generationStringList,generationIntList[itr-1][2]);

    return "(" + A + B + C + ")";
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

//generationStringListのゲッター
vector<vector<string>> Parser::getGenerationStringList(){
    return this->generationStringList;
}
//generationStringListのセッター
void Parser::setGenerationStringList(vector<vector<string>> generationStringList){
    this->generationStringList = generationStringList;
}

//generationIntListのゲッター
vector<vector<int>> Parser::getGenerationIntList(){
    return this->generationIntList;
}
//generationIntListのセッター
void Parser::setGenerationIntList(vector<vector<int>> generationIntList){
    this->generationIntList = generationIntList;
}

//wordDictionaryFileNameのゲッター
string Parser::getWordDictionaryFileName(){
    return this->wordDictionaryFileName;
}
//wordDictionaryFileNameのセッター
void Parser::setWordDictionaryFileName(string file){
    this->wordDictionaryFileName = file;
}

//grammerDictionaryFileNameのゲッター
string Parser::getGrammerDictionaryFileName(){
    return this->grammerDictionaryFileName;
}
//grammerDictionaryFileNameのセッター
void Parser::setGrammerDictionaryFileName(string file){
    this->grammerDictionaryFileName = file;
}