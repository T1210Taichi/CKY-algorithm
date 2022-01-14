#pragma once

#include <bits/stdc++.h>
using namespace std;

#ifndef __Node_H_INCLUDED__
#define __Node_H_INCLUDED__

class Node{
    private:
        //自分の生成規則
        int valueInt;
        string valueString;
        //子を指す
        Node *left;
        Node *right;
    public:
        //コンストラクタ
        Node();
        Node(const Node &src);
        Node(Node&& v);
        Node(int valueInt,string valueString,Node *left,Node *right);
        //デストラクタ
        virtual ~Node();

        //セッター
        void setValue(int valueInt,string valueString);
        void setLeft(Node *left);
        void setRight(Node *right);
        //ゲッター
        int getValueInt();
        string getValueString();
        Node getLeft();
        Node getRight();
};

#endif
