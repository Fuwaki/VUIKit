#include <string>
#include <map>
#include <concepts>
#include <vector>
#include <sstream>
namespace vui{
#define SUCCEDD 0
#define ERROR_TARGET_NOT_FOUND 1
#define ERROR_UNKNOW 2
    enum class VUITypes{
        Number,
        Str,
        Object,               
        Array,               //coming soon 
        };
    struct VUIElement{
        std::string HEAD;
        std::vector<VUIElement> Children;
        VUITypes selfType;
        //bool isArrayElement=false;
        bool checkAvaliable(std::string key){
            for (auto i : Children){
                if (i.HEAD==key){
                    return false;
                }
            }
            return true;
        }
        std::string outSelf(){
            std::vector<std::string> temps;
            for (auto i:this->Children){
                temps.push_back(i.outSelf());
            }
            switch (this->selfType){
                case VUITypes::Number:{
                    std::stringstream temp("");
                    temp<<this->HEAD<<"{"<<temps.at(0)<<"}";
                    return temp.str();
                }
                case VUITypes::Str:{
                    std::stringstream temp("");
                    temp<<this->HEAD<<'{"'<<temps.at(0)<<'"}';
                    return temp.str();
                }
                case VUITypes::Object:{
                    std::stringstream temp("");
                    temp<<this->HEAD<<"{";
                    for (int j=0;j<temps.size();j++){
                        temp<<temps.at(j);
                        if(j!=temps.size()-1){
                            temp<<",";
                            //now,it isn't the last element
                        }
                    }
                    temp<<"}";
                    return temp.str();
                }
                default:{
                    break;
                }
            }
            return "";


        }
    }VUIElement;
    /*
    


    */
    class VUICreater{
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
            target.Children.push_back(temp);
            return SUCCEDD;
        }
        /*Used to add a string as target's children,types like wchar_t,char,string,wstring are supported*/
        template<class T> requires (std::is_convertible_v<T,std::string>&&(!std::is_arithmetic_v<T>||std::is_same_v<T,char>))
        int AddObject(struct VUIElement &target,T source,const char* HEAD){
            struct VUIElement temp;
            temp.selfType=VUITypes::Str;
            temp.HEAD=std::string(HEAD);
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