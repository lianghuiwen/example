int Func(int) {
    return 0;
}

struct Obj
{
    int Func(int) {
        return 0;
    }
};

struct FuncObj
{
    int operator()(int) {
        return 0;
    }
};

struct IsLess
{
    IsLess(int val): m_val(val){}
    bool operator()(int val) {
        return val < m_val;
    }
    int m_val;
};

int main(int argc, char *argv[])
{
    typedef int (*pFunc)(int);
    typedef int (Obj::*pMemberFunc)(int);

    int (*pf1)(int) = &Func;    // or Func
    (*pf1)(100);                // or pf1(100);

    pFunc pf2 = &Func;          // or Func
    (*pf2)(100);                // or pf2(100);

    Obj obj;
    int (Obj::*pf3)(int) =  &Obj::Func; // not Obj::Func
    (obj.*pf3)(100);                    // not obj.pf3

    pMemberFunc pf4 = &Obj::Func;       // not Obj::Func
    (obj.*pf4)(100);                    // not obj.pf4

    FuncObj fobj;
    fobj(100);

    return 0;
}
