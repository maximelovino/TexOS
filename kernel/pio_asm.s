global outb;
global outw;
global inb;
global inw;

;TODO remove enter and leave and put ebp+4 and ebp+8 instead of ebp+8 and ebp+12
outb:
	enter 0,0
	mov dx,word[ebp+8]; Here we have the port
	mov al,byte[ebp+12]; Here is the data
	out dx,al;
	leave;
	ret;

outw:
	enter 0,0
	mov dx,word[ebp+8]; Here we have the port
	mov ax,word[ebp+12]; Here is the data
	out dx,ax;
	leave;
	ret;

inb:
	enter 0,0;
	mov eax,0;
	mov dx,word[ebp+8]; The port
	in al,dx;
	leave;
	ret;

inw:
	enter 0,0;
	mov eax,0;
	mov dx,word[ebp+8]; The port
	in ax,dx;
	leave;
	ret;
