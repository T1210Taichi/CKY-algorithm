#pragma once

#include <bits/stdc++.h>
using namespace std;

#ifndef __PARSER_H_INCLUDED__
#define __PARSER_H_INCLUDED__

class Parser{
    //フィールド
    private:
        vector<string> wordList;
        vector<string> wordClassList;
        
        vector<vector<string>> grammerDictionaryList;
        vector<vector<vector<string>>> CKYList;

    //メソッド
    public:
        //コンストラクタ
        Parser(string fileName);
        //受理可能か不可能か調べる
        void parser();
        //ファイルから文章を単語ごとにリストに保存して返す
        vector<string> toWords(string fileName);
        //単語辞書を読み込む
        bool readWordDictionary();
        //文法辞書を読み込む
        bool readGrammerDictionary();
        //結果を表示する
        void printWords(vector<string> words,vector<string> wordClasses);
        //CKYアルゴリズム
        void CKY();

        //wordListのゲッター
        vector<string> getWordList();
        //wordListのセッター
        void setWordList(vector<string> wordList);
        //wordClassListのゲッター
        vector<string> getWordClassList();
        //wordClassListのセッター
        void setWordClassList(vector<string> wClassList);

        //grammerのゲッター
        vector<vector<string>> getGrammerDictionaryList();
        //grammerのセッター
        void setGrammerDictionaryList(vector<vector<string>> grammerDictionaryList);
        //CKYListのゲッター
        vector<vector<vector<string>>> getCKYList();
        //CKYListのセッター
        void setCKYList(vector<vector<vector<string>>> CKYList);

};

#endif