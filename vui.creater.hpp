#include <string>
#include <map>
#include <concepts>
#include <vector>
#include <sstream>
namespace vui{
#define SUCCEDD 0
#define ERROR_TARGET_NOT_FOUND 1
#define ERROR_UNKNOW 2
/*
对于数字、字符串这种数据类型，他的HEAD由他的父亲存储，或者没有HEAD（比如在ARRAY中）
*/
    enum class VUITypes{
        Number,
        Str,
        Object,               
        Array,               //coming soon 
        };
    struct VUIBasicElement{
        VUITypes selfType=VUITypes::Object;
        virtual std::string OutSelf() =0;

    };
    struct VUIStrElement:VUIBasicElement{
        std::string BODY;
        std::string OutSelf() override{
            return BODY;
        }

    };
    
    struct VUINumElement:VUIBasicElement{
        long long BODY;
        std::string OutSelf() override{
            return std::to_string(BODY);
        }
    };
    struct VUIObjectElement:VUIBasicElement{
        std::map<std::string, VUIBasicElement> BODY;
        std::string OutSelf() override
        {
            std::stringstream ss("");
            std::vector<std::string> rt;            //means result temp
            std::string temp;
            for (auto i : BODY)
            {
                switch (i.second.selfType)
                {
                case VUITypes::Number:
                    ss << i.first << "{" << i.second.OutSelf() << "}";
                    ss >> temp;
                    rt.push_back(temp);
                    rt.clear();
                    ss.clear();
                    break;
                case VUITypes::Object:
                    ss << i.first << "{" << i.second.OutSelf() << "}";
                    ss >> temp;
                    rt.push_back(temp);
                    rt.clear();
                    ss.clear();
                    break;
                case VUITypes::Array:
                    ss << i.first << "{" << i.second.OutSelf() << "}";
                    ss >> temp;
                    rt.push_back(temp);
                    rt.clear();
                    ss.clear();
                    break;
                case VUITypes::Str:
                    ss << i.first << "{\"" << i.second.OutSelf() << "\"}";
                    ss >> temp;
                    rt.push_back(temp);
                    rt.clear();
                    ss.clear();
                    break;
                default:
                    break;
                }
            }
        }
    };
    struct VUIArrayElement:VUIBasicElement{
        std::vector<VUIBasicElement> BODY;
        std::string OutSelf() override{
            std::string temp;
            for (int i = 0; i < BODY.size(); i++)
            {
                temp.append(BODY.at(i).OutSelf());
                if (i!=BODY.size()-2){
                    temp.append(",");
                }
            }
            return temp;
        }
    };

    class VUICreater
    {
    private:
        struct VUIObjectElement ROOT;

    public:
        VUICreater(){}
        /*Used to add a object as target's children*/
        int AddObject(struct VUIElement &target,struct VUIElement source){
            target.Children.push_back(source);
            return SUCCEDD;
        }
        /*Used to add a number as target's children*/
        
        template<class T> requires (std::is_arithmetic_v<T>&&!std::is_same_v<T,char>&&!std::is_same_v<T,const char>)
        int AddObject(struct VUIElement &target,T source,const char* HEAD){
            struct VUIElement temp;
            
            temp.selfType=VUITypes::Number;
            temp.HEAD=std::string(HEAD);
            temp.Children.push_back(source);
            target.Children.push_back(temp);
            return SUCCEDD;
        }
        /*Used to add a string as target's children,types like wchar_t,char,string,wstring are supported*/
        template<class T> requires (std::is_convertible_v<T,std::string>&&(!std::is_arithmetic_v<T>||std::is_same_v<T,char>))
        int AddObject(struct VUIElement &target,T source,const char* HEAD){
            struct VUIElement temp;
            temp.selfType=VUITypes::Str;
            temp.HEAD=std::string(HEAD);
            temp.Children.push_back(source);//TODO:here is a bug
            target.Children.push_back(temp);
            return SUCCEDD;
        }
        /*
        std::string OutVUI(struct VUIElement target){
            for(auto i:target.Children){
                
            }
        }*/
    };
}