#include <cmath>
#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

vector<string> function{ "sin", "cos", "tg", "ctg", "exp", "sqrt" };
string possible_elements = "^*/+-().1234567890sincotgexpqrX";

void OperandChecker(string& task, char op)
{
    if (task.find(op) == 0)
    {
        cout << "Invalid expression, no left operand found for operator " << op << endl;
        task = "(!)";
    }
    else
    {
        int spot = task.find(op);
        if (task[spot - 1] < '0' or task[spot - 1] > '9')
        {
            cout << "Invalid expression, no left operand found for operator " << op << endl;
            task = "(!)";
        }
        if ((task.length() - 1) - task.find(op) >= 1)
        {
            int space = (task.length() - 1) - task.find(op);
            if (space == 1 and (task[spot + 1] < '0' or task[spot + 1] > '9'))
            {
                    cout << "Invalid expression, no right operand found for operator " << op << endl;
                    task = "(!)";
            }
        }
        else
        {
            cout << "Invalid expression, no right operand found for operator " << op << endl;
            task = "(!)";
        }
    }
}

void UnknownElementChecker(string& task)
{
    string copy = task;
    for (unsigned int i = 0; i < 6; i++)
    {
        while (copy.find(function[i]) != string::npos)
        {
            copy.erase(copy.find(function[i]), function[i].length());
        }
    }
    while (copy.find('X') != string::npos)
    {
        copy.erase(copy.find('X'), 1);
    }
    for (unsigned int i = 18; i < 30; i++)
    {
        if (copy.find(possible_elements[i]) != string::npos)
        {
            cout << "Unknown element detected" << endl;
            task = "(!)";
        }
    }
}

bool UnknownChar(string& task)
{
    bool UnknownCharIsFound = true;
    for (unsigned int i = 0; i < task.length() and UnknownCharIsFound; i++)
    {
        for (unsigned int j = 0; j < possible_elements.size() and UnknownCharIsFound; j++)
        {
            if (task[i] == possible_elements[j]) UnknownCharIsFound = false;
        }
    }
    if (UnknownCharIsFound) return true;
    else return false;
}

void ParameterChecker(string& task)
{
    if (task.find("X") != string::npos)
    {
        bool IsWrongPosL = true;
        bool IsWrongPosR = true;
        int spot = task.find("X");
        for (unsigned int i = 0; i < 7; i++)
        {
            if (spot != 0)
            {
                if (task[spot - 1] == possible_elements[i]) IsWrongPosL = false;
            }
            else IsWrongPosL = false;
            if (spot != task.length() - 1)
            {
                if (task[spot + 1] == possible_elements[i]) IsWrongPosR = false;
            }
            else IsWrongPosR = false;
        }
        if (IsWrongPosL or IsWrongPosR)
        {
            cout << "Invalid position of parameter X" << endl;
            task = "(!)";
        }
        else
        {
            string X;
            cout << "Enter X: ";
            getline(cin, X);
            while (task.find("X") != string::npos)
            {
                task.replace(task.find("X"), 1, X);
            }
        }
    }
}

void SpaceEraser(string& task)
{
    for (unsigned int i = 0; i < task.size(); i++)
    {
        if (task[i] == ' ')
        {
            task.erase(i, 1);
            i--;
        }
    }
}

void Polisher(string& task)
{
    if (task != "(!)")
    {
        if (task.find(".") != string::npos)
        {
            string fraction = task.substr(task.find("."), task.length() - task.find("."));
            while (fraction[fraction.length() - 1] == '0')
            {
                fraction.pop_back();
            }
            if (fraction.length() == 1) task = task.substr(0, task.find("."));
            else task = task.replace(task.find("."), task.length() - task.find("."), fraction);
        }
        cout << "Result: " << task << endl;
    }
}

