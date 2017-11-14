%include "const.inc"
extern exception_handler;
extern irq_handler;
global idt_load;

section .text   ; start of the text (code) section
align 4         ; the code must be 4 byte aligned

;------------------------------------------------
; CPU exceptions
%macro macro_interrupt_nocode 1
global proc_interrupt_%1
proc_interrupt_%1:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    %1  ; exception number
    jmp     exception_wrapper
%endmacro

%macro macro_interrupt_code 1
global proc_interrupt_%1
proc_interrupt_%1:
    cli          ; disable interrupts
    push    %1  ; exception number
    jmp     exception_wrapper
%endmacro

;;Export of processor interrupts
macro_interrupt_nocode 0
macro_interrupt_nocode 1
macro_interrupt_nocode 2
macro_interrupt_nocode 3
macro_interrupt_nocode 4
macro_interrupt_nocode 5
macro_interrupt_nocode 6
macro_interrupt_nocode 7
macro_interrupt_code 8
macro_interrupt_nocode 9
macro_interrupt_code 10
macro_interrupt_code 11
macro_interrupt_code 12
macro_interrupt_code 13
macro_interrupt_code 14
macro_interrupt_nocode 15
macro_interrupt_nocode 16
macro_interrupt_code 17
macro_interrupt_nocode 18
macro_interrupt_nocode 19
macro_interrupt_nocode 20

;------------------------------------------------
; IRQ
%macro macro_irq 1
global irq_%1
irq_%1:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    %1  ; irq number
    jmp     irq_wrapper
%endmacro

;;This exports all irq macros
%assign i 0
%rep 16
macro_irq i
%assign i i+1
%endrep

;------------------------------------------------
; Wrapper for exceptions

exception_wrapper:
    ; Save all registers
    push    eax
    push    ebx
    push    ecx
    push    edx
    push    esi
    push    edi
    push    ebp
    push    ds
    push    es
    push    fs
    push    gs

    ; Load kernel data descriptor into all segments
    mov     ax,GDT_KERNEL_DATA_SELECTOR
    mov     ds,ax
    mov     es,ax
    mov     fs,ax
    mov     gs,ax

    ; Pass the stack pointer (which gives the CPU context) to the C function
    mov     eax,esp
    push    eax
    call    exception_handler  ; implemented in idt.c
    pop     eax  ; only here to balance the "push eax" done before the call

    ; Restore all registers
    pop     gs
    pop     fs
    pop     es
    pop     ds
    pop     ebp
    pop     edi
    pop     esi
    pop     edx
    pop     ecx
    pop     ebx
    pop     eax

	; Fix the stack pointer due to the 2 push done before the call to
	; exception_wrapper: error code and exception/irq number
    add     esp,8
    iret

irq_wrapper:
    ; Save all registers
    push    eax
    push    ebx
    push    ecx
    push    edx
    push    esi
    push    edi
    push    ebp
    push    ds
    push    es
    push    fs
    push    gs

    ; Load kernel data descriptor into all segments
    mov     ax,GDT_KERNEL_DATA_SELECTOR
    mov     ds,ax
    mov     es,ax
    mov     fs,ax
    mov     gs,ax

    ; Pass the stack pointer (which gives the CPU context) to the C function
    mov     eax,esp
    push    eax
    call    irq_handler  ; implemented in idt.c
    pop     eax  ; only here to balance the "push eax" done before the call

    ; Restore all registers
    pop     gs
    pop     fs
    pop     es
    pop     ds
    pop     ebp
    pop     edi
    pop     esi
    pop     edx
    pop     ecx
    pop     ebx
    pop     eax

	; Fix the stack pointer due to the 2 push done before the call to
	; exception_wrapper: error code and exception/irq number
    add     esp,8
    iret

idt_load:
    mov     eax,[esp+4]  ; Get the pointer to the GDT, passed as a parameter.
    lidt    [eax];
	ret;
