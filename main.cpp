#pragma region Libaries&Define

#include <iostream> 
#include <windows.h> 
#include <sys/stat.h>
#include <sstream>
#include <string>
#include <iomanip>
#include <fstream>
#include <thread>
#include <algorithm>
#include <random>
#include <ctime>
#include <vector>
#include <unordered_set>

// Color constants
#define RESET   7 // White
#define RED     12 // Red
#define GREEN   10 // Green
#define YELLOW  14 // Yellow
#define BLUE    9 // Blue
#define MAGENTA 13 // Magenta
#define CYAN    11 // Cyan
#define WHITE   15 // White

using namespace std;
using namespace std::chrono;

#pragma endregion


const int NumberOfQuestionsToAsk = 5; // Number of questions to be asked

class QuizManager
{

#pragma region Data_Members

private:

	// Member variables

	//Global Variables
	const static int Total_Questions = 5; // Total Questions You Have to Set This According to Questions in Given In .CSV file
	const static int Total_Options = 4;  // TOtal Options Per Question

	// Array to store the questions
	string Question[Total_Questions];

	// 2D Array to store the options for each question
	// Each row corresponds to a question, and each column corresponds to an option
	string Option[Total_Questions][Total_Options];

	// 2D Array to store the correct answer for each question
	// Each row corresponds to a question, and the single column holds the correct answer
	string Correct[Total_Questions][1];

	// The username entered by the user
	string User_Name;

	// Total score accumulated by the user throughout the quiz
	int SCORE_ = 0;

	// Total count of correct answers provided by the user
	int CorrectAnswerCount_ = 0;

	// Total count of incorrect answers provided by the user
	int WrongAnswerCount_ = 0;

	const int Score_Per_Question = 3;

	const int Total_Score = NumberOfQuestionsToAsk * Score_Per_Question;

	steady_clock::time_point startTime;
	steady_clock::time_point endTime;

#pragma endregion


public:


	QuizManager()
	{
		LoadQuestions();
		ShuffleQuestion();
		SetZero();
	}

	void SetZero()
	{
		User_Name = "";
		SCORE_ = 0;
		CorrectAnswerCount_ = 0;
		WrongAnswerCount_ = 0;
	}


#pragma region Set_Question

	// Function to load questions and options.
	// Initializes questions and their corresponding options.

	void LoadQuestions()
	{
		Question[0] = "What is the output of the following code: std::cout << 1 + 1; ?";
		initializeOptions(0, "A: 1", "B: 2", "C: 11", "D: 3", 'B');

		Question[1] = "Which of the following is a correct comment in C++?";
		initializeOptions(1, "A: // This is a comment", "B: /* This is a comment */", "C: Both A and B", "D: None of the above", 'C');

		Question[2] = "Which of the following is used to declare a variable in C++?";
		initializeOptions(2, "A: int", "B: float", "C: double", "D: All of the above", 'D');

		Question[3] = "What is the correct syntax to output 'Hello World' in C++?";
		initializeOptions(3, "A: cout << 'Hello World';", "B: std::cout << 'Hello World';", "C: std::cout << \"Hello World\";", "D: printf('Hello World');", 'C');

		Question[4] = "Which of the following is the boolean data type in C++?";
		initializeOptions(4, "A: bool", "B: boolean", "C: int", "D: float", 'A');

	}

	// This function initializes the options for a given question and stores the correct answer.
	// Parameters:
	//   - questionIndex: Index of the question for which options are being set
	//   - optionA: The text for the first option of the question
	//   - optionB: The text for the second option of the question
	//   - optionC: The text for the third option of the question
	//   - optionD: The text for the fourth option of the question
	//   - CorrectAnswerI: The correct option (A, B, C, or D) for the question

	void initializeOptions(int questionIndex, string optionA, string optionB, string optionC, string optionD, char CorrectAnswerI)
	{
		// Set the options and correct answer for the specified question
		Option[questionIndex][0] = optionA;
		Option[questionIndex][1] = optionB;
		Option[questionIndex][2] = optionC;
		Option[questionIndex][3] = optionD;
		Correct[questionIndex][0] = CorrectAnswerI;
	}

