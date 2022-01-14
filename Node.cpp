#include "Node.h"
#include "Dictionary.h"
#include "Parser.h"

#include <bits/stdc++.h>
using namespace std;

//デフォルトコンストラクタ
Node::Node(){}
//コピーコンストラクタ
Node::Node(const Node &src){}
//ムーブコンストラクタ
Node::Node(Node&&) = default;
//コンストラクタ
Node::Node(int valueInt,string valueString,Node *left,Node *right){
    setValue(valueInt,valueString);
    setLeft(left);
    setRight(right);
}

//デストラクタ
Node::~Node(){}

//セッター
void Node::setValue(int valueInt,string valueString){
    this->valueInt = valueInt;
    this->valueString = valueString;
}
void Node::setLeft(Node *left){
    this->left = left;
}
void Node::setRight(Node *right){
    this->right = right;
}
//ゲッター
int Node::getValueInt(){
    return this->valueInt;
}
string Node::getValueString(){
    return this->valueString;
}
Node* Node::getLeft(){
    return (this->left);
}
Node* Node::getRight(){
    return (this->right);
}