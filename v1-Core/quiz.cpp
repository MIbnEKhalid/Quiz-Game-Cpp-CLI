#include <iostream>
#include <fstream>
#include <string>  
using namespace std;

class quizManager
{
private:

	const static int TotalQuestions = 10;
	const static int TotalQuestionsToAsk = 10;
    // Score Per Question
    int SPQ = 3;
    float TScore_ = TotalQuestionsToAsk * SPQ;
    float Score_;
    int CorrectAnswer;
    int WrongAnswer; 

	string Questions[TotalQuestions];
	string Options[TotalQuestions][4];
	char Correct[TotalQuestions][1];
	
	void SetQuestions()
	{
        loadQuestions(0, "What is the correct syntax to include a library in C++?", "#include<lib>", "import<lib>", "#include lib", "require<lib>", 'A');
        loadQuestions(1, "What is the data type of '3.14' in C++?", "int", "float", "double", "char", 'C');
        loadQuestions(2, "Which loop is guaranteed to execute at least once?", "for loop", "while loop", "do-while loop", "none", 'C');
        loadQuestions(3, "Which of the following is used to define a class in C++?", "class", "struct", "define", "object", 'A');
        loadQuestions(4, "Which of these is not a valid access specifier in C++?", "private", "protected", "public", "secured", 'D');
        loadQuestions(5, "Which operator is used to allocate memory dynamically in C++?", "malloc", "new", "alloc", "calloc", 'B');
        loadQuestions(6, "What is the output of this code: 'int a = 5; cout << a++;'?", "4", "5", "6", "Compilation Error", 'B');
        loadQuestions(7, "What is the use of the 'virtual' keyword in C++?", "Define abstract functions", "Enable polymorphism", "Overload operators", "Allocate memory", 'B');
        loadQuestions(8, "Which of the following is a correct declaration of a pointer?", "int ptr;", "int *ptr;", "int &ptr;", "ptr int;", 'B');
        loadQuestions(9, "What does STL stand for in C++?", "Standard Template Library", "System Type Library", "Standard Tools Library", "Simple Template Library", 'A');
	}
	
	void loadQuestions(int id, string Question, string OptionA, string OptionB, string OptionC, string OptionD, char CorrectOption)
	{
		Questions[id] = Question;
		Options[id][0] = OptionA;
		Options[id][1] = OptionB;
		Options[id][2] = OptionC;
		Options[id][3] = OptionD;
		Correct[id][0] = toupper(CorrectOption); 
	}
	
	void Quiz()
	{
		cout << endl;	
		
		system("cls");
		
		for (int i = 0; i < TotalQuestionsToAsk; i++)
		{
			HandleQuestions(i);
			
			system("pause");
			system("cls");
		}
		
		cout << endl;
		
        QuizResult();
        
        system("pause");
		system("cls");
		
		MainMenu();
	}

	void HandleQuestions(int i)
	{
		string answer;
		char a = 'A';
		
		cout << i + 1 << ": " << Questions[i] << endl << endl;
		
		for (int j = 0; j < 4; j++, a++)
		{
			cout << a << ": " << Options[i][j] << endl << endl;
		}

		cout << "Enter Your Choice: ";
		cin >> answer;
		 
   	 
		 
		while (answer.length() > 1 || toupper(answer[0]) > 'D')
		{
			cout << "Invalid choice!" << endl;
			cin.clear();
			cout << "Enter your choice again: ";
			cin >> answer;
	    }

		if (toupper(answer[0]) == Correct[i][0] || tolower(answer[0]) == Correct[i][0])
		{
			cout << "Correct!";
			Score_ += SPQ;
			CorrectAnswer++;  
		}
		else
		{
			//cout << "InCorrect!";
			cout << "InCorrect! Correct Answer Is " << Correct[i][1] << endl;
            WrongAnswer++;
		}
		
		cout << endl;
		
		WriteHistory(i, answer[0], Correct[i][0]);
	}