void BracketsChecker(string& task)
{
    if (task != "(!)")
    {
        int Left = 0;
        int Right = 0;
        for (unsigned int i = 0; i <= task.length(); i++)
        {
            if (task[i] == '(') Left++;
            if (task[i] == ')') Right++;
            if (Right > Left)
            {
                cout << "Wrong quantity of brackets" << endl;
                task = "(!)";
                break;
            }
        }
        if (Right != Left and task != "(!)")
        {
            cout << "Wrong quantity of brackets" << endl;
            task = "(!)";
        }
    }
}

void BracketInfo(string& task, unsigned int& Rspot)
{
    unsigned int OpenBracket = 1;
    unsigned int CloseBracket = 0;
    while (OpenBracket != CloseBracket)
    {
        Rspot++;
        if (task[Rspot] == '(') OpenBracket++;
        if (task[Rspot] == ')')
        {
            CloseBracket++;
        }
    }
}

void ConstantsChecker(string& task)
{
    const double pi = 3.1415926535897932384626;
    const double e = 2.7182818284590452353602;
    unsigned int spot = 0;
    while (task.find("pi") != string::npos)
    {
        task.replace(task.find("pi"), 2, std::to_string(pi));
    }
    while (task.find("e", spot) != string::npos)
    {
        if (task.find("e", spot) == task.find("exp", spot))
        {
            spot = task.find("exp", spot) + 1;
        }
        else task.replace(task.find("e", spot), 1, std::to_string(e));
    }
}

void OperandReduction(string& task, char op)
{
    if (task != "(!)")
    {
        string left_value;
        string right_value;
        int Lspot = task.find(op) - 1;
        int Rspot = task.find(op) + 1;
        while (Lspot >= 0 and ((task[Lspot] >= '0' and task[Lspot] <= '9') or task[Lspot] == '.'))
        {
            left_value.insert(left_value.begin(), task[Lspot]);
            Lspot--;
        }
        if (Lspot == 0)
        {
            if (task[Lspot] == '-' or task[Lspot] == 'M')
            {
                left_value.insert(left_value.begin(), '-');
                Lspot--;
            }
        }
        if (task[Rspot] == 'M' or task[Rspot] == '-')
        {
            task.erase(Rspot, 1);
            right_value.push_back('-');
        }
        while (Rspot <= task.size() - 1 and ((task[Rspot] >= '0' and task[Rspot] <= '9') or task[Rspot] == '.'))
        {
            right_value.push_back(task[Rspot]);
            Rspot++;
        }
        if (left_value.find('.') == 0 or left_value.find('.') == left_value.size() - 1)
        {
            cout << "Invalid expression, no left operand found for operator " << op << endl;
            task = "(!)";
        }
        if (right_value.find('.') == 0 or right_value.find('.') == right_value.size() - 1)
        {
            cout << "Invalid expression, no right operand found for operator " << op << endl;
            task = "(!)";
        }
        if (task != "(!)")
        {
            double L = stod(left_value);
            double R = stod(right_value);
            double D = 0;
            switch (op)
            {
            case '+':
                D = L + R;
                break;
            case '-':
                D = L - R;
                break;
            case '*':
                D = L * R;
                break;
            case '/':
                D = L / R;
                break;
            case '^':
                D = pow(L, R);
                break;
            }
            task.replace(Lspot + 1, Rspot - Lspot - 1, std::to_string(D));
        }
    }
}

void Simplifyer(string& task)
{
    while (task.find('^') != string::npos)
    {
        OperandChecker(task, '^');
        if (task != "(!)") OperandReduction(task, '^');
    }
    while (task.find('*') != string::npos or task.find('/') != string::npos)
    {
        if (task.find('*') < task.find('/'))
        {
            OperandChecker(task, '*');
            if (task != "(!)") OperandReduction(task, '*');
        }
        else if (task.find('/') < task.find('*'))
        {
            OperandChecker(task, '/');
            if (task != "(!)") OperandReduction(task, '/');
        }
    }
    while (task.find('+') != string::npos or task.find('-') != string::npos)
    {
        if (task.find('+') < task.find('-'))
        {
            OperandChecker(task, '+');
            if (task != "(!)") OperandReduction(task, '+');
        }
        else if (task.find('-') < task.find('+'))
        {
            if (task.find('-') == 0)
            {
                if (task[1] == '-') task.erase(0, 2);
                else task[0] = 'M';
            }
            else OperandReduction(task, '-');
        }
    }
    if (task[0] == 'M') task[0] = '-';
}

