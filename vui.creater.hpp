#pragma once
#include <algorithm>
#include <iostream>
#include <string>
#include <map>
#include <concepts>
#include <utility>
#include <vector>
#include <sstream>
#include <ranges>

using namespace std;
template <typename T>  
inline void safe_delete_void_ptr(void *&target) {  
    if (nullptr != target) {  
        T* temp = static_cast<T*>(target);  
        delete temp;  
        temp = nullptr;  
        target = nullptr;  
    }  
}  




namespace vui {
typedef std::string(*OutFunc) ();
#define SUCCEDD 0
#define ERROR_TARGET_NOT_FOUND 1
#define ERROR_UNKNOW 2
class VUIBasicElement;
class VUINumElement;
class VUIStrElement;
class VUIObjectElement;
class VUIArrayElement;
    enum class VUITypes{
        Number,
        Str,
        Object,               
        Array,               //coming soon 
        };
    auto enmuTotype(void* context, VUITypes type)
    {
        switch(type){
        case VUITypes::Number:return static_cast<VUINumElement*>(context);
        case VUITypes::Object:return static_cast<VUIObjectElement*>(context);
        case VUITypes::Str:return static_cast<VUIStrElement*>(context);
        case VUITypes::Array:return static_cast<VUIArrayElement*>(context);
        default:return nullptr;
        }
    }
    
    class VUIBasicElement{
    public:
        void* Body;            //用来储存每个类型特有的东西
        VUITypes selfType;
        VUIBasicElement()
        {
            switch (this->selfType) {
                case VUITypes::Object:{
                    safe_delete_void_ptr<VUIObjectElement>(Body);
                    break;
                }
                case VUITypes::Array:{
                    safe_delete_void_ptr<VUIArrayElement>(Body);
                    break;
                }
                case VUITypes::Str:{
                    safe_delete_void_ptr<VUIStrElement>(Body);
                    break;
                }
                case VUITypes::Number:{
                    safe_delete_void_ptr<VUINumElement>(Body);
                    break;
                }
                default: {
                    std::cout<<"something perhaps already has occured"<<std::endl;
                }
            }
        };
        ~VUIBasicElement(){};
        
    };

    class VUIStrElement{
    public:
        std::string BODY = "";
        std::string OutSelf(){
            return BODY+"wndm";
        }
    };
    
    class VUINumElement{
    public:
        long long BODY;
        std::string OutSelf() {
            return std::to_string(BODY);
        }
    };
    class VUIArrayElement {
    public:
        std::vector<VUIBasicElement> BODY;
        std::string OutSelf(){
            std::string temp;
            std::for_each(BODY.begin(),BODY.end(),[&](VUIBasicElement t){decltype() a=2;});
            // for (auto i:std::views::iota(0,int(BODY.size())))
            // {
            //     temp.append(static_cast<>(BODY.at(i).Body)->OutSelf());
            //     if (i!=BODY.size()-2){
            //         temp.append(",");
            //     }
            // }
            return temp;

        }
    };


    class VUIObjectElement{
    public:
        std::map<std::string, VUIBasicElement> BODY;
        std::string OutSelf(){
            std::stringstream ss("");
            std::vector<std::string> rt;            //means result temp
            std::string temp;
            for (auto i : BODY)
            {
                switch (i.second.selfType)
                {
                case VUITypes::Number:

                    ss << i.first << "{" <<((VUINumElement*)i.second.Body)->OutSelf() << "}";
                    rt.push_back(ss.str());
                    rt.clear();
                    ss.str("");
                    ss.clear();
                    break;
                case VUITypes::Object:
                    ss << i.first << "{" << ((VUIObjectElement*)i.second.Body)->OutSelf() << "}";
                    rt.push_back(ss.str());
                    ss.str("");
                    ss.clear();
                    break;
                case VUITypes::Array:
                    ss << i.first << "{\"" << ((VUIArrayElement*)i.second.Body)->OutSelf<>() << "\"}";
                    rt.push_back(ss.str());
                    ss.str("");
                    ss.clear();
                    break;
                case VUITypes::Str:
                    ss << i.first << "{\"" << ((VUIStrElement*)i.second.Body)->OutSelf() << "\"}";
                    rt.push_back(ss.str());
                    ss.str("");
                    ss.clear();
                    break;
                default:
                    break;
                }
            }
            for (auto i: std::views::iota(0,int(rt.size()))){
                temp.append(rt.at(i));
                if (i!=rt.size()-2){
                    temp.append(",");
                }
            }
            return temp;
        }
        VUIObjectElement(VUIObjectElement& e)
        {
            std::cout << "deep copy" << endl;
            BODY=e.BODY;
        }
        /*Used to add a object as target's children*/
        int AddElement(const char* HEAD, VUIObjectElement source)
        {
            VUIBasicElement temp;
            temp.selfType = VUITypes::Object;
            temp.Body=(void *)new VUIObjectElement(source);
            this->BODY[HEAD] = temp;
            return SUCCEDD;
        }

        /*Used to add a number as target's children*/
        template<class T> requires (std::is_arithmetic_v<T>&&!std::is_same_v<T,char>&&!std::is_same_v<T,const char>)
        int AddObject(T source,const char* HEAD){
            VUINumElement temp;
            temp.BODY = long(source);
            this->BODY[HEAD] = temp;
            return SUCCEDD;
        }

        /*Used to add a string as target's children,types like wchar_t,char,string,wstring are supported*/
        template<class T> requires (std::is_convertible_v<T,std::string>&&(!std::is_arithmetic_v<T>||std::is_same_v<T,char>))
        int AddObject(T source,const char* HEAD){
            VUIStrElement temp;
            temp.BODY = std::string(source);
            this->BODY[HEAD] = temp;
            return SUCCEDD;
        }

        
    };

    
    class VUICreater
    {
    private:
        struct VUIObjectElement ROOT;

    public:
        VUICreater(){}
    
    };
}