#ifndef __EXPRANALYSIS_1_H__
#define __EXPRANALYSIS_1_H__
#include <iostream>
#include <vector>
#include <deque>
#include <string>

/*
���������ʽ����1.0�汾
Ŀǰʵ�ֹ���Ϊ����Ӽ��˳������ŵ��������㡣
*/

#define DEBUG_FLAG 1
using namespace std;
enum TokenType {
    NUM = '0', ADD = '+', SUB = '-', MUL = '*', DIV = '/',
    L_BRACE = '(', R_BRACE = ')', PIONT = '.', END = '!'
}; //���֡�+��-��*��/��(��)��.����


struct Token {
    string value;
    TokenType type;
};
//������ʽ�ַ���������
class InputAnysis {
private:
    string dataFlow;                //������ʽ�ַ����Ļ�����
    deque<Token> tokenBuffer;      //�洢һϵ��token�Ķ���
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
            case '\0':      //����
                tmp.type = END;
                tmp.value = "";
                tokenBuffer.push_back(tmp);     //push���ڶ��к���
                return;
            default:
                cout << "ERROR TOKEN!: " << cur << endl;
                exit(-9);
            }
            tokenBuffer.push_back(tmp);     //push���ڶ��к���
#if(DEBUG_FLAG)
            cout << tmp.value << endl;
#endif
        }

    }
    float getTerm() {   //Term�ĺ�����������֡�term�������ŵ�expression
        float res = 0;
        Token t1 = popFrontToken();
        switch (t1.type) {
        case NUM:
            res = stoi(t1.value);
            break;
        case L_BRACE:               //������ű��ʽ
            pushFrontToken(t1);     //t1�Żػ��������ص����ʽ��ȡ����
            res = getExpression();
            break;
        default:
            cout << "ERROR Expression!" << endl;
            exit(-5);
        }
        //�ж�term�Ƿ����
        Token t2 = popFrontToken();
        switch (t2.type) {
        case MUL:           //��Ϊ�˳�����Ϊterm,�����ݹ����
            res *= getTerm();
            break;
        case DIV:
            res /= getTerm();
            break;
        case SUB:case ADD:case R_BRACE:      //term������־  "+ ��-��)"
            pushFrontToken(t2);              //�ѽ�����־�Żض���
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
                    cout << "ERROR-��ȱ�������ţ����ʽ������" << endl;
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
        //�жϱ��ʽ�Ƿ����
        Token t2 = popFrontToken();
        switch (t2.type) {//�����ñ��ʽΪterm
        case MUL:
            res *= getTerm();    //��ȡTerm
            break;
        case DIV:
            res /= getTerm();
            break;
        case ADD:case SUB:case END: case R_BRACE:   //���ʽ��β��־����Ϊ�����ţ���Ҫ�����ŷŻػ�������
            pushFrontToken(t2);                     //�������Ǹ������ű��ʽ������β��־
            break;
        default:
            cout << "ERROR Expression!" << endl;
            exit(-3);
        }

        return res;
    }
};
//����������
class Calcluater : public InputAnysis
{
private:
    vector<Token> tokenBuffer;      //�洢һϵ��token�Ļ�����
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