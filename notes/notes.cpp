

#include <iostream>
#include <string>
#include <conio.h>
#include <fstream>
#include <Windows.h>
#include <vector>
#include <fcntl.h>
#include <io.h>
#include <stdint.h>
using namespace std;

//Goto location on console
void gotoxy(int x, int y) {
	if (x >= 0 && y >= 0) {
		COORD kursor;
		kursor.X = x;
		kursor.Y = y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), kursor);

	}
}

//draws the menu options
void menu2(int x, int y) {
	gotoxy(x-3, y-1);
	wcout << L"╔══════╗  ╔══════╗  ╔══════╗  ╔══════╗";
	gotoxy(x - 3, y);
	wcout << L"║Ctrl O║  ║Ctrl S║  ║Ctrl C║  ║Ctrl V║";
	gotoxy(x - 3, y + 1);
	wcout << L"╚══════╝  ╚══════╝  ╚══════╝  ╚══════╝";
	gotoxy(x - 1, y + 2);
	wcout << L"Open      Save      Copy      Paste";
	gotoxy(0, 0);
}

//shows the flag in terminal
void ShowConsoleCursor(bool showFlag) {
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag;
	SetConsoleCursorInfo(out, &cursorInfo);
}

//node class for list
class Node {
public:
	char letter;
	Node* next;
	Node* prev;

	//constructor
	Node() {
		next = prev = nullptr;
	}

	//constructor with argument
	//makes a node and set's its letter
	Node(char c) {
		next = prev = nullptr;
		letter = c;
	}
};

//make a copy of vector from starting node to end node
//return the starting node
Node* makeCopy(Node* p, Node* b, Node* e) {
	//create a new node and set it to starting node letter
	Node* q = new Node(b->letter);
	//if starting node isn't end node, recursivily calls the makeCopy function
	if (b != e) {
		q->next = makeCopy(q,b->next,e);
		q->prev = p;
	}
	else {
		q->prev = p;
	}
	//return the beginning node
	return q;
}

//get the last node of a list
Node* getEnd(Node* n) {
	//while next node isnt a nullptr, set node equal to the next node
	while (n->next != nullptr) {
		n = n->next;
	}
	//return the last node
	return n;
}


