#ifndef TOOLS_HPP
#define TOOLS_HPP

#include "../../Headers/headerscpp.h"
#include "../../Headers/headersogl.h"
#include "../Handlers/ModelHandler/base_classes.h"

#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <iostream>

namespace tools
{

inline extern void FlipBool(bool &bset)
{
    if(bset)
    {
        bset=false;
    } else {
        bset=true;
    }
};

template<typename T>
inline extern std::string numberToString(T number)
{
    std::stringstream ss;
    ss << number;
    return ss.str();
};

template<typename T1,typename T2>
inline extern std::string appendStrings(T1 s1,T2 s2)
{
    std::stringstream ss;
    ss << s1 << s2;
    return ss.str();
};

template<typename T1,typename T2,typename T3>
inline extern std::string appendStrings(T1 s1,T2 s2,T3 s3)
{
    std::stringstream ss;
    ss << s1 << s2 << s3;
    return ss.str();
};

template<typename T1,typename T2,typename T3,typename T4>
inline extern std::string appendStrings(T1 s1,T2 s2,T3 s3,T4 s4)
{
    std::stringstream ss;
    ss << s1 << s2 << s3 << s4;
    return ss.str();
};

template<typename T1,typename T2,typename T3,typename T4,typename T5>
inline extern std::string appendStrings(T1 s1,T2 s2,T3 s3,T4 s4,T5 s5)
{
    std::stringstream ss;
    ss << s1 << s2 << s3 << s4 << s5;
    return ss.str();
};

template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
inline extern std::string appendStrings(T1 s1,T2 s2,T3 s3,T4 s4,T5 s5,T6 s6)
{
    std::stringstream ss;
    ss << s1 << s2 << s3 << s4 << s5 << s6;
    return ss.str();
};

template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
inline extern std::string appendStrings(T1 s1,T2 s2,T3 s3,T4 s4,T5 s5,T6 s6,T7 s7)
{
    std::stringstream ss;
    ss << s1 << s2 << s3 << s4 << s5 << s6 << s7;
    return ss.str();
};

inline extern void check_mouse_reset(GLFWwindow* window,double xpos,double ypos,double swidth,double sheight,float border)
{
    if(xpos < border)
    {
        glfwSetCursorPos(window,border,ypos);
    }
    if(xpos > swidth-border)
    {
        glfwSetCursorPos(window,swidth-border,ypos);
    }
    if(ypos < border)
    {
        glfwSetCursorPos(window,xpos,border);
    }
    if(ypos > sheight-border)
    {
        glfwSetCursorPos(window,xpos,sheight-border);
    }
};

template<typename T>
inline extern void normal_device_coordinate(T &x,T &y,int swidth,int sheight)
{
    T xmid = swidth/(T)2.0;
    T ymid = sheight/(T)2.0;

    x = (x - xmid)/xmid;
    y = -(y - ymid)/ymid;
};

template<typename T>
inline extern void pixel_device_coordinate(T &x,T &y,int swidth,int sheight)
{
    T xmid = swidth/2.0f;
    T ymid = sheight/2.0f;

    x = xmid*(1+x);
    y = ymid*(1-y);
};

template<typename T>
inline extern void NDC_set_cursor_pos(GLFWwindow* window,T &x,T &y,int swidth,int sheight)
{
    T xpp=x,ypp=y;
    pixel_device_coordinate(xpp,ypp,swidth,sheight);
    glfwSetCursorPos(window,(double)xpp,(double)ypp);
};

//using namespace boost::filesystem;
inline extern std::vector<std::string> GetFilesInDirectory(std::string directory)
{
    boost::filesystem::path p(directory);
    std::vector<std::string> rtn;

    if(boost::filesystem::is_directory(p)) {
        std::cout << p << " is a directory containing:\n";

        for(auto& entry : boost::make_iterator_range(boost::filesystem::directory_iterator(p), {}))
        {
            //std::cout << entry << "\n";
            std::stringstream ss;
            ss << entry;
            rtn.push_back(ss.str());
        }
    }

    return rtn;
};

}
#endif
