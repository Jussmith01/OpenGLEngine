#include "console.h"

std::vector<std::string> Console::cbuffer;

void Console::Init (Properties *props)
{
    cPrint(tools::appendStrings("**Console Initialized** ",2));

    this->props=props;
    ctext.Setup("../Fonts/FreeSans.ttf",props->WinWidth,props->WinHeight,props->FontSize);
};

void Console::Draw ()
{
    int N = cbuffer.size();
    for(int i=0;i<N;++i)
    {
        ctext.RenderTextLeftJustified(cbuffer[N-1-i],-0.95f,-0.95f+i*0.04,1.0f,glm::vec3(1.0f));
        if (i>=10) {break;}
    }
};

void Console::Clear ()
{
    //Write a dump function
    ctext.Cleanup();
    cbuffer.clear();
};

void Console::cPrint(std::string line)
{
    cbuffer.push_back("> ");
    cbuffer.back().append(line);
};