//driver class
int main() {
	//Used for special characters
	_setmode(_fileno(stdout), _O_U16TEXT);

	//variables
	Node* rows[10];
	Node* start;
	Node* current;
	Node* end;
	Node* hb;
	Node* he;
	Node* copy;
	char c;
	int x = 0;
	int y = 0;

	//initializing file name
	string filename = "c:\\temp\\output.txt";

	//set the color of text to white
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 15);


	//set everything to nullptr
	start = current = end = nullptr;
	hb = he = copy = nullptr;
	//open filename
	ifstream file(filename);

	//set every array node to nullptr
	for (int i = 0; i < 10; i++) {
		rows[i] = nullptr;
	}

	//line variable
	string line;

	//open input.txt and make a list of it.
	//for every line in the file
	while (getline(file,line)){
		//for every character in the line
		for (string::iterator it = line.begin(); it != line.end(); ++it) {
			//set character equal to the character of line
			c = *it;
			//increase x
			x++;

			//if starting node, make a new node
			if (start == nullptr) {
				Node* p = new Node(c);
				start = p;
				current = p;
				end = p;
				rows[y] = start;

			}
			//if adding to the beggining of a line
			else if (current == nullptr) {
				Node* p = new Node(c);
				rows[y] = p;
				start->prev = p;
				p->next = start;
				start = p;
				current = p;



			}
			//if adding to the end of a line
			else if (current->next == nullptr) {
				Node* p = new Node(c);
				end->next = p;
				p->prev = end;
				end = p;
				current = p;
			}
			//every other case
			else {
				Node* p = new Node(c);
				p->prev = current;
				p->next = current->next;
				current->next->prev = p;
				current->next = p;
				current = p;

			}
		}
		//increase y
		y++;
		start = current = end = rows[y];
		x = 0;
	}

	//clear screen
	system("cls");
	//draw menu
	menu2(45,26);
	//print the list
	for (int i = 0; i < 10; i++) {
		if (rows[i] != nullptr) {
			Node* T;
			T = rows[i];
			while (T != nullptr) {
				wcout << T->letter;
				
				T = T->next;
			}
			wcout << endl;
		}

	}
	//goto where needed
	gotoxy(x, y);
	

	//while true
	while(1){
		//set c to user's input
		c = _getch();
		
		//if pressed esc quit
		if (c == 27) {
			exit(0);
		}

		//ctrl o, open file
		else if (c == 15) {
			//open current file, save the content
			ofstream out(filename);
			for (int i = 0; i < 10; i++) {
				if (rows[i] != nullptr) {
					Node* T;
					T = rows[i];
					while (T != nullptr) {
						out << T->letter;
						T = T->next;
					}
					out << endl;
				}

			}
			//close current file
			out.close();
			//clear screen
			system("cls");
			//ask for new file location
			wcout << "File Location: ";
			//set the new file location
			cin >> filename;

			//open new file location
			ifstream file(filename);
			//if file location is invalid, ask until it is correct
			while(!(file.is_open())) {
				system("cls");
				wcout << "Unable to open file, try again.\nFile Location: ";
				cin >> filename;
				ifstream file(filename);
			}
			//clear screen
			system("cls");

			//reinitialize everything by setting nodes to nullptr and setting location to 0,0
			start = current = end = nullptr;
			hb = he = copy = nullptr;
			x = 0;
			y = 0;

			//set all array nodes to nullptr
			for (int i = 0; i < 10; i++) {
				rows[i] = nullptr;
			}

			//refer to previus file reading for further commentation.
			//reads new file and makes list from it
			string line;
			while (getline(file, line)) {
				for (string::iterator it = line.begin(); it != line.end(); ++it) {
					c = *it;
					x++;
					if (start == nullptr) {
						Node* p = new Node(c);
						start = p;
						current = p;
						end = p;
						rows[y] = start;

					}
					else if (current == nullptr) {

						Node* p = new Node(c);
						rows[y] = p;
						start->prev = p;
						p->next = start;
						start = p;
						current = p;



					}
					else if (current->next == nullptr) {
						Node* p = new Node(c);
						end->next = p;
						p->prev = end;
						end = p;
						current = p;
					}
					else {
						Node* p = new Node(c);
						p->prev = current;
						p->next = current->next;
						current->next->prev = p;
						current->next = p;
						current = p;

					}
				}
				y++;
				start = current = end = rows[y];
				x = 0;
			}
		}

		//ctrol c, makes a copy
		else if (c == 3) {
		//if first time pressing copy, set hb to the node you are at
			if (hb == nullptr and he == nullptr) {
				hb = current;
			}
			//if second time pressing copy, set he to the node you are at
			else if(hb != nullptr && he == nullptr){
				he = current;

			}
			//if third time pressing make a copy of the nodes between the previous two by calling the makeCopy function
			//set hb and he to nullptr to start again
			else if(hb!=nullptr && he!=nullptr){
				copy = makeCopy(copy, hb, he);
				hb = he = nullptr;
				
			}
		}
		
		//ctrl v, paste copied list
		else if (c == 22) {
			
			//if we have something copied
			if (copy != nullptr) {
				//if beggining of line with no text
				if (current == nullptr && end == nullptr) {
					//create a new copy of the copy and point everything to the correct locations
					Node* newCopy = nullptr;
					newCopy = makeCopy(newCopy, copy, getEnd(copy));
					start = newCopy;
					end = start;
					while (end->next != nullptr) {
						end = end->next;
						x++;
					}
					x++;
					current = end;
					rows[y] = start;
					
				}
				//if beginning of line but there is text
				else if (current == nullptr && end != nullptr) {
					//create a new copy of the copy and point everything to the correct locations
					Node* newCopy = nullptr;
					newCopy = makeCopy(newCopy, copy, getEnd(copy));
					current = newCopy;
					start->prev = getEnd(newCopy);
					while (current->next != getEnd(newCopy)) {
						current = current->next;
						x++;
					}
					x++;
					getEnd(newCopy)->next = start;
		
					start = newCopy;
					rows[y] = start;
					
				}
				//if not at the beggining of line
				else if (current != nullptr) {
					//create copy of the copy and point everything to the correct location
					Node* newCopy = nullptr;
					Node* temp;
					newCopy = makeCopy(newCopy, copy, getEnd(copy));
					temp = getEnd(newCopy);

					if (current->next != nullptr) {
						current->next->prev = getEnd(newCopy);
					}
					getEnd(newCopy)->next = current->next;
					current->next = newCopy;
					newCopy->prev = current;
					
					current = newCopy;
					while (current->next != temp) {
						current = current->next;
						x++;
					}
					current = current->next;
					end = getEnd(start);
					x+=2;
				}
			}
		}
		
		//ctrl s, save content
		else if (c == 19) {
			//open file, for every array node, get list and put it in file.
			ofstream out(filename);
			for (int i = 0; i < 10; i++) {
				if (rows[i] != nullptr) {
					Node* T;
					T = rows[i];
					while (T != nullptr) {
						out << T->letter;
						T = T->next;
					}
					out << endl;
				}

			}
			out.close();
		}

		//enter
		else if(c == 13) {
			//only allow next line if there exist content on the previous line
			if (rows[y] != nullptr) {
				//increase y and set start to that node of the array
				y++;
				start = current = end = rows[y];
				x = 0;
			}
			

		}

		//back space, delete letter
		else if(c == 8) {
			//deleting empty line
			if (end == nullptr && y!=0) {
				for (int i = y; i < 9; i++) {
					rows[i] = rows[i + 1];
				}
				y--;
				start = rows[y];
				end = start;
				while (end->next != nullptr) {
					end = end->next;
					x++;
				}
				x++;
				current = end;

			}

			//deleting first letter of line
			else if (current == start && start != nullptr) {
				Node* temp = start;
				if (start == end) {
					end = nullptr;
				}
				start = current->next;
				current = nullptr;
				rows[y] = start;
				x--;
				std::free(temp);
				
			}
			//deleting last letter of line
			else if (current == end && end!= nullptr) {
				Node* temp = current;
				current->prev->next = nullptr;
				end = current->prev;
				current = current->prev;
				x--;
				std::free(temp);
			}
			//deleting letters inbetween lines
			else if(current!=end && current!= nullptr) {
				Node* temp = current;
				current->prev->next = current->next;
				current->next->prev = current->prev;
				current = current->prev;
				x--;
				std::free(temp);
			}
			
		}

		//left, move left
		else if(c == 75) {
		//if current isnt a nullptr set current to its left, decrease x
			if (current != nullptr) {
				current = current->prev;
				x--;
				
			}
			
		}

		//right,move right
		else if(c == 77) {
			//if far left of line and there is text
			if (current == nullptr && start != nullptr) {
				current = start;
				x++;
			}
			//if current isnt far left and current right isnt a nullptr
		else if (current != nullptr && current->next != nullptr) {
			current = current->next;
			x++;
		}
		}

		//up, move up
		else if(c == 72) {
		//if not on the first line
			if (y != 0) {
				//decrease y
				y--;
				start = rows[y];
				end = start;
				if (start == nullptr) {
					current = nullptr;
					x = 0;
				}
				else {
					//get end node
					while (end->next != nullptr) {
						end = end->next;
					}

					//if x was =, set current to nullptr
					if (x == 0) {
						current = start->prev;
					}
					else {
						current = start;
						int count = 1;
						//figure out where current lands after moving up
						while (current != end && count < x) {
							current = current->next;
							count++;
						}
						x = count;

				}
				
				}
				
				
				
				
			}
		}

		//down,move down
		else if(c == 80) {
		//if not on line 10 and there exist a line under you
			if (y != 10 and rows[y+1]!=nullptr) {
				//increase y, figure out new start, end, current
				y++;
				start = rows[y];
				end = start;
				while (end->next != nullptr) {
					end = end->next;
				}

				//if x was 0, set current to nullptr
				if (x == 0) {
					current = start->prev;
				}
				else {
					//find where current lands
					current = start;
					int count = 1;
					while (current != end && count < x) {
						current = current->next;
						count++;
					}
					x = count;

				}
			}
		}

		//if input is a letter
		else if(c != -32 and c!=0) {
		//increase x to move right
			x++;
			
			//if starting node
			if (start == nullptr) {
				Node* p = new Node(c);
				start = p;
				current = p;
				end = p;
				rows[y] = start;
	
			}
			//if first letter of the line
			else if(current == nullptr) {
				
				Node* p = new Node(c);
				rows[y] = p;
				start->prev = p;
				p->next = start;
				start = p;
				current = p;

				

			}
			//if last letter of line
			else if (current->next == nullptr) {
					Node* p = new Node(c);
					end->next = p;
					p->prev = end;
					end = p;
					current = p;
				}

			//every other case
			else {
					Node* p = new Node(c);
					p->prev = current;
					p->next = current->next;
					current->next->prev = p;
					current->next = p;
					current = p;

			}

		}

		//***************refresh screen and rewrite everything*************************

		//clear screen
		system("cls");
		//redraw menu
		menu2(45,26);
		//rewrite list
		for (int i = 0; i < 10; i++) {
			if (rows[i] != nullptr) {
				Node* T;
				T = rows[i];
				while (T != nullptr) {
					if (T == hb && he != nullptr) {
						SetConsoleTextAttribute(hConsole, 103);
					}
					wcout << T->letter;
					if (T == he) {
						SetConsoleTextAttribute(hConsole, 15);
					}
					T = T->next;
				}
				wcout << endl;
			}

		}
		//goto where x y needs to be
		gotoxy(x, y);

		

	}

	



	return 0;
}

