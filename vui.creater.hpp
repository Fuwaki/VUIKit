#include <cstdio>
#include <cstring>
#include <deque>
#include <functional>
#include <list>
#include <memory>
#include <string>
#include <iostream>
#include <vector>
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
    //是不是第一个
    int _isFirst; // 0未检测，1是，2不是

    //如果该类型是Object或者Array，那么它是有一个son的
    Object* son = nullptr;
    //自己的头
    std::string head;
    //下面是数据，应该只有一个会是有数据的
    std::string *strd;
    long long *numd;
    //向前遍历，直到自己这个节点延伸下去，到的最尾端
    void foreach (const std::function<void(Object*)>& func)
    {
        Object* now;
        now = this;
        while (now->next != nullptr) {
            func(now);
            //向前推
            now = now->next;
        }
    }
    auto isFirst() -> bool
    {
    begin:
        switch (this->_isFirst) {
        case 0:
            _isFirst = (last == nullptr) ? 1 : 2;
            goto begin;
        case 1:
            return true;
        case 2:
            return false;
        }
    }
    Object() = default;
    ~Object() = default;
};
//函数声明区
auto strOutFunction(Object* in) -> std::string;
auto numOutFunction(Object* in) -> std::string;
auto arrayOutFunction(Object* in) -> std::string;
auto objOutFunction(Object*) -> std::string;
auto addHead(const char* head, const char* body) -> std::string;

auto strOutFunction(Object* in) -> std::string
{
    return std::string("\"").append(in->strd->c_str()).append("\"");
}
auto numOutFunction(Object* in) -> std::string
{
    return std::to_string(*in->numd);
}
//输出数组的文本的函数的in参数应该是数组的第一个节点
auto arrayOutFunction(Object* in) -> std::string
{
    std::string result;
    in->foreach ([&](Object* now) {
        switch (now->type) {
        case VUITypes::Array: {
            result.append(arrayOutFunction(now));
            break;
        }
        case VUITypes::Number: {
            result.append(numOutFunction(now));
            break;
        }
        case VUITypes::Str: {
            result.append(strOutFunction(now));
            break;
        }
        case VUITypes::Object: {
            result.append(objOutFunction(now));
            break;
        }
        }
        //要不要在后面加一个逗号？
        //最后一个是空的，所以应该是倒数第二才是有效的
        if (now->next->next != nullptr) {
            result.append(",");
        }
    });
    return result;
}
auto objOutFunction(Object* in) -> std::string
{
    std::string result;
    in->foreach ([&](Object* now) {
        switch (now->type) {
        case VUITypes::Array: {
            result.append(addHead(now->head.c_str(),arrayOutFunction(now).c_str()));
            break;
        }
        case VUITypes::Number: {
            result.append(addHead(now->head.c_str(),numOutFunction(now).c_str()));
            break;
        }
        case VUITypes::Str: {
            result.append(addHead(now->head.c_str(),strOutFunction(now).c_str()));
            break;
        }
        case VUITypes::Object: {
            result.append(addHead(now->head.c_str(),objOutFunction(now).c_str()));
            break;
        }
        }
        //要不要在后面加一个逗号？
        if (now->next->next != nullptr) {
            result.append(",");


        }
    });
    return result;
}
//会为out创建控件，所以out输入时只需是个空指针
auto addHead(const char* head, const char* body) -> std::string
{
    char buf[strlen(head) + strlen(body) + 2];
    sprintf(buf, "%s{%s}", head, body);
    return std::string(buf);
}
class Manager {
private:
    //对象和数据分开储存
    //数据
    std::list<long long> numList;
    std::list<std::string> strList;
    //根对象,也就是链表的头部
    Object* rootObj = new Object();
    //链表的最尾段
    Object* end = rootObj;

public:
    //添加字符串
    auto addStr(const char* HEAD, char* BODY) -> void
    {
        this->end->head = HEAD;
        this->end->type = VUITypes::Str;
        //把数据添加到管理区
        this->strList.emplace_back(BODY);
        this->end->strd = &this->strList.back();

        //添加一个新的元素
        auto temp = new Object();
        this->end->next = temp;
        temp->last = this->end;
        this->end = temp;
    };

    //添加数字
    auto addNum(const char* HEAD, long long BODY) -> void
    {
        this->end->head = HEAD;
        this->end->type = VUITypes::Number;
        //把数据添加到管理区
        this->numList.emplace_back(BODY);
        this->end->numd = &this->numList.back();

        //添加一个新的元素
        auto temp = new Object();
        this->end->next = temp;
        temp->last = this->end;
        this->end = temp;
    };

    //添加列表
    auto addArray(const char* HEAD) -> void
    {
        //在列表中的时候，列表的子元素的head应该是空的
        //目前这个地方只是创建了Array，暂时还不需要把子元素放进去
        this->end->head = HEAD;
        this->end->type = VUITypes::Array;

        //添加一个新的元素
        auto temp = new Object();
        this->end->next = temp;
        temp->last = this->end;
        this->end = temp;
    };

    //添加子元素
    auto addObject(const char* HEAD) -> void
    {
        this->end->head = HEAD;
        this->end->type = VUITypes::Object;
        //目前这个地方也是创建，不需要添加子元素

        //添加一个新的元素
        auto temp = new Object();
        this->end->next = temp;
        temp->last = this->end;
        this->end = temp;
    };

    //输出
    auto outString() -> std::string
    {
        return objOutFunction(this->rootObj);
    };

    ~Manager()
    {
        //释放数据区内存
        numList.erase(numList.begin());
        numList.clear();
        strList.erase(strList.begin());
        strList.clear();
        //释放对象内存
        Object* temp;
        temp = end;
        while (temp->next != nullptr) {
            //向前面移
            temp = temp->next;
            delete temp->last;
        }
    }
};

}