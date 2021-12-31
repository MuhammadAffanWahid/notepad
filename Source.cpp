#include<iostream>
#include<conio.h>
#include<fstream>
#include<Windows.h>
#include<string>
#include<string.h>
using namespace std;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Total_Files = 0;
string A[1000];
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void getRowColbyLeftClick(int& rpos, int& cpos)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Events;
	INPUT_RECORD InputRecord;
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	do
	{
		ReadConsoleInput(hInput, &InputRecord, 1, &Events);
		if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
			rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			break;
		}
	} while (1);

	//cpos = cpos / 2;
}
void gotoRowCol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOuput, scrn);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Block
{
public:
	string Val = "";
	Block* Ptr = 0;
};
class Linked_List
{
private:
	Block* front = nullptr;
	Block* End = nullptr;
public:
	void Insert(string word)
	{
		Block* T = new Block;
		T->Val = word;
		T->Ptr = NULL;
		if (front == nullptr)
		{
			front = T;
			End = T;
		}
		else
		{
			End->Ptr = T;
			End = End->Ptr;
		}
	}
	void Print()
	{
		ofstream Rdr;
		Rdr.open("SaveFile.txt");

		//fstream Rdr("SaveFile.txt", ios::out / ios::in);
		Block* N;
		N = front;
		while (N != nullptr)
		{
			Rdr << N->Val << endl;
			N = N->Ptr;
		}
		//Rdr.close();
	}
	void Delete()
	{
		if (front == nullptr)
			return;
		front = front->Ptr;
	}
	int Search(string File_N)
	{
		Block* T;
		T = front;
		for (int i = 0; T != nullptr; i++)
		{
			if (T->Val == File_N)
				return true;
			else
			{
				T = T->Ptr;
			}
		}
		return false;
	}
};
Linked_List Files;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Stack
{
	char A[500] = {};
	int Capacity = 500;
public:
	int top = -1;
	int Full()
	{
		if (top == (Capacity - 1))
			return true;
		else
			return false;
	}
	void Push(char n)
	{
		//if (Full())    //to check if stack is full 
		//	return 0;
		top++;
		A[top] = n;
		//return n;
	}
	int pop()
	{
		int temp; //to store and print which number to be deleted
		if (Empty()) // to check if stack is empty
			return 0;
		temp = A[top];
		top--;
		return temp;

	}
	int Top()
	{
		return A[top];
	}
	int Empty()
	{
		if (top == -1)
			return true;
		else
			return false;
	}
	void Print(ofstream& Wrt)
	{
		for (int i = 0; i <= top; i++)
		{
			Wrt << A[i];
		}
	}
	void display()
	{
		for (int i = 0; i <= top; i++)
		{
			cout << A[i];
		}
	}
};
Stack Undo;
Stack Redo;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class NotePad
{
public:
	void Choice(ifstream& Rdr)
	{
		int choice = -1;
		int R = 0, C = 0;
		gotoRowCol(5, 80);  // interface 
		cout << "*************************************" << endl;
		gotoRowCol(6, 80);
		cout << "|             New file              |" << endl;
		gotoRowCol(7, 80);
		cout << "*************************************" << endl;
		gotoRowCol(8, 80);
		cout << "*************************************" << endl;
		gotoRowCol(9, 80);
		cout << "|            Open file              |" << endl;
		gotoRowCol(10, 80);
		cout << "*************************************" << endl;
		gotoRowCol(11, 80);
		cout << "*************************************" << endl;
		gotoRowCol(12, 80);
		cout << "|                Exit               |" << endl;
		gotoRowCol(13, 80);
		cout << "*************************************" << endl;
		while (choice == -1)
		{

			getRowColbyLeftClick(R, C); // this function is used to used mouse click
			//cout << R << " " << C;
			if ((R >= 5 && R <= 7) && (C >= 80 && C <= 136)) choice = 1;
			if ((R >= 8 && R <= 10) && (C >= 80 && C <= 136)) choice = 2;
			if ((R >= 11 && R <= 13) && (C >= 80 && C <= 136)) choice = 3;
		}
		switch (choice)	// choice to enter which function to be operate 
		{
		case 1:
			//cout << " Open the new pad\n ";
			Create_File();
			break;
		case 2:
			//cout << " Open the saved pad\n ";
			Open_File();
			break;
		case 3:
			//cout << " Exit File\n " << endl;
			Closing(Rdr);
			exit(1);
			break;
		}
	}
	void Closing(ifstream& Rdr)
	{
		Rdr.close();
		Files.Print();	// this will write the file name in "SaveFile" before closing
	}
	void Create_File()	// new File created 
	{
		system("cls");
		string file_name;
		cout << "Enter File name: ";
		cin >> file_name;				// enter the file name
		if (Files.Search(file_name.c_str()))	//this is used to search that this will name file will be Exist already or not 
		{								// search in linked list SavedFile Txt file  
			cout << "File already exists\n";
			return;
		}
		Files.Insert(file_name.c_str());
		ofstream Wrt(file_name.c_str());

		system("cls");
		Edit_File(Wrt);	// call edit File function to write the data in txt file 
		system("cls");//
		system("color 09");
		Wrt.close();
	}
	void Load_File(ifstream& Rdr)//Loads and opens the data of an existing file
	{
		char ch;
		//Rdr >> ch;
		while (!Rdr.eof())//this loop iterates till end of an existing file and loads the existing data
		{
			Rdr.get(ch);
			Undo.Push(ch);
		}
		Undo.pop();
	}
	void Open_File()	// Existing File open
	{
		system("cls");
		string file_name;
		cout << "Enter File name: ";
		cin >> file_name;
		if (!Files.Search(file_name.c_str()))//this conditional checks if the file is already present or not
		{
			cout << "File does not exists\n";//if file is not present this message would show up
			return;
		}
		ifstream Rdr(file_name.c_str());
		Load_File(Rdr);
		Rdr.close();

		ofstream Wrt(file_name.c_str());
		system("cls");
		Undo.display();
		Edit_File(Wrt);
		system("cls");
		system("color 09");
		Undo.display();

		Wrt.close();
	}
	void Edit_File(ofstream& Wrt)
	{
		system("Color F0");
		char Data;
		char c;
		while (true)
		{
			c = _getch();
			if (c == 13)	// 13 is used to move the courser to next line As Enter
			{
				cout << endl;
				Undo.Push('\n');
				continue;
			}
			if (c == 32)	//32 is used to place a space between two characters
			{
				cout << " ";
				Undo.Push(' ');
				continue;
			}
			if (c == 8) //8 is used to perform a backspace function means it will remove the previous character
			{
				Undo.pop();
				system("cls");
				Undo.display();
				continue;
			}
			if (c == 26) // 26 is used to perform the undo function 
			{
				if (Undo.top != -1)
				{
					Redo.Push(Undo.Top());
					Undo.pop();
					system("cls");
					Undo.display();
				}
				continue;
			}
			if (c == 25)	// 25 is used to perform the redo function
			{
				if (Redo.top != -1)
				{
					Undo.Push(Redo.Top());
					Redo.pop();
					system("cls");
					Undo.display();
				}
				continue;
			}
			if (c == 27)//27 is used to close the file
			{
				Undo.Print(Wrt);
				Undo.top = -1;
				Redo.top = -1;
				break;
			}

			cout << c;
			Undo.Push(c);
		}
		system("Color 07");
	}
};
void Input(ifstream& Rdr)
{
	string fn;	//a string used to store the inputs in file 
	Rdr >> fn;	//Rdr used to read and store in fn

	while (!Rdr.eof())	//a loop till end of file
	{
		Files.Insert(fn);	//insert function called
		Rdr >> fn;	//Rdr is reading and storing in fn
	}
}
int main()
{
	system("color 09");
	NotePad N;
	ifstream Rdr;
	ofstream wrtr;
	Rdr.open("SaveFile.txt"); // Save file is used to store the Existing File name 
	wrtr.open("RecentFile.txt");  // this file is used to wrt the last 5 recent files name 
	Input(Rdr);
	while (true)//a loop will continue until the terminating condition is applied
	{
		N.Choice(Rdr);
	}
	Rdr.close();

	return _getch();
}
