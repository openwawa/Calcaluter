#ifndef __EXPRANALYSIS_1_H__
#define __EXPRANALYSIS_1_H__
#include <iostream>
#include <vector>
#include <deque>
#include <string>

/*
计算器表达式分析1.0版本
目前实现功能为计算加减乘除带括号的整数运算。
*/

#define DEBUG_FLAG 1
using namespace std;
enum TokenType {
    NUM = '0', ADD = '+', SUB = '-', MUL = '*', DIV = '/',
    L_BRACE = '(', R_BRACE = ')', PIONT = '.', END = '!'
}; //数字、+、-、*、/、(、)、.、空


struct Token {
    string value;
    TokenType type;
};
//计算表达式字符串分析器
class InputAnysis {
private:
    string dataFlow;                //计算表达式字符串的缓冲区
    deque<Token> tokenBuffer;      //存储一系列token的队列
public:
    InputAnysis(string input) :dataFlow{ input } {}
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
    void  anysisDataFlowToTokens() {
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
                //int value=cur-'0';
                tmp.value.clear();
                tmp.value.append({ cur });
                while (dataFlow[size] >= '0' && dataFlow[size] <= '9')
                {
                    //value = value * 10 + dataFlow[size]-'0';
                    tmp.value.append({ dataFlow[size] });
                    size++;
                }
                tmp.type = NUM;
                //tmp.value = value;

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
    float getTerm() {   //Term的后面可以是数字、term、带括号的expression
        float res = 0;
        Token t1 = popFrontToken();
        switch (t1.type) {
        case NUM:
            res = stoi(t1.value);
            break;
        case L_BRACE:               //后接括号表达式
            pushFrontToken(t1);     //t1放回缓冲区，回调表达式获取函数
            res = getExpression();
            break;
        default:
            cout << "ERROR Expression!" << endl;
            exit(-5);
        }
        //判断term是否结束
        Token t2 = popFrontToken();
        switch (t2.type) {
        case MUL:           //若为乘除，则为term,继续递归计算
            res *= getTerm();
            break;
        case DIV:
            res /= getTerm();
            break;
        case SUB:case ADD:case R_BRACE:      //term结束标志  "+ 、-、)"
            pushFrontToken(t2);              //把结束标志放回队列
            break;
        case END:
            break;
        default:
            cout << "ERROR Expression!" << endl;
            exit(-4);
        }
        return res;
    }
    float getExpression() {
        float res = 0;
        Token t1 = popFrontToken();
        switch (t1.type) {
        case NUM:
            res = stoi(t1.value);
            break;
        case L_BRACE:
        {
            res = getExpression();
            for (Token t = popFrontToken(); t.type != R_BRACE; t = popFrontToken())
            {
                switch (t.type) {
                case ADD:
                    res += getExpression();
                    break;
                case SUB:
                    res -= getExpression();
                    break;
                default:
                    cout << "ERROR-》缺少左括号，表达式不完整" << endl;
                    exit(-67);
                }
            }
            break;
        }
        case END:
            break;
        default:
            cout << "ERROR EXPRESSION!" << endl;
            exit(-2);
        }
        //判断表达式是否结束
        Token t2 = popFrontToken();
        switch (t2.type) {//表明该表达式为term
        case MUL:
            res *= getTerm();    //读取Term
            break;
        case DIV:
            res /= getTerm();
            break;
        case ADD:case SUB:case END: case R_BRACE:   //表达式结尾标志，若为右括号，需要将括号放回缓冲区，
            pushFrontToken(t2);                     //右括号是给的括号表达式遍历结尾标志
            break;
        default:
            cout << "ERROR Expression!" << endl;
            exit(-3);
        }

        return res;
    }
};
//计算器对象
class Calcluater : public InputAnysis
{
private:
    vector<Token> tokenBuffer;      //存储一系列token的缓冲区
public:
    Calcluater(string input) : InputAnysis{ input } {}
    void addTokenToBuffer(Token t1) {
        tokenBuffer.push_back(t1);
    }
    const Token getTokenFromBuffer(void) {
        Token tmp = tokenBuffer.at(tokenBuffer.size() - 1);
        tokenBuffer.pop_back();
        return tmp;
    }
    float CalResult(void) {
        float res = 0;
        anysisDataFlowToTokens();
        res = getExpression();
        for (Token t = popFrontToken(); t.type != END; t = popFrontToken())
        {
            switch (t.type) {
            case ADD:
                res += getExpression();
                break;
            case SUB:
                res -= getExpression();
                break;
            case END:
                break;
            default:
                cout << "ERROR EXPRESSION!" << endl;
                exit(-11);
            }
        }
        return res;
    }
};
#endif