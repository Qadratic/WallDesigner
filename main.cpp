#include<iostream>
#include<windows.h>
#include<fstream>
#include<conio.h>
#include<cstdlib>
using namespace std;

int m=500,n=500;
bool flag=1;
unsigned char wall='#';
char matrix[1000][1000];
HWND console;
RECT r;
HANDLE hOut;
CONSOLE_FONT_INFOEX cfi;
CONSOLE_CURSOR_INFO ci;

void initConst(int size){
	console = GetConsoleWindow();
	hOut=GetStdHandle(STD_OUTPUT_HANDLE);
	ci.dwSize=50;
	ci.bVisible=0;
	cfi.cbSize=sizeof(cfi);
	cfi.nFont=0;
	cfi.FontFamily=FF_DONTCARE;
	cfi.FontWeight=FW_NORMAL;
	cfi.dwFontSize.X=size;
	cfi.dwFontSize.Y=size;
	
	SetConsoleCursorPosition(hOut,{0,m+2});
	cout<<"Wall_Designer(Beta)\nby Ripudaman_Kaushik";
	SetConsoleCursorPosition(hOut,{n+2,0});
	cout<<" Press Q";
	SetConsoleCursorPosition(hOut,{n+2,1});
	cout<<" to save&quit";
}

void disp(){
	SetConsoleCursorPosition(hOut,{0,0});
	for(int i=0;i<n+2;i++){cout<<wall;}
	cout<<endl;
	for(int i=0;i<m;i++){
		cout<<wall;
		for(int j=0;j<n;j++){
			cout<<matrix[i][j];
		}
		cout<<wall;
		cout<<endl;
	}
	for(int i=0;i<n+2;i++){cout<<wall;}
}

void initPrev(string name){
	ifstream f1(name.c_str());
	if(!f1.is_open()){cout<<"cannot read from file! \nexiting program..\n\n"; exit(0);}
	int x,y;
	while(f1>>x){
		f1.get();
		f1>>y;
		matrix[x][y]=wall;
	}
}

void wallSetter(int y,int x,int button){
	x--;
	y--;
	if(x>=m||y>=n){return;}
	if(x<0||y<0){return;}
	if(button==1){
		matrix[x][y]=wall;
		SetConsoleCursorPosition(hOut,{y+1,x+1});
		cout<<wall;
	}
	if(button==2){
		matrix[x][y]=' ';
		SetConsoleCursorPosition(hOut,{y+1,x+1});
		cout<<' ';
	}
	
}

void getter(){
	
	int x,y,button=0;

	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
 
	DWORD dwPreviousMode = 0;
 	GetConsoleMode(hIn, &dwPreviousMode);
 	DWORD dwNewMode = dwPreviousMode | ENABLE_MOUSE_INPUT;
 	dwNewMode &= ~ENABLE_QUICK_EDIT_MODE;
 	SetConsoleMode(hIn, dwNewMode | ENABLE_EXTENDED_FLAGS);

 	BOOL fContinue = TRUE;
 	DWORD dwEvents;
 	INPUT_RECORD input;

	while (flag && ReadConsoleInput(hIn, &input, 1, &dwEvents) && dwEvents > 0) {
  		switch (input.EventType) {
  		case KEY_EVENT:
   		if (input.Event.KeyEvent.wVirtualKeyCode == 0x51) {
    	flag = FALSE;
   		}
   		SetConsoleMode(hIn, dwPreviousMode);
   		SetConsoleCursorPosition(hOut,{0,0});
  		case MOUSE_EVENT:
  		button=input.Event.MouseEvent.dwButtonState;
    	x=input.Event.MouseEvent.dwMousePosition.X;
    	y=input.Event.MouseEvent.dwMousePosition.Y;
		wallSetter(x,y,button);
   		break;
  		}
	}
}

int main(){
	int size;
	GetWindowRect(console, &r);
	system("cls");
	char choice,c;
	string name;

	cout<<"Welcome to the Wall_Designer(Beta)\n";
st1:cout<<"\nDo you want to edit a previous design[y/n]? ";
	choice=getch();
	if(choice=='y'){
		cout<<"\nEnter file name: ";
		getline(cin,name);
		
	}
	else if(choice=='n'){
		cout<<"\nEnter new file name: ";
		getline(cin,name);
	}
	else{
		cout<<"\nInvalid choice.";
		goto st1;
	}
	cout<<"Enter grid size[MxN]: ";
	cin>>m>>c>>n;
	cout<<"Enter pixel size: ";
	cin>>size;
	if(choice=='y'){initPrev(name);}

	system("cls");
	initConst(size);
	wcscpy(cfi.FaceName,L"Consolas");
	SetCurrentConsoleFontEx(hOut,FALSE,&cfi);
	MoveWindow(console, r.left, r.top, size*(n+22), size*(m+11), TRUE);
	SetConsoleCursorInfo(hOut,&ci);
	disp();
	getter();
	//saving
	ofstream f2(name.c_str());
	system("cls");
	if(!f2){
		cout<<"cannot open file!\n";
	}
	else{
		cout<<"Saving to file.. "<<name<<"\nDone!\npress any key to quit.";
	}
	for(int i=0;i<m;i++){
		for(int j=0;j<n;j++){
			if(matrix[i][j]==wall){
				f2<<i<<','<<j<<endl;
			}
		}
	}
	getch();
}