	void ShuffleQuestion()
	{
		string Q[Total_Questions];
		string O[Total_Questions][Total_Options]; // Array to store options for each question
		string C[Total_Questions][1]; // Array to store correct answers for each question

		// Copy questions, options, and correct answers to temporary arrays
		for (int i = 0; i < Total_Questions; i++)
		{
			Q[i] = Question[i];
			for (int j = 0; j < Total_Options; j++) {
				O[i][j] = Option[i][j];
			}
			C[i][0] = Correct[i][0];
		}

		int questionIndices[Total_Questions]; // Array to store question indices
		for (int i = 0; i < Total_Questions; ++i) {
			questionIndices[i] = i; // Fill array with question indices
		}

		// Seed for random number generation based on current time
		srand(static_cast<unsigned int>(time(0)));

		// Iterate over the questionIndices array and shuffle the indices
		for (int i = Total_Questions - 1; i > 0; --i) {
			int j = rand() % (i + 1); // Generate a random index
			// Swap questionIndices[i] and questionIndices[j]
			int temp = questionIndices[i];
			questionIndices[i] = questionIndices[j];
			questionIndices[j] = temp;
		}

		// Assign shuffled questions, options, and correct answers back to the original arrays
		for (int i = 0; i < Total_Questions; i++)
		{
			Question[i] = Q[questionIndices[i]];
			for (int j = 0; j < Total_Options; j++) {
				Option[i][j] = O[questionIndices[i]][j];
			}
			Correct[i][0] = C[questionIndices[i]][0];
		}
	}

#pragma endregion


#pragma region Question_Process_Functions

	// Presents each question to the user, captures user input, validates answers, and delivers informative feedback.
	void ProcessQuestion(int questionIndex, int Qno)
	{
		SetColor(CYAN);

		// Display the question number and the question itself
		cout << "Question " << Qno << ": " << to_uppercase(User_Name) << ", " << Question[questionIndex] << endl << endl;


		for (int i = 0; i < Total_Options; ++i) {

			cout << Option[questionIndex][i] << endl << endl;

		}

		string userAnswer;
		bool validAnswer = false;

		// Loop until a valid answer is entered
		while (!validAnswer) {
			cout << "Enter your answer: ";
			cin >> userAnswer;

			if (userAnswer == "exit")
			{
				SetZero();
				clearScreen();
				MainMenu();
			}

			else if (userAnswer == "quit")
			{
				Quit();
			}

			// Convert the user answer to uppercase
			for (char& c : userAnswer) {
				c = toupper(c);
			}

			// Check if the answer is valid (single character between 'A' and 'D')
			if ((userAnswer.length() == 1 && (userAnswer[0] >= 'A' && userAnswer[0] <= 'D'))) {
				validAnswer = true;
			}
			else {
				cout << "Invalid answer." << endl;
			}
		}

		// Get the correct answer for this question
		string correctAnswer = Correct[questionIndex][0];

		// Check if it's the last question and stop the timer
		if (Qno == NumberOfQuestionsToAsk) {
			StopTimer();
		}


		if (userAnswer == correctAnswer)
		{
			// If correct, display message, update score, and increment correct answer count
			SetColor(GREEN);
			cout << "Correct answer!" << endl;
			SetColor(CYAN);
			SCORE_ = SCORE_ + Score_Per_Question;
			CorrectAnswerCount_++;
		}

		else
		{
			// If incorrect, display correct answer, update wrong answer count
			SetColor(RED);
			cout << "Incorrect answer.";
			SetColor(GREEN);
			cout << " The correct answer is: " << correctAnswer << endl;
			SetColor(CYAN);
			WrongAnswerCount_++;
		}

		// Write question details to history.txt
		WriteQuestionDetailsToHistory(questionIndex, Qno, userAnswer, correctAnswer);

		// Pause the system to display feedback and clear the screen for the next question
		system("pause");
		clearScreen();
	}


	void WriteQuestionDetailsToHistory(int questionIndex, int Qno, const string& userAnswer, const string& correctAnswer)
	{
		// Open the history file in append mode
		ofstream outFile(ReturnUserHistroyFilePath(User_Name), ios::app);

		// Check if the file is successfully opened
		if (outFile.is_open())
		{
			// If it's the first question, add a header with user name
			if (Qno == 1)
			{
				outFile << "\n-------------------------------------------\n\n\n";
				outFile << "\n\t\t\tUser Name: " << User_Name << endl << endl;
			}

			// Write the question and its options to the history file
			outFile << "Question " << Qno << ": " << Question[questionIndex] << endl;
			for (int i = 0; i < Total_Options; ++i) {
				outFile << Option[questionIndex][i] << endl;
			}

			// Write user's answer and correct answer to the history file
			outFile << "\nUser Answer: " << userAnswer << endl;
			outFile << "Correct Answer: " << correctAnswer << endl << endl << endl;



			// Close the history file
			outFile.close();
		}
		else {
			// Display an error message if the file cannot be opened
			cout << "Unable to open history.txt for writing." << endl;
		}
	}

#pragma endregion


#pragma region Question_Display_Functions


