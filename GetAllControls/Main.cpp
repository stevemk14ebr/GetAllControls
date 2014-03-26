#include <Windows.h>
#include <iostream>
#include <fcntl.h>
#include <stdio.h>
#include <io.h>
using namespace std;
int windowindex=0;
enum colour { DARKBLUE = 1, DARKGREEN, DARKTEAL, DARKRED, DARKPINK, DARKYELLOW, GRAY, DARKGRAY, BLUE, GREEN, TEAL, RED, PINK, YELLOW, WHITE };
VOID CreateConsole( )
{
    int out,in = 0; 
    HANDLE lStdHandle = 0;
    AllocConsole( );
    out = _open_osfhandle( PtrToUlong(GetStdHandle( STD_OUTPUT_HANDLE )), _O_TEXT );
	in  = _open_osfhandle(PtrToUlong(GetStdHandle(STD_INPUT_HANDLE)),  _O_TEXT);
	int modes=0;
	*stdout= *_fdopen( out, "w" ); //write mode
	*stdin=*_fdopen(in,"r");//read mode
    setvbuf( stdout, NULL, _IONBF, 0 );//set no buffering 
	
}  
void ClearScreen()
  {
  HANDLE                     hStdOut;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  DWORD                      count;
  DWORD                      cellCount;
  COORD                      homeCoords = { 0, 0 };

  hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
  if (hStdOut == INVALID_HANDLE_VALUE) return;

  /* Get the number of cells in the current buffer */
  if (!GetConsoleScreenBufferInfo( hStdOut, &csbi )) return;
  cellCount = csbi.dwSize.X *csbi.dwSize.Y;

  /* Fill the entire buffer with spaces */
  if (!FillConsoleOutputCharacter(
    hStdOut,
    (TCHAR) ' ',
    cellCount,
    homeCoords,
    &count
    )) return;

  /* Fill the entire buffer with the current colors and attributes */
  if (!FillConsoleOutputAttribute(
    hStdOut,
    csbi.wAttributes,
    cellCount,
    homeCoords,
    &count
    )) return;

  /* Move the cursor home */
  SetConsoleCursorPosition( hStdOut, homeCoords );
  }
void DrawTest(){
	COORD curpos={0,0};
	CONSOLE_SCREEN_BUFFER_INFO info;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),colour::BLUE);
	for(int i=0 ; i<=5; i++){
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&info);
		curpos=info.dwCursorPosition;
		curpos.X++;
		
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),curpos);
		cout<<"\xDC"<<"\n";
	}
	//\xDB = U+2588 FULL BLOCK
	//\xDC' = U+2584 LOWER HALF BLOCK
	//\xDF = U+2580 UPPER HALF BLOCK
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),colour::GRAY);
}
BOOL CALLBACK EnumChildCallback(HWND hwnd, LPARAM lParam) {
	HWND hButton=hwnd;
	char title[256];
	char classname[256];
	GetWindowTextA(hwnd,title,sizeof(title));
	GetClassNameA(hwnd,classname,sizeof(classname));

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),colour::GREEN);
	printf("%s %s \n","Element Type: ",classname);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),colour::GRAY);

	printf("%s %s \n","Child Window Name: ",title);	
	printf("%s %08X \n \n","--Child Window Handle: ",hwnd);
	
	//SendMessageW(hButton, BM_CLICK, 0, 0); 
	
    
	return TRUE; // must return TRUE; If return is FALSE it stops the recursion

}
BOOL CALLBACK EnumWindowCallback(HWND hwnd,LPARAM lParam){
	
	DWORD WindowProcID=0;
	GetWindowThreadProcessId(hwnd,&WindowProcID);
	if(WindowProcID==GetCurrentProcessId())
	{
		char title[256];
		GetWindowTextA(hwnd,title,sizeof(title));
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),colour::RED);
		printf("%d %s %s \n \n",windowindex," Name: ",title);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),colour::GRAY);
		windowindex++;
		EnumChildWindows(hwnd,EnumChildCallback,NULL);
		
	}
	return TRUE;
}
DWORD WINAPI LoopFunction(LPVOID LpParam){
	while (1)
	{
		if(GetAsyncKeyState(0x59))//Y
		{
			ClearScreen();
			windowindex=0;
			EnumWindows(EnumWindowCallback,NULL);
			
			Sleep(500);
		}
	}
	
}
BOOL WINAPI DllMain (HINSTANCE hModule, DWORD dwAttached, LPVOID lpvReserved)
{
    if (dwAttached == DLL_PROCESS_ATTACH) {
        
	  CreateThread(NULL,0,LoopFunction,NULL,0,NULL);
	  CreateConsole();
	  
    }
    return 1;
}