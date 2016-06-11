#include <stdio.h>
#include <stdlib.h>

int main(){
	char c = 0;
	char data[102] = {0};
	int i = 0;
	
//读取xml文件
 fp = fopen("update.xml","r");
 if(fp == NULL) return 0;
 while((c = getc(fp)) != EOF)
 {
  data[i++] = c;
 }
 xml_takecell(data,0,i,"update.xml");
}
 
//对data数组进行相应的处理，如下：
int xml_takecell(char *data, int start, int end, char *name)
{
 int flag = 0;
 int j = 0;
 int nowstart = 0;
 int nowend = 0;
 char value[45] = {0};
 int keywords = 0;
 int isok = 0;

 if(update == 1) return 1;

 nowstart = xml_getnamestart(data,start,end,name);
 nowend = xml_getnameend(data,nowstart,end,name); 
 keywords = iskeywords(name);
 
 switch(keywords)
 {
 case KEY_UPDATELIST:
  xml_getvalue(data,nowstart,nowend,value);
  node_name = keywords;
  flag = 0;
  break;
 case KEY_ENABLE:
  xml_getvalue(data,nowstart,nowend,value);
  flag = 0;
  break;
 case KEY_START:
  xml_getvalue(data,nowstart,nowend,value);
  flag = 0;
  break;
 case KEY_END:
  xml_getvalue(data,nowstart,nowend,value);
  flag = 0;
  break;
 case KEY_ID:
  node_name = keywords;
  flag = 1;
  break;
 case KEY_SWVERSION:
  node_name = keywords;
  flag = 1;
  break;
 case KEY_CONFIG:
  node_name = keywords;
  flag = 1;
  break;

  ……
 default:
  flag = 1;
  node_name = 0;
  break;
 } 

 isok = checkupdate(node_name,keywords,value);
 if(isok == 1) return 0;

 if(flag == 1 && nowend <end)
 {
  cleanstring(name, 15);
  xml_takecell(data,nowstart,nowend,name);
 }
 if(nowend <end) 
 {
  cleanstring(name, 15);
  xml_takecell(data,nowend,end,name); 
 }

 return 0;
}

/*这是实现递归的核心代码，其中，函数xml_getnamestart()、xml_getnameend()是对同一个标签的开始处和结尾处进行查询，返回值是所在数组的位置，然后可以在此标签内部进行递归查询，直到找到最低一级的标签，并利用 xml_getvalue()获取标签的属性值。函数checkupdate()是对获取的属性值合法性做判断，如果是非法的，可以直接跳出此次处理。KEY_UPDATELIST、KEY_SWVERSION这些是自己事先定义的标签名。*/

//各函数如下：

//函数xml_getnamestart：

int xml_getnamestart(char *data, int start, int end, char *name)
{
 int now = start;
 int i = 0;
 int flag = 0;

 while(now < end)
 {
  now++;
 /*在这里，可以忽略<!--注释 -->形式的注释*/ 

 if(data[now-1] == '<' && data[now] != '!' && data[now] != '/')
   flag = 1; 
  else if((data[now] == '>' || data[now] == ' ') && flag == 1)
  {
   flag = 0;
//   printf("xml_getnamestart---->name is %s now is %d\n",name,now);
   break;
  }
  if(flag == 1)
  {
   name[i] = data[now];
   i++;
  }

 }
 return now;
}

//函数xml_getnameend：

int xml_getnameend(char *data, int start, int end, char *name)
{
 int now = start;
 int i = 0;
 int flag = 0;
 char value[15] = {0};
 int ok = 0;

 while(now < end)
 {
  now++;
  if(data[now-1] == '<' && data[now] == '/')
  {
   flag = 1; 
   i = 0;
   ok = 0;
   now++;  
  }
 /*这里表示标签以空格或>结尾，也解释了属性值1为什么不能解析*/

 else if((data[now] == '>' || data[now] == ' ') && flag == 1)
  {
   flag = 0;
  }
  if(flag == 1)
  {
   if(name[i] == data[now])
   {
    i++;
    ok++; 
   }
   else if(name[i] != data[now])
   {
    i = 0;
    ok = 0;
    flag = 0;
   }
   
  }
  
  if(ok == strlen(name)) break;
 }
// printf("xml_getnameend---->name is %s now is %d \n",name,now);
 return now;
}

//函数xml_getvalue：

/*在 > 和 < 之间为属性值*/

int xml_getvalue(char *data, int start, int end, char *value)
{
 int i = 0;
// char value[45] = {0};
 cleanstring(value,45);
 if(data[i+start] == '>') start++;
 for(i = 0; i+start< end; i++)
 {
  if(data[i+start] == '<') break;
  value[i] = data[start+i]; 
 }

 return 0;
}