	// Initiates the quiz experience by presenting instructions and guiding users through the questions.
	void DisplayQuestions()
	{
		clearScreen(); // Clear the console


		cout << "Enter User Name: ";

		string userInput;
		bool validInput = false;

		while (!validInput) {
			getline(cin >> ws, userInput);

			if (userInput.empty() || userInput.find_first_not_of("abcdefghijklmnopqrstuvwxyz0123456789_") != string::npos) {
				// Check if the input is empty or contains invalid characters for file names
				SetColor(YELLOW);
				cout << "Invalid input. Only lower Case and numbers are allowed.\nPlease enter a valid User Name: ";
				SetColor(CYAN);
			}
			else if (IfFileExists(userInput)) {
				char choice;
				bool validChoice = false;
				while (!validChoice) {
					SetColor(13);
					cout << "User Already Exists.\nDo you want to save data in the same file? Previous Data will not be removed (y/n): ";
					SetColor(CYAN);
					cin >> choice;
					if (choice == 'y' || choice == 'Y') {
						validInput = true;
						validChoice = true;
					}
					else if (choice == 'n' || choice == 'N') {
						cout << "Enter a different User Name: ";
						validChoice = true;
					}
					else {
						SetColor(YELLOW);
						cout << "Invalid choice. Please enter 'y' for yes or 'n' for no." << endl;
						SetColor(CYAN);
					}
					// Clear the input buffer to prevent any additional characters from interfering with the next input
					cin.clear();
#pragma push_macro("max")
#undef max
					cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
#pragma pop_macro("max")
				}
			}
			else {
				// If the input is valid and the file doesn't exist, set validInput to true and exit the loop
				validInput = true;
			}
		}

		// Assign the validated user input to User_Name
		User_Name = userInput;

		checkAndCreateHistoryFile();

		// Display quiz instructions
		DisplayQuizInstruction();

		// Start the timer when the first question is displayed
		StartTimer();

		// Display questions
		for (int i = 0; i < NumberOfQuestionsToAsk; ++i) {
			ProcessQuestion(i, i + 1);
		}

		// Display quiz result
		DisplayQuizResult();
		MainMenu(); // Go back to the main menu after completing the quiz
	}

	// This function displays the quiz instructions to the user.

	void DisplayQuizInstruction()
	{

		system("pause");
		clearScreen();

        cout << "Total Questions: " << Total_Questions << endl;
        cout << "Number of Questions to Ask: " << NumberOfQuestionsToAsk << endl;
        cout << "Score Per Question: " << Score_Per_Question << endl;
        cout << "Total Score: " << Total_Score << endl;

		CentredMessage("_________________________________________");
		CentredMessage("|                                        |.");
		CentredMessage("|              Instruction               |.");
		CentredMessage("|________________________________________|.");
		CentredMessage("|                                        |.");
		CentredMessage("|  =>  Each question contain "+to_string(Score_Per_Question)+" Points.   |.");
		CentredMessage("|  =>  Total Questions = 25              |.");
		CentredMessage("|  =>  Total Scrore = 25 x 3 = 75        |.");
		CentredMessage("|  =>  Only inputs A to D are valid.     |.");
		CentredMessage("|  =>  Typing \"exit\" returns you to    |.");
		CentredMessage("|                the main menu.          |.");
		CentredMessage("|  =>  Typing \"quit\" ends the program. |.");
		CentredMessage("|________________________________________|.");


		system("pause");
		clearScreen();

	}

	void DisplayQuizResult()
	{
		SetColor(WHITE);
		clearScreen();
		rlines("=");

		CentredMessage("                        _");
		CentredMessage("                        --| |--");
		CentredMessage(" _____                  _| | ");
		CentredMessage(" |  __ \\                | | |   ");
		CentredMessage(" | |__) |___  ___ _    _| | |   ");
		CentredMessage(" |  _  // _ \\/ __|||  | | | |   ");
		CentredMessage(" | | \\ \\  __/\\__ \\||__| | | |   ");
		CentredMessage(" |_|  \\_\\___||___/||__|_|_|_|   \n\n");




		rlines("=");
		cout << endl << endl;

		//Display Result
		DisplayAnswerStats();

		//Sets the score, correct answer count, and wrong answer count to zero.

		SetZero();
		system("pause");
		clearScreen();			SetColor(GREEN);

	}