    void WriteHistory(int i, char UserAnswer, char CorrectAns)
    {
        // Open file in append mode to avoid overwriting existing data
        ofstream MyFile("filename.txt", ios::app); 
    
        // Check if the file is open
        if (MyFile.is_open())
        {
            char a = 'A';

            // Write the question and options to the file
            MyFile << i + 1 << ": " << Questions[i] << endl;
        
            for (int j = 0; j < 4; j++, a++)
            {
                MyFile << a << ": " << Options[i][j] << endl;
            }

            // Write user and correct answers
            MyFile << "User Answer: " << UserAnswer << endl;
            MyFile << "Correct Answer: " << CorrectAns << endl << endl;
        
            // Close the file
            MyFile.close();
        }
        else
        {
            // Handle file opening failure
            cout << "Unable to open file." << endl;
        }
    }

    void WriteResult()
    {
        // Open file in append mode to avoid overwriting existing data
        ofstream MyFile("filename.txt", ios::app); 
    
        // Check if the file is open
        if (MyFile.is_open())
        {
                	float per = (Score_/TScore_)*100;

        // Score_ TScore_ WrongAnswer CorrectAnswer
        MyFile << "Result" << endl << endl;
        MyFile << "Score: " << Score_ << " / "<< TScore_  << endl;
        MyFile << "Total Correct Answers: " << CorrectAnswer << endl;
        MyFile << "Total Wrong Answers: " << WrongAnswer << endl;
        MyFile << "Percentage: " << per << "%" << endl;
        
        if(per>=40)
        {
        	MyFile << "Status: Pass" << endl;
		}
		else
		{
			MyFile << "Status: Fail" << endl;
		}
					MyFile << "\n\n------------end------------" << endl;
					MyFile << "---------------------------" << endl;

            MyFile.close();
        }
        else
        {
            // Handle file opening failure
            cout << "Unable to open file." << endl;
        }
    }


    void QuizResult()
    {
    	float per = (Score_/TScore_)*100;

        // Score_ TScore_ WrongAnswer CorrectAnswer
        cout << "Result" << endl << endl;
        cout << "Score: " << Score_ << " / "<< TScore_  << endl;
        cout << "Total Correct Answers: " << CorrectAnswer << endl;
        cout << "Total Wrong Answers: " << WrongAnswer << endl;
        cout << "Percentage: " << per << "%" << endl;
        
        if(per>=40)
        {
        	cout << "Status: Pass" << endl;
		}
		else
		{
			cout << "Status: Fail" << endl;
		}
		WriteResult(); 
	}
public:

	quizManager() : Score_(0), CorrectAnswer(0), WrongAnswer(0) 
	{
		SetQuestions();
	} 

	void MainMenu()
	{
		cout << "Source Code:   github.com/mibnekhalid/Quiz-Game-Cpp-CLI" << endl;
		cout << "         or         mbktechstudio.com/Quiz-Game-Cpp-CLI" << endl;
		cout << "Documentation: docs.mbktechstudio.com/Quiz-Game-Cpp-CLI" << endl<< endl;
    
		cout << "Welcome to the C++ Quiz Game" << endl << endl;
		cout << "1. Start Quiz" << endl;
		cout << "2. Exit" << endl;
		cout << "Enter your choice: ";
		string choice;
		cin >> choice;
		while (choice[0] < '1' || choice[0] > '2' || choice.length() > 1 )
		{
			cout << "Invalid choice!" << endl;
			cin.clear();
			cout << "Enter your choice again: ";
			cin >> choice;
		}
		if (choice == "1") { Quiz(); }
		if (choice == "2") { Exit(); }
	} 

	void Exit()
	{
		system("pause");
		system("cls");
		cout << "Developed By: " << endl;
        cout << "               Muhammad Ibn E Khalid\n" << endl << endl;
		cout << "Source Code:   github.com/mibnekhalid/Quiz-Game-Cpp-CLI" << endl;
		cout << "         or         mbktechstudio.com/Quiz-Game-Cpp-CLI" << endl;
		cout << "Documentation: docs.mbktechstudio.com/Quiz-Game-Cpp-CLI" << endl<< endl;
    }

};


int main()
{  
	quizManager qm;
	qm.MainMenu();
}
