global outb;
global outw;
global inb;
global inw;

outb:
	enter 0,0;
	mov dx,[ebp+8]; Here we have the port
	mov eax,[ebp+12]; Here is the data
	out dx,al;
	leave;
	ret;

outw:
	enter 0,0
	mov dx,[ebp+8]; Here we have the port
	mov eax,[ebp+12]; Here is the data
	out dx,ax;
	leave;
	ret;

inb:
	enter 0,0;
	mov eax,0;
	mov dx,[ebp+8]; The port
	in al,dx;
	leave;
	ret;

inw:
	enter 0,0;
	mov eax,0;
	mov dx,[ebp+8]; The port
	in ax,dx;
	leave;
	ret;
