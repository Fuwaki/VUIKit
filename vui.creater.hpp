#include <cstdio>
#include <cstring>
#include <cwchar>
#include <deque>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <regex>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
// TODO:提高代码复用性]
// TODO:使用更多c函数，提高速度
namespace VUI::Creater {


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
    std::wstring head;
    //下面是数据，应该只有一个会是有数据的
    std::wstring *data;
    //遍历自己的子元素，到的子链表最尾端,函数返回true停止，true继续
    void foreach (const std::function<bool(Object*)>& func)
    {
        Object* now;
        now = this->son;
        bool status=true;
        while(status) {
            if (func(now)) {
                break;
            };
            if (now->next == nullptr) {
                status = false;
            }
            if (status) {
                now = now->next;
            }
        
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
    auto setData(std::wstring* d) -> void { this->data = d; };
    //这个函数只会拷贝一个空壳，因为元素和数据是分开放的
    Object(Object& obj)
    {
        //仅仅拷贝数据，而不拷贝前后关系,数据区位置也不管
        this->head = obj.head;
        this->type = obj.type;
        this->_isFirst = obj._isFirst;
    };
    auto operator=(Object& obj) -> Object&
    {
        this->head = obj.head;
        this->type = obj.type;
        this->_isFirst = obj._isFirst;
        return *this;
    };
    Object(Object& obj, Object* last, Object* next, Object* son)
    {
        *this = obj;
        this->last = last;
        this->next = next;
        this->son = son;
    }
    //使用一个新的son，如果原来有的话，那么原来的将会变成一个孤魂，不会被delete，所以千万不要这样用
    auto newSon() -> Object*
    {
        this->son = new Object();

        return this->son;
    }
    ~Object() =default;
};
//函数声明区
auto strOutFunction(Object* in) -> std::wstring;
auto numOutFunction(Object* in) -> std::wstring;
auto arrayOutFunction(Object* in) -> std::wstring;
auto objOutFunction(Object*) -> std::wstring;
auto addHead(const wchar_t* head, const wchar_t* body) -> std::wstring;

auto strOutFunction(Object* in) -> std::wstring
{
    return std::wstring(L"\"").append(in->data->c_str()).append(L"\"");
}
auto numOutFunction(Object* in) -> std::wstring
{
    return *in->data;
}
//输出数组的文本的函数的in参数应该是数组的第一个节点
auto arrayOutFunction(Object* in) -> std::wstring
{
    std::wstring result;
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
            result.append(L",");
        }
        return false;
    });
    return result;
}
auto objOutFunction(Object* in) -> std::wstring
{
    std::wstring result;
    in->foreach ([&](Object* now) {
        switch (now->type) {
        case VUITypes::Array: {
            result.append(addHead(now->head.c_str(), arrayOutFunction(now).c_str()));
            break;
        }
        case VUITypes::Number: {
            result.append(addHead(now->head.c_str(), numOutFunction(now).c_str()));
            break;
        }
        case VUITypes::Str: {
            result.append(addHead(now->head.c_str(), strOutFunction(now).c_str()));
            break;
        }
        case VUITypes::Object: {
            result.append(addHead(now->head.c_str(), objOutFunction(now).c_str()));
            break;
        }
        }
        //要不要在后面加一个逗号？
        if (now->next != nullptr) {
            result.append(L",");
        }
        return false;
    });
    return result;
}

auto addHead(const wchar_t* head, const wchar_t* body) -> std::wstring
{
    std::wstring temp;
    temp.append(head).append(L"{").append(body).append(L"}");
    return temp;
}
class Manager {
private:
    //对象和数据分开储存
    //数据
    std::list<std::wstring> dataList;
    //根对象
    Object* rootObj = new Object();
    //可能会有多个子obj，每个obj都要他们的最尾端
    //只有当某个obj不为空的时候，ends中才有记录
    std::map<Object*, Object*> ends;

public:
    auto addElement(wchar_t* head, std::wstring body,VUITypes type, Object* dest = nullptr)
    {
        if (type == VUITypes::Array || type == VUITypes::Object) {
            throw "nmsl,tm谁让你type选这两个类型的？这个函数不管这个！！！";
            return ;
        }
        
        dest = dest == nullptr ? this->rootObj : dest;
        Object* t;
        //如果obj为空，那么先添加一个子
        if (dest->son == nullptr) {
            t = dest->newSon();
            ends[dest] = t;
        } else {
            t = new Object();
            ends[dest]->next = t;
            ends[dest]=t;
        }
        // t就是操作数啦,目前t是空的
        t->head.append(head);

        t->type = type;
        this->dataList.emplace_back(std::move(body));
        t->data=&this->dataList.back();
    }
    auto addObject(wchar_t* head, Object* dest = nullptr)
    {
        dest = dest == nullptr ? this->rootObj : dest;
        Object* temp = new Object();
        temp->head=head;
        temp->type = VUITypes::Object;
        ends[dest]->next = temp;
        ends[dest] = temp;
        return temp;
        }
    //如果找不到，那么会返回一个nullptr，防止空指针，请一定要检验（如何检验？判别式、澄清石灰水、带火星的小木条都可以）
    //路径描述语法：a.b.c
    //例子：root有个叫son的obj，但是son有个叫做fuckingson的obj,写成root.son.fuckingson
    auto findObj(std::wstring path, Object* begin = nullptr) -> Object*
    {
        std::deque<std::wstring> paths;
        begin = begin == nullptr ? rootObj : begin;
        Object* res;

        {
            //解析path,以“.”分割字符串，存入paths
            std::wstring::size_type pos;
            path += '.'; //扩展字符串以方便操作
            int size = path.size();
            for (int i = 0; i < size; i++) {
                pos = path.find('.', i);
                if (pos < size) {
                    std::wstring s = path.substr(i, pos - i);
                    paths.push_back(s);
                    i = pos;
                }
            }
        }
        Object* temp = rootObj;
        for (auto i : paths) {
            temp->foreach ([&](Object* in) {
                if (in->head == i) {
                    if (i != paths.back()) {
                        //找到了,没找完
                        temp = in;
                        return true;
                    } else {
                        //找到了，找完了
                        res = in;
                        return true;
                    }
                    //不是这一个
                    return false;
                }
            });
        }
        return res;
    }

    //输出
    auto outString() -> std::wstring
    {
        return addHead(this->rootObj->head.c_str(),objOutFunction(this->rootObj).c_str());
    };
    Manager(std::wstring HEAD)
    {
        this->rootObj->type = VUITypes::Object;
        this->rootObj->head = std::move(HEAD);
    }
    //使用深拷贝来构造
    Manager(Manager& in)
    {
        this->dataList=in.dataList;
        //链表的整个结构需要完全拷贝
    }

    ~Manager()
    {
        //释放数据区内存
        // TODO:释放数据储存区的代码会导致错误，需要修改
        //释放对象内存
        ends.clear();
    }
};
}
