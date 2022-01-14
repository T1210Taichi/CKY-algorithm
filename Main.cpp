#include <bits/stdc++.h>
using namespace std;

#include "Parser.h"
#include "Dictionary.h"
#include "Node.h"

int main(){

    cout << "文章ファイル名を入力"<< endl;
    string sentenceFileName = "sentence.txt";
    //cin >> sentenceFileName;
    cout << "単語辞書ファイル名を入力"<< endl;
    string wordDictionaryFileName = "wordDictionary.txt";
    //cin >> wordDictionaryFileName;
    cout << "文法規則ファイル名を入力"<< endl;
    string grammerDictionaryFileName = "grammerDictionary.txt";
    //cin >> grammerDictionaryFileName;
    Parser p = Parser(sentenceFileName,wordDictionaryFileName,grammerDictionaryFileName);
    p.parser();

    return 0;

}