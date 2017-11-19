global outb;
global outw;
global inb;
global inw;

outb:
	mov dx,word[esp+4]; Here we have the port
	mov al,byte[esp+8]; Here is the data
	out dx,al;
	ret;

outw:
	mov dx,word[esp+4]; Here we have the port
	mov ax,word[esp+8]; Here is the data
	out dx,ax;
	ret;

inb:
	mov eax,0;
	mov dx,word[esp+4]; The port
	in al,dx;
	ret;

inw:
	mov eax,0;
	mov dx,word[esp+4]; The port
	in ax,dx;
	ret;
