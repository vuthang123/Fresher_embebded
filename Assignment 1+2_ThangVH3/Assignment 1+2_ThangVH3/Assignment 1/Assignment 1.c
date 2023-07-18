#include <stdio.h>

int main() {
    char *c;
    /*
    	- Thay vì sử dụng con trỏ hai chiều **s, ta có thể sử dụng một con trỏ đơn *s 
 		để trỏ trực tiếp tới chuỗi ký tự.
 		- Do đó, ta chỉ cần sửa đổi kiểu của con trỏ từ char ** sang char *,và gán giá 
		trị của con trỏ trực tiếp bằng chuỗi ký tự foo.
	*/
    char *s;
    char foo[] = "Hello World";
    /*
		Sau khi sửa đổi, ta sẽ không cần phải sử dụng phép giải tham chiếu hai lần để 
		truy cập đến chuỗi ký tự, 
		Thay vào đó ta chỉ cần sử dụng một con trỏ đơn để truy cập đến giá trị của chuỗi
		ký tự một cách trực tiếp.
	*/
	s = foo;
	printf("s is %s\n",(char *)s);
	c = &s[0];
	printf("s[0] is %c\n", *c);

	return 0;
}


