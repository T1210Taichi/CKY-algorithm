#include <bits/stdc++.h>
using namespace std;

#include "Parser.h"
#include "Dictionary.h"

int main(){

    cout << "ファイル名を入力"<< endl;
    string fileName = "sentence.txt";
    //cin >> fileName;
    Parser p = Parser(fileName);
    p.parser();

    return 0;

}