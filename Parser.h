#pragma once

#include <bits/stdc++.h>
using namespace std;

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
        vector<vector<vector<string>>> CKYList;
        //何番生成規則から生成されたか数字を保存
        vector<vector<int>> generationIntList;
        //生成規則の文字列を保存
        vector<vector<string>> generationStringList;

    //メソッド
    public:
        //コンストラクタ
        Parser(string sentenceFileName,string wordDictionaryFileName,string grammerDictionaryFileName);
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
        string makeSSentence(vector<vector<int>> generationIntList,vector<vector<string>> generationStringList,int itr);

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

        //generationStringListのゲッター
        vector<vector<string>> getGenerationStringList();
        //generationStringListのセッター
        void setGenerationStringList(vector<vector<string>> generationStringList);

        //generationIntListのゲッター
        vector<vector<int>> getGenerationIntList();
        //generationIntListのセッター
        void setGenerationIntList(vector<vector<int>> generationIntList);

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