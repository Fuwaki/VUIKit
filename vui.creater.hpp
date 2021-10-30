#include <string>
#include <map>
#include <concepts>
#include <vector>
#include <sstream>
#include <ranges>


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
                    rt.push_back(ss.str());
                    rt.clear();
                    ss.str("");
                    ss.clear();
                    break;
                case VUITypes::Object:
                    ss << i.first << "{" << i.second.OutSelf() << "}";
                    rt.push_back(ss.str());
                    ss.str("");
                    ss.clear();
                    break;
                case VUITypes::Array:
                    ss << i.first << "{" << i.second.OutSelf() << "}";
                    rt.push_back(ss.str());
                    ss.str("");
                    ss.clear();
                    break;
                case VUITypes::Str:
                    ss << i.first << "{\"" << i.second.OutSelf() << "\"}";
                    rt.push_back(ss.str());
                    ss.str("");
                    ss.clear();
                    break;
                default:
                    break;
                }
            }
            for (auto i: std::views::iota(0,rt.size())){
                temp.append(rt.at(i));
                if (i!=rt.size()-2){
                    temp.append(",");
                }
            }
            return temp;
        }

        /*Used to add a object as target's children*/
        int AddElement(std::string HEAD,VUIObjectElement source){
            this->BODY[HEAD] = source;
            return SUCCEDD;
        }

        /*Used to add a number as target's children*/
        template<class T> requires (std::is_arithmetic_v<T>&&!std::is_same_v<T,char>&&!std::is_same_v<T,const char>)
        int AddObject(struct VUIElement &target,T source,const char* HEAD){

            return SUCCEDD;
        }

        /*Used to add a string as target's children,types like wchar_t,char,string,wstring are supported*/
        template<class T> requires (std::is_convertible_v<T,std::string>&&(!std::is_arithmetic_v<T>||std::is_same_v<T,char>))
        int AddObject(struct VUIElement &target,T source,const char* HEAD){

            return SUCCEDD;
        }
    };
    
    struct VUIArrayElement:VUIBasicElement{
        std::vector<VUIBasicElement> BODY;
        std::string OutSelf() override{
            std::string temp;
            for (auto i:std::views::iota(0,BODY.size()))
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
    
    };
}