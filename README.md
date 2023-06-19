# BookManageSys

一个用链表菜单实现的C++图书管理系统

核心是菜单的实现：

```
// 菜单项类
class MenuItem {
public:
    string name;      // 菜单项名称
    vector<MenuItem*> subMenus;  // 子菜单
    // 返回值为void，参数为空的 函数指针
    // func = xxx 可以复制成返回值为void，参数为空的 函数
    // void print();
    // 定义 func = print;
    // 执行 func() = print();
    void (*func)();  // 执行函数
    MenuItem* parent;    // 父菜单项

    MenuItem();

    // 构造函数
    MenuItem(string name);

    // 复制构造函数
    MenuItem(const MenuItem &other);

    MenuItem& operator=(const MenuItem &other);

    void showMenu();

    void addSubMenu(MenuItem* menu);

    void execFunc(MenuItem* menu);

};
```

这样可以通过这样声明菜单对象：

```
MenuItem m_main("主菜单");

MenuItem m_return_main("返回主菜单"), m_exit("退出系统");

// 主菜单下面的菜单
MenuItem m_user_manage("用户管理菜单"), m_reader_manage("读者管理菜单"),m_book_manage("图书管理"),m_book_circulation_manage("图书流通管理");
// 用户管理菜单
MenuItem m_user_input("用户信息输入"), m_user_edit("用户信息修改"), m_user_passwd_edit("用户密码修改"), m_user_delete("用户信息删除"),m_user_display("用户信息显示");
// 读者管理菜单
MenuItem m_reader_input("读者信息输入"),m_reader_delete("读者信息删除"),m_reader_find("读者信息查询"),m_reader_display("读者信息显示"),m_reader_edit("读者信息修改");
//图书管理菜单
MenuItem m_book_input("图书信息输入"), m_book_edit("图书信息修改"),m_book_find("图书信息查询"),m_book_total("汇总统计");
//图书物流管理
MenuItem m_book_circulation_borrow("借书处理"), m_book_circulation_return("还书处理");
//图书信息查询
MenuItem m_book_find_number("按书号查询"),m_book_find_name("按书名查询"),m_book_find_author("按作者查询"),m_book_find_publish("按出版社查询");
MenuItem m_cur;
```

然后通过这样定义菜单功能：

```
m_return_main.func = return_main;

m_exit.func = exit_pro;

void return_main() {
    m_cur = m_main;
}

void exit_pro() {
    exit(0);
}
```

通过这样定义菜单的子菜单：

```
m_main.addSubMenu(&m_user_manage);
m_main.addSubMenu(&m_reader_manage);
m_main.addSubMenu(&m_book_manage);
m_main.addSubMenu(&m_book_circulation_manage);
m_main.addSubMenu(&m_exit);
```

一个菜单要么有子菜单（点击进入子菜单），要么是一个执行选项（点击执行函数），逻辑在这：

```
m_cur = m_main;

int op = 0;

m_cur.showMenu();
while (cin >> op) {
    MenuItem m_select = *m_cur.subMenus[op - 1];
    if (m_select.func == NULL) {
        m_cur = m_select;
    }
    else {
        m_select.func();
    }
    m_cur.showMenu();
}
```

总之这套菜单结构非常好用~
