section .data
    one dd 1.0
	two dd 2.0
	zero dd 0.0
	six dd 6.0
	onehtw dd 120.0
	sevenF dd 5040.0


section .text

global alpha_blending

; Main function
alpha_blending:
    push rbp
	mov rbp, rsp
    ;rdi -> 1 bitmap
    ;rsi -> 2 bitmpa
    ;rdx -> x
    ;rcx -> y
    ;r8 -> IMageWidth
	;r9 -> Size

	;X, Y na (1, 1)
	mov r11, 1
	mov r12, 1
    mov rax, 0; licznik




blend_colors:
    cmp rax, r9 ; jesli licznik wiekszy niż wielkość koniec
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
    ;movss xmm1, xmm0    ; kopia odległości
	;Y-Yc
	subss xmm2, xmm3 ; odelgóść na y
	;power 2
	mulss xmm0, xmm0
	mulss xmm2, xmm2
	;Xw + Yw
	addss xmm0, xmm2
	sqrtss  xmm0, xmm0  ;pierwiastek

    ;divss   xmm1, xmm0  ;siunus x/r
	;
	addss 	xmm1, [one]
	divss	xmm1, [two]
    ; xmm1<-sinus x
	;
	cvtsi2ss xmm2, r8
	divss xmm0, xmm2

	;---------SINUS-----------------
	;x - x^3/6
	;save x

	movss xmm1, [zero]
	subss xmm1, xmm0
	mulss xmm1, xmm0
	mulss xmm1, xmm0
	divss xmm1, [six]
	addss xmm1, xmm0

	; x-x^3/6
	;---------X^5----------------------
	movss xmm2, xmm0

	mulss xmm2, xmm0
	mulss xmm2, xmm0
	mulss xmm2, xmm0
	mulss xmm2, xmm0

	divss xmm2, [onehtw]

	; x-x^3/6+x^5/120
	addss xmm1, xmm2

	movss xmm2, [zero]
	subss xmm2, xmm0

	mulss xmm2, xmm0
	mulss xmm2, xmm0
	mulss xmm2, xmm0
	mulss xmm2, xmm0
	mulss xmm2, xmm0
	mulss xmm2, xmm0

	divss xmm2, [sevenF]
	;x-x^3/6 + x^5/120 - x^7 /
	subss xmm1, xmm2

;----------------blendR---------------
	movss xmm0, [one]	;1
	subss xmm0, xmm1 ; 1 - a

	movss xmm2, [rdi+rax] ;bajt 1 bitmapy
	movss xmm3, [rsi+rax] ;bajt 2 bitmapy

	mulss xmm2, xmm1	; B1*alpha
	mulss xmm3, xmm0	; B2*(1-alpha)

	addss xmm2, xmm3	;B1+B2

	;write B to image pixel
	movss [rdi + rax], xmm2	;zapis nowgo B w rdi


;----------------blendG---------------

	movss xmm0, [one]
	subss xmm0, xmm1
	inc rax
	cmp rax, r9 ; jesli licznik wiekszy niż wielkość koniec
    jge endl
	movss xmm2, [rdi+rax] ;bajt 1 bitmapy
	movss xmm3, [rsi+rax] ;bajt 2 bitmapy

	mulss xmm2, xmm1
	mulss xmm3, xmm0

	addss xmm2, xmm3

	;cvtss2si eax, xmm2

    ;write G to image pixel
	movss [rdi + rax], xmm2


	;------------BlendB------------
	movss xmm0, [one]
	subss xmm0, xmm1
	inc rax
	cmp rax, r9 ; jesli licznik wiekszy niż wielkość koniec
    jge endl
	movss xmm2, [rdi+rax] ;bajt 1 bitmapy
	movss xmm3, [rsi+rax] ;bajt 2 bitmapy


	mulss xmm2, xmm1
	mulss xmm3, xmm0

	addss xmm2, xmm3

	;write R to image pixel
	movss [rdi + rax], xmm2


    inc rax
	;inc rax
	inc r11

	cmp r11, r8
	jl blend_colors

	mov r11, 1
	inc r12

	jmp blend_colors

endl:
	mov rsp, rbp
	pop rbp
	ret