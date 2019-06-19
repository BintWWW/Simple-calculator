/*
Programmer: Bintao Wang
Program: bwangP7.cpp
Purpose: getting line of calculation. and calculate the result if you write in the correct line

Input: a line of calculation (e.g. 1*3.3^9/8-6=)
Output: The postfix version of the calculation, and the result of this calculation. If you didn't follow the
instruction correctly. You will get an error message saying you are putting so many floats, or you are putting
so many operators.
*/
#include <iostream>
#include <fstream>
#include <iomanip>
#include <assert.h>
#include <stack>
#include <math.h>
using namespace std;

struct node
{
    float Float;    //float number
    char Symbol;    //operator
    int weight;     //weight of operator
    node *next=NULL;
    char type;      //type of the input
};
class orz
{
    node *head;
    //node *next;
public:
    orz()
    {
        head = NULL;
    };
    void read();             //reading float and char and float and char and float ........
                            //and build a linked list putting floats in
    void input(node*);      //putting char in stack into linked list
    node readFloat();       //cin the floats
    node readSym();         //cin the operators, and define the weight of the operator by the way
    node getData();         //reading linked list
    void evaluateExpression( stack<double>&, char&, bool&,double );
    void evaluateOpr( stack<double>&, char&, bool& );
    void result(stack<double>&,bool );
    void realMain();
};


void orz::read()
{
    bool isFloat=true;          //because the third item must be a float
    stack<node*>mystack;        //build a stack

    node *newnumber;
    newnumber = new node;

    newnumber->next=NULL;
    head=newnumber;
    *newnumber=readFloat();     //input the first float

    node *newword;
    newword = new node;

    *newword=readSym();         //input the first operator
    mystack.push(newword);

    while(1)                    //have an infinite loop until there comes a '='
    {
        newword=new node;
        if(isFloat)
        {
            *newword = readFloat();     //input the following float
            input(newword);
        }
        else
        {
            *newword = readSym();       //input the following operator
            if(newword->Symbol == '=')      //it happens when there is a '=', it will terminate the loop
            {
                while(!mystack.empty())     //clearing everything remaining in the stack
                {
                    input(mystack.top());
                    mystack.pop();
                }
                node *newword;
                newword = new node;
                newword->type='S';
                newword->Symbol= '=';
                newword->next=NULL;
                input(newword);             //put in the '='
                break;
            }

            while(!mystack.empty())
            {
                if(newword->weight > mystack.top()->weight)
                {
                    mystack.push(newword);      //if the new one has higher precedent, push it to the top
                    break;
                }
                else if(newword->weight == mystack.top()->weight)
                {
                    input(mystack.top());       //put top of the stack into linked list, and pop it off
                    mystack.pop();
                    mystack.push(newword);      //push in the new operator
                    break;
                }
                else
                {
                    input(mystack.top());       //if the new one has lower that the top one
                    mystack.pop();              //get top and pop top out and do the while loop again
                }
            }
        }
        isFloat=!isFloat;               //switching whether next newword is a float or not.
    }
}

node orz::readFloat()       //reading the float only
{
    node *newfloat;
    newfloat = new node;
    newfloat->type='F';      //F means it's a float
    cin>>newfloat->Float;
    return *newfloat;
}

node orz::readSym()         //reading symbols only
{
    node *newSym;
    newSym=new node;
    newSym->type='S';      //S means it's a symbol
    cin>>newSym->Symbol;

    switch(newSym->Symbol)     //defining the weight of the symbol
    {
    case '^':
        newSym->weight=3;      //highest
        break;
    case '*':
        newSym->weight=2;      //medium
        break;
    case '/':
        newSym->weight=2;
        break;
    case '+':
        newSym->weight=1;      //lowest
        break;
    case '-':
        newSym->weight=1;
        break;
    default:
        newSym->weight=0;      //not counted
    }
    return *newSym;
}

void orz::input(node* newword)      //inputing new word
{
    node *temp;
    temp = new node;
    node *cur;
    cur = new node;
    cur = head;                     //set cur to the head
    while(cur)                      //while the cur can move to the next list
    {
        temp=cur;                   //before cur move to the next list, mark
        cur = cur->next;
    }
    temp->next=newword;             //and a newword to that markplace
}

node orz::getData()         //reading linked list
{
    node *temp;
    temp = new node;
    temp = head;
    head=head->next;
    return *temp;
}

void orz::evaluateExpression( stack<double>& stack, char& sym, bool& isExpOk,double w)
{
    double Float;
    Float = w;
   // cout<<ch<<endl;
    while (sym != '=')
    {

        if (sym =='#')
        {
            stack.push(Float);  //pushing in the floats
        }
        else
        {
            evaluateOpr( stack, sym, isExpOk);
        }

        if (isExpOk)//if no error { inpF >> ch; outF << ch;
        {
            node *newitem;
            *newitem = getData();
            if (newitem->type == 'F')
            {
                sym = '#';
                Float = newitem->Float;
            }
            else
            {
                sym = newitem->Symbol;
            }
            //cout<<ch;
        }

        else
        {
            cout<<"error";
            break;
        }
    } //end while (!= '=')
}
void orz::evaluateOpr( stack<double>& stack, char& sym, bool& isExpOk)
{
    double op1, op2;
    if (stack.empty())
    {
        cout << " (Not enough operands)";
        isExpOk = false;
    }
    else
    {
        op2 = stack.top();
        stack.pop();
        if (stack.empty())
        {
            cout << " (Not enough operands)";
            isExpOk = false;
        }
        else
        {
            op1 = stack.top();
            stack.pop();
            switch (sym)            //calculating
            {
            case '^':
                stack.push(pow(op1,op2));
                break;
            case '+':
                stack.push(op1 + op2);
                break;
            case '-':
                stack.push(op1 - op2);
                break;
            case '*':
                stack.push(op1 * op2);
                break;
            case '/':
                if (op2 != 0) stack.push(op1 / op2);
                else
                {
                    cout << " (Division by 0)";    //end evaluateOpr
                    isExpOk = false;
                }
                break;
            default:
                cout << " (Illegal operator)";
                isExpOk = false;
            }//end switch
        } //end else
    } //end else
}
void orz::result(stack<double>& stack, bool isExpOk)
{
    double result;
    if (isExpOk) //if no error, print the result
    {
        if (!stack.empty())
        {
            result = stack.top();
            stack.pop();
            if (stack.empty()) cout << "Result: " << setprecision(3) << result << endl;
            else cout << " (Error: Too many operands)" << endl;
        } //end if
        else cout << " (Error in the expression)" << endl;
    }
    else cout<< " (Error in the expression)" << endl;
    cout << "_________________________________" << endl << endl;
} //end printResult
void orz::realMain()
{
    bool expressionOk;
    float num;
    char ch;
    stack<double> stack;

    node *newitem;
    *newitem = getData();
    if (newitem->type=='F')
    {
        ch = '#';
        num = newitem->Float;
    }
    else
    {
        ch = newitem->Symbol;
    }

    expressionOk = true;
    evaluateExpression(stack, ch, expressionOk,num);
    result( stack, expressionOk);
}

int main()
{
    orz calc;
    cout<<"put in the equation: "<<endl;
    calc.read();
    calc.realMain();
    cout<<"I've abided honor code" << endl;
    return 0;
}
