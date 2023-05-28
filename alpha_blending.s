section .data
    one dd 1.0
	max dd 255.0

section .text

global alpha_blending

; Main function
alpha_blending:
    push rbp
	mov rbp, rsp

	; Define variables on stack
    ;%define image1  [rbp]
    ;%define image2  [rbp + 4]
    ;%define size    [rbp + 8]
    ;rdi -> 1 bitmap
    ;rsi -> 2 bitmpa
    ;rdx -> x
    ;rcx -> y
    ;r8 -> IMageWidth

	;initial (X,Y) position
	mov r11, 1
	mov r12, 1
    mov r9, r8; szerokość
    imul r8, r8; wielkość
	imul r8, 4
    mov rax, 0; licznik




blend_colors:
    cmp rax, r8 ; jesli licznik wiekszy niż wielkość koniec
    jge endl
	;---------Sinus-------------------
  ;X
	cvtsi2ss xmm0, r11  ; przenieś pozycje x do float xmm0
	;Xc
	cvtsi2ss xmm1, rdx ;przenieś pozycje x_klik do float xmm1
	;Y
	cvtsi2ss xmm2, r12  ;przenieś pozycje y do float xmm2
	;Yc
	cvtsi2ss xmm3, rcx ;przenieś pozycje y_klik do float xmm3
	;X- Xc
	subss xmm0, xmm1    ; odelgóść na x
    movss xmm1, xmm0    ; kopia odległości
	;Y-Yc
	subss xmm2, xmm3 ; odelgóść na y
	;power 2
	mulss xmm0, xmm0
	mulss xmm2, xmm2
	;Xw + Yw
	addss xmm0, xmm2
	sqrtss  xmm0, xmm0  ;pierwiastek
    divss   xmm1, xmm0  ;siunus x/r
    ; xmm1<-sinus x

	movss xmm1, [rdi + rax + 3] ; read alpha value of object
	divss xmm1, [max]
;----------------blendB---------------
	movss xmm0, [one]	;1
	subss xmm0, xmm1 ; 1 - a

	movss xmm2, [rdi+rax] ;bajt 1 bitmapy
	movss xmm3, [rsi+rax] ;bajt 2 bitmapy

	mulss xmm2, xmm1	; B1*alpha
	mulss xmm3, xmm0	; B2*(1-alpha)

	addss xmm2, xmm3	;B1+B2

	;write B to image pixel
	movss [rdi + rax], xmm2	;zapis nowgo B w rdi

	movss xmm1, [rsi + rax + 3] ; read alpha value of object


    divss xmm1, [max] ; convert alpha to [0,1] value

;----------------blendG---------------

	movss xmm0, [one]
	subss xmm0, xmm1

	movss xmm2, [rdi+rax+1] ;bajt 1 bitmapy
	movss xmm3, [rsi+rax+1] ;bajt 2 bitmapy

	mulss xmm2, xmm1
	mulss xmm3, xmm0

	addss xmm2, xmm3

	;cvtss2si eax, xmm2

    ;write G to image pixel
	movss [rdi + rax+1], xmm2


	;-----R-----------------
	inc rax
	;-------------BlendR------------
	movss xmm0, [one]
	subss xmm0, xmm1

	movss xmm2, [rdi+rax+2] ;bajt 1 bitmapy
	movss xmm3, [rsi+rax+2] ;bajt 2 bitmapy


	mulss xmm2, xmm1
	mulss xmm3, xmm0

	addss xmm2, xmm3

	;cvtss2si eax, xmm2

	;write R to image pixel
	movss [rdi + rax+2], xmm2

    add rax, 4
	inc r11

	cmp r11, r9
	jle blend_colors

	mov r11, 1
	inc r12

	jmp blend_colors

endl:
	mov rsp, rbp
	pop rbp
	ret