void SubtaskChecker(string& task)
{
    if (task.find('(') != string::npos and task != "(!)")
    {
        unsigned int Lspot = task.find('(');
        unsigned int Rspot = Lspot;
        BracketInfo(task, Rspot);
        string subtask = task.substr(Lspot + 1, Rspot - Lspot - 1);
        SubtaskChecker(subtask);
        if (subtask == "(!)")
        {
            task = "(!)";
        }
        else
        {
            task = task.replace(Lspot, Rspot - Lspot + 1, subtask);
            if (task.find('(') != string::npos) SubtaskChecker(task);
        }
        Simplifyer(task);
    }
    else Simplifyer(task);
}

void FunctionsChecker(string& task)
{
    if (task != "(!)")
    {
        for (unsigned int i = 0; i < 6; i++)
        {
            while (task.find(function[i]) != string::npos)
            {
                if (task.find(function[i] + '(') == string::npos)
                {
                    cout << "No argument for " << function[i] << " found" << endl;
                    task = "(!)";
                    break;
                }
                unsigned int Lspot = task.find(function[i]) + function[i].length();
                unsigned int Rspot = Lspot;
                BracketInfo(task, Rspot);
                string functask = task.substr(Lspot + 1, Rspot - Lspot - 1);
                if (functask.length() == 0)
                {
                    cout << "No argument for " << function[i] << " found" << endl;
                    task = "(!)";
                    break;
                }
                FunctionsChecker(functask);
                SubtaskChecker(functask);
                double D = 0;
                switch (i)
                {
                case 0:
                    D = sin(stod(functask));
                    break;
                case 1:
                    D = cos(stod(functask));
                    break;
                case 2:
                    D = tan(stod(functask));
                    break;
                case 3:
                    D = 1 / tan(stod(functask));
                    break;
                case 4:
                    D = exp(stod(functask));
                    break;
                case 5:
                    D = stod(functask);
                    D = pow(D, 0.5);
                default:
                    break;
                }
                task = task.replace(task.find(function[i]), Rspot - task.find(function[i]) + 1, std::to_string(D));
            }
        }
    }
}

int main()
{
    string Command, Task;
    cout << "Commands list" << endl << "1. Calculate" << endl << "2. Exit" << endl;
    cout << "Command: ";
    getline(cin, Command);
    if (Command != "1" and Command != "2") Command = "3";
    while (stoi(Command) != 2)
    {
        switch (stoi(Command))
        {
        case 1:
            cout << "Enter the expression: ";
            getline(cin, Task);
            SpaceEraser(Task);  //Удаляет пробелы
            if (UnknownChar(Task))
            {
                cout << "Unknown symbol detected" << endl;
            }
            else
            {
                BracketsChecker(Task);  //Проверяет, правильно ли стоят скобки
                ConstantsChecker(Task);  //Проверяет на наличие констант и заменяет их на числа
                UnknownElementChecker(Task);  
                ParameterChecker(Task);  //Проверяет наличие параметра, запрашивает значение и подставляет его
                FunctionsChecker(Task);  //Ищет функции и считает их
                SubtaskChecker(Task);  //Ищет подзадачи(выражения в скобках) и их решает
                Polisher(Task);  //Небольшой костыль ¯\_(ツ)_/¯
            }
            break;
        default:
            cout << "Wrong command!" << endl;
            cout << "Commands list" << endl << "1. Calculate" << endl << "2. Exit" << endl;
            break;
        }
        cout << "Command: ";
        getline(cin, Command);
        if (Command != "1" and Command != "2") Command = "3";
    }
}