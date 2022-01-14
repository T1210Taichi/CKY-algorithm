#include "Parser.h"
#include "Dictionary.h"
#include "Node.h"


#include <bits/stdc++.h>
using namespace std;

using np = Node *;//Nodeのポインタ型

//コンストラクタ
Parser::Parser(string sentenceFileName,string wordDictionaryFileName,string grammerDictionaryFileName){
    setWordList(toWords(sentenceFileName));
    setWordDictionaryFileName(wordDictionaryFileName);
    setGrammerDictionaryFileName(grammerDictionaryFileName);
}
//デストラクタ
Parser::~Parser(){}

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

    vector<vector<vector<np>>> P = getCKYList();
    //作業しているブロックをカウント
    int countNum = 1;

    //三角行列を表示
    for(int i=1;i<=n;i++){
        //空白を調整
        if(i != 1){
            for(int k=0;k<0;k++){
                cout << setw(10) << "";
            }
        }
        //表示
        for(int j=1;j<=n;j++){
            vector<np> node = P[i][j];
            int foge=0;
            string s="";
            for(np n:node){
                if(foge++ == 0)
                    s += (*n).getValueString() + to_string((*n).getValueInt());
                else
                    s += "(" + (*n).getValueString() + to_string((*n).getValueInt()) + ")";
            }
            cout << setw(10) << s;
        }
        cout << endl;
    }

    //受理か非受理か
    if((*P[1][n][0]).getValueString().compare("SENTENCE") == 0)
        cout << "acceptance" << endl;
    else
        cout << "not acceptance" << endl;

    //S文を表示
    string s = makeSSentence(P,P[1][n][0]);
    cout << s << endl;
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
    vector<vector<vector<np>>> P(n+1,vector<vector<np>>(n+1,vector<np>()));

    //作業しているブロックをカウント
    int countNum = 1;

    //入力文に対応する終端記号
    for(int i=1;i<=n;i++){
        np word = new Node(0,sentenceList[i-1],nullptr,nullptr);
        np n0 = new Node(i,wordClassList[i-1],word,nullptr);
        //品詞を代入
        P[i][i].push_back({n0});

        
        //作業ブロックをカウント
        countNum++;
        //A -> Bの生成規則を適応
        for(vector<string> arry:grammerDictionaryList){
            for(np node:P[i][i]){
                if((*node).getValueString().compare(arry[1]) == 0 && arry.size() == 2){
                    np n = new Node(i,arry[0],n0,nullptr);
                    P[i][i].push_back({n});
                }
            }
        }
        
    }

    //debug
    /*
    for(int i=1;i<=n;i++){
        for(np node: P[i][i]){
            do{
                cout << setw(10) << (*node).getValueString() + to_string((*node).getValueInt());
                node = (*node).getLeft();
            }while(node != nullptr);
            cout << endl;
        }
        
    }
    */

    for(int d=1;d<n;d++){//高さ
        for(int i=1;i<=n-d;i++){//横幅
            int j = i + d;

            for(int k=i;k<j;k++){//深さ
                for(vector<string> arry:grammerDictionaryList){//生成規則
                    //A -> BCのみ
                    if(arry.size() > 2){

                        //P[i][k]とP[k+1][j]の全探索
                        for(np nik:P[i][k]){
                            for(np nkj:P[k+1][j]){
                                if(((*nik).getValueString().compare(arry[1]) == 0) && ((*nkj).getValueString().compare(arry[2]) == 0)){
                                    P[i][j].push_back(new Node(countNum,arry[0],nik,nkj));
                                }
                            }
                        }
                    }

                }
            }
            //ブロックを横に移動
            countNum++;
        }
    }

    setCKYList(P);

}

//S文の作成
//(CKYList,何番目の句か)
string Parser::makeSSentence(vector<vector<vector<np>>> P,np n){

    if(n == nullptr){
        return "";
    }
    //品詞まで届いたら単語をセットで返す
    if((*(*n).getLeft()).getLeft() == nullptr && (*n).getRight() == nullptr){
        return "(" + (*n).getValueString() + to_string((*n).getValueInt()) + " \"" + (*(*n).getLeft()).getValueString() + "\")";
    }
    //A -> B C
    string A = (*n).getValueString() + to_string((*n).getValueInt());
    string B = makeSSentence(P,(*n).getLeft());
    string C = makeSSentence(P,(*n).getRight());
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
vector<vector<vector<np>>> Parser::getCKYList(){
    return this->CKYList;
}
//CKYListのセッター
void Parser::setCKYList(vector<vector<vector<np>>> CKYList){
    this->CKYList = CKYList;
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