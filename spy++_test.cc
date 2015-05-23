/*
Spy++ (SPYXX.EXE) 是一个基于 Win32 的实用工具，它提供系统的进程、线程、窗口和窗口消息的图形视图。使用 Spy++ 可以执行下列操作： 
1、显示系统对象（包括进程、线程和窗口）之间关系的图形树。 
2、搜索指定的窗口、线程、进程或消息。 
3、查看选定的窗口、线程、进程或消息的属性。 
4、直接从视图中选择窗口、线程、进程或消息。 
5、通过鼠标定位，使用查找程序工具选择窗口。 
6、使用复杂的消息日志选择参数设置消息选项。
*/
 
// 示例1：利用spy找到窗口类名及标题
void Example_1()
{
    HWND proc = FindWindow("TForm1","NetSetMan");   
    if(proc)   
    {   
        HWND h1 = NULL;   
        char className[32];   
        char capitalName[32];   
      
        while(h1 = FindWindowEx(proc,h1,NULL,NULL))   
        {   
            GetClassName(h1,className,sizeof(className));   
            CWnd::FromHandle(h1)->GetWindowText(capitalName,sizeof(capitalName));   
            printf("%s:%s/n",className,capitalName);   
      
            HWND h2 = NULL;   
            //while(h2 = FindWindowEx(h1,h2,NULL,NULL))   
            //{   
            //  GetClassName(h2,className,sizeof(className));   
            //  CWnd::FromHandle(h2)->GetWindowText(capitalName,sizeof(capitalName));   
            //  printf("/t%s:%s/n",className,capitalName);   
            //     
            //  if(strcmp(capitalName,"应用") == 0)   
            //  {   
            //      CWnd::FromHandle(h2)->SendMessage(BM_CLICK);   
            //  }   
            //}   
               
            if(h2 = FindWindowEx(h1,NULL,"TTntButton.UnicodeClass","应用"))   
            {   
                CWnd::FromHandle(h2)->SendMessage(BM_CLICK);   
            }   
        }   
    }else  
    {   
        printf("process not found!/n");   
    }
}

// 示例2：当控件没有标题时可以通过排列位置找到该控件
void Example_2()
{
    HWND proc = FindWindow("MrTalk_clazz","MediaRing Talk");   
    HWND my1 = NULL;   
    HWND my2 = NULL;   
    if(proc)   
    {   
        HWND h1 = NULL;   
        char className[32];   
        char capitalName[32];   
      
        while(h1 = FindWindowEx(proc,h1,NULL,NULL))   
        {   
            GetClassName(h1,className,sizeof(className));   
            CWnd::FromHandle(h1)->GetWindowText(capitalName,sizeof(capitalName));   
            //printf("%s:%s/n",className,capitalName);   
      
            int index = 0;   
            HWND h2 = NULL;   
            while(h2 = FindWindowEx(h1,h2,NULL,NULL))   
            {   
                ++index;   
                GetClassName(h2,className,sizeof(className));   
                CWnd::FromHandle(h2)->GetWindowText(capitalName,sizeof(capitalName));   
                //printf("/t%s:%s/n",className,capitalName);   
      
                if(index == 6)   
                {   
                    my1 = h2;   
                    ((CComboBox *)CWnd::FromHandle(h2))->SetCurSel(0);      
                }   
      
                if(index == 17)   
                {   
                    my2 = h2;   
                    CWnd::FromHandle(h2)->SendMessage(BM_CLICK);   
                }   
            }   
        }   
    }else  
    {   
        printf("process not found!/n");   
    }
}

int main(int argc, char *argv[])
{
    Example_1();
}