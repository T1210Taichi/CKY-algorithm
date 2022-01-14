#pragma once

#include "Dictionary.h"
#include "Node.h"

#include <bits/stdc++.h>
using namespace std;
using np = Node *;//Nodeのポインタ型

#ifndef __PARSER_H_INCLUDED__
#define __PARSER_H_INCLUDED__

class Parser{
    //フィールド
    private:
        //ファイル名
        string wordDictionaryFileName;
        string grammerDictionaryFileName;
        //入力文を単語ごとに保存
        vector<string> wordList;
        //入力文の品詞を保存
        vector<string> wordClassList;
        //生成規則を保存
        vector<vector<string>> grammerDictionaryList;
        //三角行列を保存
        vector<vector<vector<np>>> CKYList;

    //メソッド
    public:
        //コンストラクタ
        Parser(string sentenceFileName,string wordDictionaryFileName,string grammerDictionaryFileName);
        //デストラクタ
        ~Parser();
        //受理可能か不可能か調べる
        void parser();
        //ファイルから文章を単語ごとにリストに保存して返す
        vector<string> toWords(string fileName);
        //単語辞書を読み込む
        bool readWordDictionary();
        //文法辞書を読み込む
        bool readGrammerDictionary();
        //結果を表示する
        void printWords();
        //CKYアルゴリズム
        void CKY();
        //S文を作成
        string makeSSentence(vector<vector<vector<np>>> P,np n);

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
        vector<vector<vector<np>>> getCKYList();
        //CKYListのセッター
        void setCKYList(vector<vector<vector<np>>> CKYList);

        //wordDictionaryFileNameのゲッター
        string getWordDictionaryFileName();
        //wordDictionaryFileNameのセッター
        void setWordDictionaryFileName(string file);

        //grammerDictionaryFileNameのゲッター
        string getGrammerDictionaryFileName();
        //grammerDictionaryFileNameのセッター
        void setGrammerDictionaryFileName(string file);

};

#endif