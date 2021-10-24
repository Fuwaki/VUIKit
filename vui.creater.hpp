#include <string>
#include <map>
#include <any>
#include <concepts>
namespace vui{
#define SUCCEDD 0
#define ERROR_TARGET_NOT_FOUND 1
#define ERROR_UNKNOW 2
    enum class VUITypes{
        Number,
        Str,
        Object,               
        Array               //Coming soon
        };
    struct VUIElement{
        VUITypes selfType;
        std::map<std::string,std::any> children;
        
    }VUIElement;
    class VUICreater{
        VUICreater(){}
        /*Used to add a object as target's children*/
        int AddObject(struct VUIElement &target,struct VUIElement source,const char* HEAD){
            target.children.insert(std::make_pair(std::string(HEAD),source));
            return SUCCEDD;
        }
        /*Used to add a number as target's children*/
        template<class T> requires (std::is_arithmetic_v<T>&&!std::is_same_v<T,char>&&!std::is_same_v<T,const char>)
        int AddObject(struct VUIElement &target,T source,const char* HEAD){
            struct VUIElement temp;
            temp.selfType=VUITypes::Number;
            temp.children.insert(std::make_pair(std::string(HEAD),source))
            target.children.insert(std::make_pair(std::string(HEAD),temp));
        }
        /*Used to add a string as target's children,types like wchar_t,char,string,wstring are supported*/
        template<class T> requires (std::is_convertible_v<T,std::string>&&(!std::is_arithmetic_v<T>||std::is_same_v<T,char>))
        int AddObject(struct VUIElement &target,T source,const char* HEAD){
            struct VUIElement temp;
            temp.selfType=VUITypes::Str;
            temp.children.insert(std::make_pair(std::string(HEAD),source))
            target.children.insert(std::make_pair(std::string(HEAD),temp));
        }

        std::string OutVUI(struct VUIElement target){
            for(auto i:target.children){
                
            }
        }

    };
}