	// Function to display answer statistics to the user.
	void DisplayAnswerStats()
	{
		// Get elapsed time of the quiz
		int elapsedTime = GetElapsedTime();
		// Total score achievable in the quiz (25 questions * 2 points per question + 15 extra points)
		float spercentage = (static_cast<double>(SCORE_) / Total_Score) * 100; // Example score percentage calculation
		float cpercentage = (static_cast<double>(CorrectAnswerCount_) / NumberOfQuestionsToAsk) * 100; // Example accuracy calculation

		std::ostringstream cpercentageStream;
		cpercentageStream << std::fixed << std::setprecision(2) << cpercentage;

		std::ostringstream spercentageStream; // Fix variable name here
		spercentageStream << std::fixed << std::setprecision(2) << spercentage;

		CentredMessage("Time Elapsed: " + to_string(elapsedTime) + " Seconds");
		// Display user's score and calculate score percentage
		SetColor(MAGENTA);
		CentredMessage("Score: " + to_string(SCORE_));
		CentredMessage("Score Percentage: " + spercentageStream.str() + "%");
		CentredMessage("Correct Answer: " + to_string(CorrectAnswerCount_));
		CentredMessage("Wrong Answer: " + to_string(WrongAnswerCount_));
		// Display score percentage
		CentredMessage("Score Percentage: " + cpercentageStream.str() + "%");
		// Display pass/fail status based on score percentage

		if (spercentage >= 60)
		{
			SetColor(GREEN);
			rlines("-");
			CentredMessage("Pass");
			rlines("-");
			SetColor(WHITE);
		}

		else
		{
			SetColor(RED);
			rlines("-");
			CentredMessage("Fail");
			rlines("-");
			SetColor(WHITE);
		}

		SetColor(WHITE);
		cout << endl << endl;
		rlines("=");
		cout << endl;
		SetColor(CYAN);

		// Write quiz results to file
		WriteResultsToFile(elapsedTime, spercentage, cpercentage);
	}

	void WriteResultsToFile(int elapsedTime, int spercentage, int cpercentage)
	{

		ofstream resultFile(ReturnUserHistroyFilePath(User_Name), ios::app);
		if (resultFile.is_open())
		{
			resultFile << "\n\t\t " << User_Name << " Result \n\n";
			resultFile << "Time Elapsed: " << elapsedTime << " Seconds" << endl;
			resultFile << "Score: " << SCORE_ << endl;
			resultFile << "Score Percentage: " << spercentage << "%" << endl;
			resultFile << "Correct Answer: " << CorrectAnswerCount_ << endl;
			resultFile << "Wrong Answer: " << WrongAnswerCount_ << endl;
			resultFile << "Percentage Correct: " << cpercentage << "%" << endl;
			// Determine pass/fail status based on score percentage
			if (spercentage >= 60)
			{
				resultFile << "Result Status: Pass" << endl;
			}
			else
			{
				resultFile << "Result Status: Fail" << endl;
			}
			resultFile << "\n-------------------------------------------\n\n\n";
			resultFile << endl;
			resultFile.close();
			cout << "Results written successfully." << endl;
		}

		else
		{
			cout << "Unable to open for writing." << endl;
		}
	}


#pragma endregion


#pragma region Main_Functions

	// Function to display information about the project

