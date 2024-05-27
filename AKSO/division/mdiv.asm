global mdiv

mdiv:
	mov     r11, rdx 						; r11 to y
	xor		rdx, rdx						; ustawienie rdx na 0 przyda się w linijce 29 i w pętli dzielącej

	xor		r9, r9
	add		r9, [rdi + 8 * (rsi - 1)]		; w r9 jest najbardziej znaczący bajt x, żeby pamiętać znak
	jns		.positive_x						
	

; mnożenie x przez -1 poprzez odwrócenie bitów i dodanie 1
    mov     rcx, rsi
	xor		r8,	r8
	stc
.negate_x_1:
	not		QWORD [rdi + 8 * r8]
	adc		QWORD [rdi + 8 * r8], 0
	inc 	r8 
    loop .negate_x_1
; koniec mnożenia x przez -1
	

; od teraz w tablicy na wejściu powinna być dodatnia liczba (chyba, że był tam min_int)


; sprawdzanie czy jest sytuacja min_int/-1, czyli overflow:
	xor     QWORD [rdi + 8 * (rsi - 1)], 0
	jns		.positive_x	                   	; jeżeli nie zmienił się znak x po mnożeniu przez -1, to znaczy, że x = min_int
	cmp     r11, -1
	jne     .positive_x
	div     rdx                             ; dzielenie przez 0, żeby wywołać błąd (rdx zostało ustawione na początku na 0)
; koniec sprawdzania, czy jest overflow
	
	
.positive_x:
	xor		r10, r10
	add		r10, r11						; w r10 jest y, żeby pamiętać znak
	jns		.positive_y
	neg		r11								; od teraz w r11 jest dodatnia liczba (jeżeli y to był min_int to też jest ok)
.positive_y:


; dzielenie dwóch dodatnich liczb
; na początek reszta z poprzedniego dzielenia to 0 i jest w rdx
	mov		rcx, rsi
.divide:
	mov     rax, QWORD [rdi + 8 * (rcx - 1)]
	div		r11								; w rdx jest reszta z poprzedniego dzielenia
	mov		[rdi + 8 * (rcx - 1)], rax
	loop 	.divide	
	mov		rax, rdx						; przeniesienie ostatecznej reszty z dzielenia do rax
; koniec dzielenia dwóch dodatnich liczb


	xor		r10, r9							; jeżeli x ALBO y było ujemne należy odwrócić 
	jns		.check_x


; mnożenie x przez -1 poprzez odwrócenie bitów i dodanie 1
	mov     rcx, rsi
	xor		r8,	r8
	stc
.negate_x_2:
	not		QWORD [rdi + 8 * r8]
	adc		QWORD [rdi + 8 * r8], 0
	inc 	r8 
	loop .negate_x_2
; koniec mnożenia x przez -1


.check_x:
	xor		r9, 0							; jeżeli x był ujemny zmień znak reszty
	jns		.return
	neg		rax
.return:
	ret
