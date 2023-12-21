#ifndef __EXPRANALYSIS_2_H__
#define __EXPRANALYSIS_2_H__
#include <iostream>
#include <vector>
#include <deque>
#include <string>
/*
计算器表达式2.0版本，根据文法优化了代码的结构，使其更加优雅合理 、层次化
目前实现功能为计算加减乘除带括号的整数运算。
*/
#define DEBUG_FLAG 0
using namespace std;
enum TokenType {
    NUM = '0', ADD = '+', SUB = '-', MUL = '*', DIV = '/',
    L_BRACE = '(', R_BRACE = ')', PIONT = '.', END = '!'
}; //数字、+、-、*、/、(、)、.、空


struct Token {
    string value;
    TokenType type;
};
//词法分析器
class Lexer {


};

//语法分析器
class Parser {
private:
    string dataFlow;                //计算表达式字符串的缓冲区
    deque<Token> tokenBuffer;      //存储一系列token的队列
public:
    Parser(string input) :dataFlow{ input } {}
    const string& getDataFlow() { return *&dataFlow; }
    Token popFrontToken(void) {
        Token tmp{ {""},END };
        if (!tokenBuffer.empty())
        {
            tmp = tokenBuffer.front();
            tokenBuffer.pop_front();
        }
        return tmp;
    }
    void  pushFrontToken(const Token t1) {
        tokenBuffer.push_front(t1);
    }
    void  analysisDataFlowToTokens() {
        Token tmp;
        char cur = 0;
        int size = 0;
        while (!dataFlow.empty())
        {
            cur = dataFlow[size++];
            switch (cur)
            {
            case '0':case '1':case '2':case '3':case '4':
            case '5':case '6':case '7':case '8':case '9': {
                tmp.value.clear();
                tmp.value.append({ cur });
                while (dataFlow[size] >= '0' && dataFlow[size] <= '9')
                {
                    tmp.value.append({ dataFlow[size] });
                    size++;
                }
                tmp.type = NUM;
                break;
            }
            case '+':case '-':case '*':case '/':case '(':case ')':
                tmp.type = (TokenType)cur;
                tmp.value = cur;
                break;
            case '\0':      //结束
                tmp.type = END;
                tmp.value = "";
                tokenBuffer.push_back(tmp);     //push放在队列后面
                return;
            default:
                cout << "ERROR TOKEN!: " << cur << endl;
                exit(-9);
            }
            tokenBuffer.push_back(tmp);     //push放在队列后面
#if(DEBUG_FLAG)
            cout << tmp.value << endl;
#endif
        }

    }
    float getFactor() { //factor-> token | (expr)
        float res = 0;
        Token t1 = popFrontToken();
        switch (t1.type) {
        case NUM:
            res = stoi(t1.value);
            cout << res<<" ";
            break;
        case L_BRACE:
        {
            res = getExpression(1);
            break;
        }
        default:
            cout << "ERROR Expression!" << endl;
            exit(-5);
        }
        return res;
    }
    float getTerm() {   //term->factor/term | factor*term | factor
        float res = 0;
        res = getFactor();
        Token t2 = popFrontToken();
        switch (t2.type) {
        case MUL:           //若为乘除，则为term,继续递归计算
            res *= getTerm();
            cout << "* ";
            break;
        case DIV:
            res /= getTerm();
            cout << "/ ";
            break;
        case SUB:case ADD:case R_BRACE:      //term结束标志  "+ 、-、)"
            pushFrontToken(t2);              //把结束标志放回队列
            break;
        case END:
            break;
        default:
            cout << "ERROR Term!" << endl;
            exit(-4);
        }
        return res;
    }

    float getRest(int flag) {       //rest->+term rest |-term rest | end
        float res = 0;
        Token t = popFrontToken();
        switch (t.type) {
            case ADD:
                res += getTerm();
                cout << "+ ";
                res += getRest(flag);
                break;
            case SUB:
                res -= getTerm();
                cout << "- ";
                res += getRest(flag);
                break;
            case END:
                break;
            case R_BRACE:      
                if (!flag) {//括号不完整问题直接退出
                    cout << "ERROR EXPRESSION!" << endl;
                    exit(-55);
                }
                break;
            default:
                cout << "ERROR EXPRESSION!" << endl;
                exit(-11);
        }
        return res;
    
    }
    /*flag 用于判断迭代过程中的括号表达式（expr）的判断处理*/
    float getExpression(int flag) { //expr->term rest
        return getTerm() + getRest(flag);
    }
};
//计算器对象
class Calcluater : public Parser
{
private:
    vector<Token> tokenBuffer;      //存储一系列token的缓冲区
public:
    Calcluater(string input) : Parser{ input } {}
    void addTokenToBuffer(Token t1) {
        tokenBuffer.push_back(t1);
    }
    const Token getTokenFromBuffer(void) {
        Token tmp = tokenBuffer.at(tokenBuffer.size() - 1);
        tokenBuffer.pop_back();
        return tmp;
    }
    float CalResult(void) {
        analysisDataFlowToTokens();
        return  getExpression(0);
    }
};
#endif