	void AboutProject()
	{
		rlines("-");
		CentredMessage("About Project");
		rlines("-");

		SetColor(YELLOW);
		CentredMessage("\nProject Description:");
		cout << endl;
		cout << "This quiz game serves as a comprehensive demonstration of fundamental C++ concepts applied in real-world software development. It showcases the utilization of concepts such as classes, arrays, functions, loops, conditionals, string manipulation, file handling, and more, to create an interactive quiz experience." << endl << endl;

		SetColor(CYAN);
		CentredMessage("Purpose:");
		cout << endl;
		cout << "The primary purpose of this project is to demonstrate how basic C++ concepts can be effectively utilized to develop a fully functional quiz game. By exploring the codebase, learners can gain insights into how to structure and implement a project in C++, reinforcing their understanding of the language's core principles." << endl << endl;

		SetColor(GREEN);
		CentredMessage("Quiz Purpose:");
		cout << endl;
		cout << "The quiz game itself aims to entertain and challenge users by testing their knowledge across various subjects through a series of multiple-choice questions. It provides a fun and engaging platform for users to assess their understanding while encouraging continuous learning and exploration of different topics." << endl << endl;

		SetColor(MAGENTA);
		CentredMessage("Development:");
		cout << endl;
		cout << "The quiz game was developed using C++ programming language and leverages the Windows Console for the user interface. Throughout development, emphasis was placed on clarity, efficiency, and adherence to best coding practices." << endl << endl;

		SetColor(RED);
		CentredMessage("Credits:");
		cout << endl;
		cout << "- Programmed by Muhammad Bin Khalid" << endl;
		cout << "- Special thanks to OpenAI Model." << endl << endl;

		SetColor(GREEN);
		CentredMessage("Future Plans:");
		cout << endl;
		cout << "In the future, we plan to expand the quiz game by adding more questions across different categories, enhancing the user interface for improved usability, and incorporating additional features to enrich the gaming experience. Additionally, we aim to implement file handling functionality to allow users to save their quiz progress and load previous sessions, providing a seamless and personalized experience." << endl << endl;


		SetColor(WHITE);
		CentredMessage("\n\nWe are ending with the following quote:\n");
		SetColor(CYAN);
		CentredMessage("\"C makes it easy to shoot yourself in the foot; C++ makes it harder, but when you do it, it blows your whole leg off.\" - Bjarne Stroustrup\n\n");

		SetColor(WHITE);

		CentredMessage("Thank you for exploring our quiz game! We hope it serves as a valuable resource for learning and inspiration in your journey with C++ programming.\n\n");


		system("pause");
		clearScreen();

		SetColor(GREEN);

		MainMenu();
	}

	// Function to display the main menu and handle user input

	void MainMenu()
	{

		string optionChoice;
		SetColor(BLUE);
		rlines("-");
		CentredMessage("Source Code: https://s.mbktechstudio.com/C7-tqhC4");
		rlines("-");
		SetColor(GREEN);

		cout << endl << endl;
		rlines("-");
		CentredMessage("Welcome To Quiz Game.");
		rlines("-");
		cout << endl << endl;
		cout << "1: Start Quiz" << endl;
		cout << "2: Show Quiz Instructions" << endl;
		cout << "3: About Quiz Game Cpp CLI" << endl;
		cout << "4: Quit" << endl;
		bool validSelect = false;

		while (!validSelect) {
			cout << "Select: ";

			SetColor(WHITE);
			cin >> optionChoice;

			// Check if the answer is valid
			if ((optionChoice.length() == 1 && (optionChoice[0] >= '1' && optionChoice[0] <= '4'))) {
				validSelect = true;
			}
			else {
				SetColor(MAGENTA);
				cout << "Invalid answer." << endl;
			}
		}

		if (optionChoice[0] == '1')
		{
			DisplayQuestions();
		}

		else if (optionChoice[0] == '2')
		{
			DisplayQuizInstruction();

			SetColor(GREEN);

			MainMenu();
		}

		else if (optionChoice[0] == '3')
		{
			clearScreen();
			AboutProject();
		}

		else if (optionChoice[0] == '4')
		{
			Quit();
		}


	}


	void Quit()
	{
		clearScreen();
		CentredMessage("\nProgrammed End\n\n\n");
		rlines("=");
		CentredMessage("\nDeveloped by:                ");
		CentredMessage("                  Muhammad Ibin E Khalid\n\n");
		rlines("-");
		CentredMessage("\nSource Code: mbktechstudio.com/Quiz-Game-Cpp-CLI");
		CentredMessage("Documentation: docs.mbktechstudio.com/Quiz-Game-Cpp-CLI");
		CentredMessage("Tools Used: mbktechstudio.com/CentreTheMessageCpp");
		CentredMessage("License: MIT License\n\n");
		rlines("=");

		pause();
		exit(1);
	}


#pragma endregion


#pragma region ToolsFunctions

	string to_uppercase(const std::string& str)
	{
		std::string result = str;
		std::transform(result.begin(), result.end(), result.begin(), ::toupper);
		return result;
	}

	bool IfFileExists(const string& filename) {

		const string filepath = ReturnUserHistroyFilePath(filename);

		std::ifstream file(filepath);
		return file.good();
	}

	const string ReturnUserHistroyFilePath(const string userr)
	{
		const string filepath = "UserHistoryData/" + userr + "_history.txt"; // Convert to lowercase
		return filepath;
	}

