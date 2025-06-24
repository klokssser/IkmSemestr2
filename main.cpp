#include <iostream>
#include <string>  
#include <algorithm> 
#include <limits>    

#include "Stack.h" 

//Функция проверки пользовательского ввода
bool checkInput(const std::string& expr) {
    if (expr.empty()) throw std::invalid_argument("Введена пустая строка.");

    int counter = 0;
    for (char c : expr) {
        if (isupper(c))
            counter++;

        else if (c == '+' || c == '-' || c == '*' || c == '/') {
            // Для выполнения операции в стеке должно быть как минимум 2 операнда
            if (counter < 2) throw std::invalid_argument("Недостаточно операндов для операции.");
            // Оператор заменяет два операнда одним результатом, поэтому счетчик уменьшается на 1
            counter--;
        }
        else
            throw std::invalid_argument("В выражении присутствуют недопустимые символы.");

    }
    // В конце в стеке должен остаться ровно один элемент (результат)
    if (counter != 1) throw std::invalid_argument("Неверный баланс операндов и операторов в выражении.");
}


//Функция, которая преобразует постфиксную форму записи в последовательность инструкций
void generateInstructions() {
    std::cout << "Генерация инструкций для вычислительной машины.\n";
    std::cout << "Введите выражение в постфиксной форме (например, ABC*+DE-/): ";
    std::string expression;

    std::getline(std::cin, expression);

    // Удаляем все пробелы из строки
    expression.erase(std::remove_if(expression.begin(),
        expression.end(),
        [](unsigned char c) { return std::isspace(c); }),
        expression.end());

    // Проверяем корректность выражения
    checkInput(expression);

    Stack<std::string> operandStack;
    Stack<std::string> freeTempVarsStack; // Стек для хранения свободных временных переменных
    int tempVarCounter = 1;

    
    std::cout << "\n--- Последовательность инструкций ---\n";

    for (size_t i = 0; i < expression.length(); ++i) {
        char ch = expression[i];

        if (isupper(ch))
            operandStack.push(std::string(1, ch));

        else {
            std::string op2 = operandStack.top(); operandStack.pop();
            std::string op1 = operandStack.top(); operandStack.pop();

            // Если операнды были временными переменными, они освобождаются и могут быть использованы снова
            if (op1[0] == 'T') freeTempVarsStack.push(op1);
            if (op2[0] == 'T') freeTempVarsStack.push(op2);

            // Генерируем основные инструкции
            std::cout << "LD " << op1 << std::endl;
            switch (ch) {
            case '+': std::cout << "AD " << op2 << std::endl; break;
            case '-': std::cout << "SB " << op2 << std::endl; break;
            case '*': std::cout << "ML " << op2 << std::endl; break;
            case '/': std::cout << "DV " << op2 << std::endl; break;
            }

            // Если это не последняя операция, сохраняем результат во временную переменную
            if (i < expression.length() - 1) {
                std::string tempVar;

                if (!freeTempVarsStack.isEmpty()) {
                    tempVar = freeTempVarsStack.top();
                    freeTempVarsStack.pop();
                }
                else
                    tempVar = "T" + std::to_string(tempVarCounter++);

                std::cout << "ST " << tempVar << std::endl;
                operandStack.push(tempVar);
            }
        }
    }
    
    std::cout << "-------------------------------------\n";
}

// Основная функция, управляющая циклом повторного запуска
int main() {
    setlocale(LC_ALL, "Russian");
    char repeat = 'y';
    while (repeat == 'y' || repeat == 'Y') {
        try {
            generateInstructions();
        }
        catch (const std::invalid_argument& e) {
            std::cerr << "\n[Ошибка ввода]: " << e.what() << std::endl;
        }
        catch (const std::runtime_error& e) {
            std::cerr << "\n[Критическая ошибка]: " << e.what() << std::endl;
            std::cerr << "Входное выражение скорее всего имело неверный формат." << std::endl;
        }
        std::cout << "\nХотите запустить программу снова? (y/n): ";
        std::cin >> repeat;
        
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cout << "Программа завершена." << std::endl;
    return 0;
}
