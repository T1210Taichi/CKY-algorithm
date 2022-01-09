#include "Dictionary.h"
#include "Parser.h"

#include <bits/stdc++.h>
using namespace std;

//文字列を空白で分割
vector<string> Dictionary::split(string str, string separator){
    if (separator == "") return {str};
    vector<string> result;
    string tstr = str + separator;
    long l = tstr.length(), sl = separator.length();
    string::size_type pos = 0, prev = 0;
    
    for (;pos < l && (pos = tstr.find(separator, pos)) != string::npos; prev = (pos += sl)) {
        result.emplace_back(tstr, prev, pos - prev);
    }
    return result;
}

//単語辞書
string Dictionary::wordDictionary(string word){
    //ファイル
    ifstream file("wordDictionary.txt");

    string line="";
    vector<string> wv;
    //1行ずつ読み込む
    while(getline(file,line)){

        wv = split(line," ");
        //読み込んんだ単語がwordと同じなら品詞を返す
        if(wv[0] == word){
            //cout << "word : " + wv[0] + " : " + wv[1] << endl;

            string wordClass = wv[1];
            return wordClass;
        }

        wv.clear();
    }

    return NULL;
}

//文法辞書
vector<vector<string>> Dictionary::grammerDictionary(){

    vector<vector<string>> list;

    ifstream file("grammerDictionary.txt");
    string line="";
    //1行ずつ読み込む
    while(getline(file,line)){
        //1行読み込んで空白で分割
        vector<string> grammer = split(line," ");
        //listに追加
        list.push_back(grammer);
    }

    return list;
}