	// Function to check if a folder exists
	bool folderExists(const std::string& folderName) {
		struct stat info;
		if (stat(folderName.c_str(), &info) != 0) {
			return false; // Folder does not exist
		}
		else if (info.st_mode & S_IFDIR) {
			return true; // Folder exists
		}
		else {
			return false; // Not a folder
		}
	}

	void checkAndCreateHistoryFile()
	{

		std::ifstream file(ReturnUserHistroyFilePath(User_Name));

		if (file.good()) {
			file.close();

		}
		else {
			// File doesn't exist, so create it
			std::ofstream newFile(ReturnUserHistroyFilePath(User_Name));
			if (newFile.is_open()) {
				// File created successfully, so close it and print message
				newFile.close();
				//	cout << "history.txt file created successfully." << endl;
			}

			else {
				// Unable to create file, print error message
				cout << "Failed to create history.txt file." << endl;
				system("pause");
				clearScreen();
			}
		}
	}

	void clearScreen()
	{

#ifdef _WIN32
		system("cls"); // For Windows
#else
		system("clear"); // For Unix-like systems
#endif
	}

	void pause()
	{

#ifdef _WIN32
		system("pause"); // For Windows
#else
		cout << "Press any key to continue . . .";// For Unix-like systems
		cin.get();
#endif
	}

	void SetConsoleFontSize(int size)
	{
		CONSOLE_FONT_INFOEX fontex;
		fontex.cbSize = sizeof(CONSOLE_FONT_INFOEX);
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		GetCurrentConsoleFontEx(hOut, 0, &fontex);
		fontex.FontWeight = 700;
		fontex.dwFontSize.X = size;
		fontex.dwFontSize.Y = size;
		SetCurrentConsoleFontEx(hOut, FALSE, &fontex);
	}

	// Start the timer
	void StartTimer() {
		startTime = steady_clock::now();
	}

	// Stop the timer
	void StopTimer() {
		endTime = steady_clock::now();
	}

	// Get the elapsed time in seconds
	int GetElapsedTime() {
		return duration_cast<seconds>(endTime - startTime).count();
	}

	// Display the timer
	void DisplayTimer(int seconds) {
		int minutes = seconds / 60;
		int remainingSeconds = seconds % 60;
		cout << "Time Elapsed: " << minutes << " minutes " << remainingSeconds << " seconds" << endl;
	}

	// Function to set text color

	void SetColor(int textColor) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, textColor | (0));
	}

	// Function to display a message centered on the console

	inline void CentredMessage(const string& textMessage)
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		int rows;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		rows = csbi.srWindow.Right - csbi.srWindow.Left + 1;

		istringstream iss(textMessage);
		string line;

		while (getline(iss, line)) {
			if (line.size() <= rows) {
				int startPosition = (rows - line.size()) / 2;
				cout << setw(startPosition + line.size()) << line << endl;
			}

			else {
				size_t middle = line.size() / 2;
				string part1 = line.substr(0, middle);
				string part2 = line.substr(middle);
				int startPositionPart1 = (rows - part1.size()) / 2;
				int startPositionPart2 = (rows - part2.size()) / 2;
				cout << setw(startPositionPart1 + part1.size()) << part1 << endl;
				cout << setw(startPositionPart2 + part2.size()) << part2 << endl;
			}
		}
	}

	inline void rlines(string op)
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		int rows;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		rows = csbi.srWindow.Right - csbi.srWindow.Left + 1;

		for (int i = 0; i < rows; i++) {
			cout << op;
		}
	}

#pragma endregion


};

// Main function

int main()
{
	QuizManager quizManager;
	const char* folderName = "UserHistoryData";

    // Use CreateDirectory to create the folder
    if (CreateDirectory(folderName, NULL)) {
    //    std::cout << "Folder created successfully: " << folderName << std::endl;
    } else {
        DWORD error = GetLastError();
        if (error == ERROR_ALREADY_EXISTS) {
        //    std::cout << "Folder already exists: " << folderName << std::endl;
        } else {
            std::cout << "Failed to create folder. Error code: " << error << std::endl;
        }
    }

    // CheckIfFolderExists 

	folderName = "UserHistoryData";
	if (!quizManager.folderExists(folderName))
	{
		quizManager.SetColor(RED);
		cout << "UserHistoryData Folder does not exist. Please create a folder named 'UserHistoryData' in the same directory as this program and rerun this program." << endl;
		quizManager.SetColor(WHITE);
		quizManager.pause();
		quizManager.Quit();
	}

	quizManager.SetColor(GREEN);
	quizManager.MainMenu();

	return 0;
}
