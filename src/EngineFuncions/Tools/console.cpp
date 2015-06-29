#include "console.h"
#include "ctime"

// Declare global static variables -- Console Class
std::vector<std::string> Console::cbuffer;
bool Console::consoleActive;

// ----------------------
// Initialize the Console
// ----------------------
/*
Arg1: pointer to the Properties class

Sets up the text printing and other
variables of the engines console.
*/
void Console::Init (Properties *props)
{
    cPrint("**Console Initialized**");

    this->props=props;
    ctext.Setup("../Fonts/FreeSans.ttf",props->WinWidth,props->WinHeight,props->FontSize);

    this->consoleActive=false;
};

// ----------------------
//    Draw the Console
// ----------------------
/*
No arguments.

If the console is active this
will draw the last 10 lines of
the console buffer to the
screen.
*/
void Console::Draw ()
{
    if (consoleActive)
    {
        int N = cbuffer.size();
        for(int i=0; i<N; ++i)
        {
            ctext.RenderTextLeftJustified(cbuffer[N-1-i],-0.95f,-0.95f+i*0.04,1.0f,glm::vec3(1.0f));
            if (i>=10)
            {
                break;
            }
        }
    }
};

// ----------------------
//   Clear the Console
// ----------------------
/*
No arguments.

First this dumps all of the
console into a log file, then
cleans up the ScreenWriter
and clears cbuffer.
*/
void Console::Clear()
{
    //Dump Console Function
    std::cout << "Console Cleanup..." << std::endl;
    DumpConsole(false);

    // Cleanup
    ctext.Cleanup();
    cbuffer.clear();
};

//*******************
// Static Functions
//*******************
// ----------------------
//   Dump the Console
// ----------------------
/*
Arg 1: true=error;false=no error

This function is called by this
classes cleanup function. It dumps
all of cbuffer into:

        bin/Logs/cDump.dat

It also prints a time stamp at the
top of the file.
*/
void Console::DumpConsole(bool errchk)
{
    std::cout << "Dumping Console to \"bin/Logs/cDump.dat\"" << std::endl;

    std::ofstream cDump;
    cDump.open("../Logs/cDump.dat");

    time_t ct = time(0);
    struct tm* now = localtime(&ct);

    cDump << "Time and Date: ";
    cDump << (now->tm_mday) << "-";
    cDump << (now->tm_mon+1) << "-";
    cDump << (now->tm_year+1900) << " ";
    cDump << (now->tm_hour) << ":";
    cDump << (now->tm_min) << ":";
    cDump << (now->tm_sec) << "\n";

    for (auto&& line : cbuffer)
    {
        cDump << line << std::endl;
    }

    if (errchk)
    {
        cDump << "***Error: Abnormal Termination***" << std::endl;
    } else {
        cDump << "***Normal Termination***" << std::endl;
    }

    cDump.close();
};

//Prints a line the the console buffer
void Console::cPrint(std::string line)
{
    cbuffer.push_back("> ");
    cbuffer.back().append(line);
};

//Prints a line the the console buffer with blank before
void Console::cPrint(bool spacef,std::string line)
{
    cbuffer.push_back(" ");
    cbuffer.push_back("> ");
    cbuffer.back().append(line);
};

//Prints a line the the console buffer with blank after
void Console::cPrint(std::string line,bool spaceb)
{
    cbuffer.push_back("> ");
    cbuffer.back().append(line);
    cbuffer.push_back(" ");
};

//Prints a line the the console buffer with blank before and after
void Console::cPrint(bool spacef,std::string line,bool spaceb)
{
    cbuffer.push_back(" ");
    cbuffer.push_back("> ");
    cbuffer.back().append(line);
    cbuffer.push_back(" ");
};

//Update input into the console
void Console::Update(bool F12)
{
    if(F12)
    {
        consoleActive = !consoleActive;
    }
};
