#pragma once

#include <bits/stdc++.h>
using namespace std;

#ifndef __DICTIONARY_H_INCLUDED__
#define __DICTIONARY_H_INCLUDED__

class Dictionary{
    public:
        //strをseparatorで区切る
        static vector<string> split(string str, string separator);
        //入力単語に対する品詞を返す
        static string wordDictionary(string word);
        //生成規則を返す
        static vector<vector<string>> grammerDictionary();
};

#endif
