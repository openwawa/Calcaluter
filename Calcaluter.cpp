#include "Calcaluter.h"


int main()
{
    Calcluater* cal = new Calcluater("(3+8)-(5*2/4+(3*2+123)/89)+67"); //(3 + 8) - (5 * 2 / 4 + (3 * 2 + 123) / 89) + 67
    cout << "Input expression:" << cal->getDataFlow() << endl;
    cout << "Postfix expression:";
    cout << "\nResult:" << cal->CalResult();
    return 0;
}