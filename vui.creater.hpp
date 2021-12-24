#include <vector>
#include <list>
#include <string>
#include <deque>
#include <cstring>
namespace VUI::Creater {
std::vector<long long> numList;
std::vector<std::string> strList;

enum class VUITypes {
    Number,
    Str,
    Object,
    Array,
};
class Object {
    //使用了链表的思路把每一个元素穿起来
    //一个VUI元素中应该有什么东西
public:
    //该元素的类型
    VUITypes type;
    
    //链表的前一个和后一个
    Object* last = nullptr;
    Object* next = nullptr;

    //如果该类型是Object或者Array，那么它是有一个son的
    Object* son = nullptr;
    //自己的头
    std::string head;
    //下面是数据，应该只有一个会是有数据的
    std::list<std::string>::iterator strd;
    std::list<long long>::iterator numd;
    Object() = default;
    ~Object()= default;
    
    
    };
class Manager {
private:
    //对象和数据分开储存
    //数据
    std::list<long long> numList;
    std::list<std::string> strList;
    //根对象
    Object *rootObj = new Object();
    //链表的最尾段
    Object *end = rootObj;


public:
    //添加字符串
    auto addStr(const char* HEAD, char* BODY) ->void
    {
        this->end->head = HEAD;
        this->end->type = VUITypes::Str;
        //把数据添加到管理区
        this->strList.emplace_back(BODY);
        this->end->strd = this->strList.end();

        //添加一个新的元素
        auto temp = new Object();
        this->end->next = temp;
        temp->last=this->end;
        this->end=temp;
        
    };

    //添加数字
    auto addNum(const char* HEAD, int BODY) ->void
    {
        this->end->head = HEAD;
        this->end->type = VUITypes::Number;
        //把数据添加到管理区
        this->numList.emplace_back(BODY);
        this->end->numd = this->numList.end();

        //添加一个新的元素
        auto temp = new Object();
        this->end->next = temp;
        temp->last=this->end;
        this->end=temp;
        
        
    };

    //添加列表
    auto addArray(const char* HEAD) ->void
    {
        //在列表中的时候，列表的子元素的head应该是空的
        //目前这个地方只是创建了Array，暂时还不需要把子元素放进去
        this->end->head = HEAD;
        this->end->type = VUITypes::Array;
        
        //添加一个新的元素
        auto temp = new Object();
        this->end->next = temp;
        temp->last=this->end;
        this->end=temp;
    };

    //添加子元素
    auto addObject(const char* HEAD) ->void
    {
        this->end->head = HEAD;
        this->end->type = VUITypes::Object;
        //目前这个地方也是创建，不需要添加子元素

        //添加一个新的元素
        auto temp = new Object();
        this->end->next = temp;
        temp->last=this->end;
        this->end=temp;
    };

    //获取元素
    auto getValue(const  KEY,void* Output) -> void
    {
        std::deque<const char*> list;
        // KEY示例:"fuck.a"
        for(char &c:KEY)
    };

};

}