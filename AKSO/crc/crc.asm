; Atanazy Gawrysiak

global _start

; definiowanie stałych
SYS_READ    equ 0
SYS_WRITE   equ 1
SYS_OPEN    equ 2
SYS_CLOSE   equ 3
SYS_LSEEK   equ 8
SYS_EXIT    equ 60
STDOUT      equ 1
BUFF_LEN    equ 65536 ; 2^16, żeby faktyczne dane z fragmentu się zmieściły
O_RDONLY    equ 0
SEEK_CUR    equ 1
NEW_LINE    equ 10

section .bss
    buffer: resb BUFF_LEN ; tu będziemy wczytywać dane z pliku
    fd: resb 8 ; file descriptor pliku który będziemy 
    len: resb 8 ; tu będzie trzymana długość poszczególnych fragmentów

section	.text

%macro exit 1 ; macro do używnia SYS_EXIT z kodem jako parametr
    mov     rax, SYS_EXIT
    mov     rdi, %1
    syscall
%endmacro

%macro exit_and_close 1 ; macro kończące program i zamykające plik
    mov     rax, SYS_CLOSE
    mov     rdi, [rel fd]
    syscall
    exit %1 ; zakończ program z kodem jaki podano
%endmacro

%macro chceck_if_returned_negative 0 ; macro do sprawdzania czy syscall nie powiódł się (czyli rax jest ujemny)
    test    rax, rax
    jns      %%OK
    exit_and_close 1
%%OK:
%endmacro

%macro chceck_if_error 1 ; macro do sprawdzania czy w rax jest to co chcemy, żeby było
    cmp rax, %1
    je %%OK
    exit_and_close 1 ; jeżeli jest coś innego niż oczekiwaliśmy zakończ program z błędem
%%OK:
%endmacro

_start:
    cmp     DWORD [rsp], 3 ; sprawdzanie czy na wejściu są dwa parametry
    je      .OK0
    exit    1 ; jeżeli nie, to kończymy z błędem
.OK0:
    xor     r8, r8 ; w r8 będziemy trzymali wielomian CRC przesunięty do lewej strony, ale bez pierwszego bitu (zawsze jest równy jeden)
    xor     rdi, rdi ; w rdi zapisana będzie długość wielomianu CRC (bez pierwszego bitu)
    mov     rcx, [rsp + 24] ; rcx to wskaźnik na wielomian CRC zapisany jako ciąg znaków
.convert_loop:
    xor     rax, rax
    mov     al, byte [rcx] ; w najmniej znaczącym bajcie rax jest ASCII code bitu
    test    rax, rax ; sprawdź czy to bajt zerowy
    jz      .end_convert_loop ; jeżeli natrafimy na bajt zerowy, to oznacza, że skończył się napis
    inc     rdi ; zaktualizuj długość wielomianu
    sub     rax, '0' ; zamień ASCII code na liczbę
    cmp     rax, 0
    je      .OK1
    cmp     rax, 1
    je      .OK1
    exit    1 ; jeżeli nie zadziałały powyższe jumpy to znaczy, że to nie jest poprawny zapis wielomianu
.OK1:
    shl     r8, 1 ; zrób miejsce na nowy bit
    add     r8, rax ; ostatni bit r8 to teraz nowo wczytany bit
    inc     rcx ; przesuń wskaźnik na kolejny znak
    jmp     .convert_loop ; wczytaj kolejny znak
.end_convert_loop:
    mov     r9, rdi ; teraz w r9 będzie trzymana długość wielomianu
    sub     rdi, 64
    neg     rdi ; po tej i powyższej operacji rdi = 64 - rdi
    mov     cl, dil ; w cl jest o ile trzeba przesunąć w lewo r8, aby wielomian zaczynał się z lewej strony rejestru
    shl     r8, cl

    mov     rax, SYS_OPEN ; przygotowanie do otwarcia pliku
    mov     rdi, [rsp + 16] ; rdi to wskaźnik na nazwę pliku
    mov     rsi, O_RDONLY ; tylko do odczytu
    mov     rdx, 0
    syscall

    test    rax, rax 
    jns     .OK2 ; SYS_OPEN zwrócił ujemną liczbę, to skończ program z błędem
    exit    1
.OK2:
    mov     [rel fd], rax ; od teraz w fd wskazuje na file descriptor pliku
    xor     rbx, rbx ; w rbx będzie ostateczny wynik, który będzie aktualizowany
.main_loop:
    mov     rax, SYS_READ ; przygotowanie do przeczytania długości fragmentu
    mov     rdi, [rel fd] ; wczytujemy z pliku o tym deskryptorze
    lea     rsi, [rel len] ; wczytywać będziemy do miejsca gdzie trzymamy długość fragmentu
    mov     rdx, 2 ; dwa bajty, bo tyle ma długość
    syscall
    chceck_if_error 2 ; jeżeli SYS_READ zwrócił coś innego niż 2 to źle

    mov     rax, SYS_READ ; przygotowanie do odczytania faktycznej treści fragmentu
    mov     rdi, [rel fd] ; wczytujemy z pliku o tym deskryptorze
    lea     rsi, [rel buffer] ; wczytujemy do buforu
    mov     rdx, [rel len] ; nie ma sensu wczytywać więcej niż jeden fragment, bo może być skok gdzieś
    syscall
    chceck_if_error [rel len] ; jeżeli SYS_READ zwrócił coś innego niż długość to źle

    xor     rdx, rdx
    cmp     rdx, [rel len] 
    je      .end_for_each_byte ; jeżeli fragment jest pusty nic nie rób dla niego
    lea     rsi, [rel buffer] ; rsi to wskaźnik na początek buforu
.for_each_byte:
    xor     rax, rax
    mov     al, byte [rsi + rdx] ; wczytaj następny bajt do najmniej znaczącego bajtu rax
    shl     rax, 56 ; teraz ten bajt siedzi w najbardziej znaczącym bajcie rax
    xor     rbx, rax ; zaktualizuj wynik o nowy fragment (wynik z poprzednich fragmentów zxoruj z nowym bajtem)
    mov     rcx, 8
.for_each_bit:
    bt      rbx, 63
    jc      .xor ; jeżeli najbardziej znaczący bit jest zapalony wykonaj sekcję .xor, a jak nie to .dont_xor
.dont_xor:
    shl     rbx, 1
    jmp     .continue
.xor:
    shl     rbx, 1 ; przesunięcie w prawo, bo wiadomo, że pierwszy bit się wyzeruje (wielomian zaczyna się od 1)
    xor     rbx, r8 ; zxoruj pozostałe bity (xorowanie z zerami nic nie zmienia, więc to nie ma znaczenia jeżeli wielomian nie był długości 65)
.continue:
    loop    .for_each_bit ; powtórz dla każdego bitu, czyli 8 razy
    inc     rdx ; chcemy analizować następny bajt
    cmp     rdx, [rel len]
    jne     .for_each_byte ; jeżeli nie przeanalizowano jeszcze całego fragmentu, powtórz dla następnego bajtu
.end_for_each_byte:

    mov     rax, SYS_READ ; przygotowanie do odczytania faktycznej treści fragmentu
    mov     rdi, [rel fd] ; wczytujemy z pliku o tym deskryptorze
    lea     rsi, [rel buffer] ; wczytujemy do buforu
    mov     rdx, 4 ; 4 bajty, bo tyle ma offset
    syscall
    chceck_if_error 4 ; jeżeli SYS_READ zwrócił coś innego niż 4 to źle

    mov     rax, SYS_LSEEK ; przygotowanie do zmiany pozycji w pliku
    mov     rdi, [rel fd] ; o tym deskryptorze
    movsx   rsi, DWORD [rel buffer] ; zmieniamy pozycję o to co przed chwilą wczytaliśmy (4bajty danych)
    mov     rdx, SEEK_CUR ; chcemy zmienić pozycję relatywnie od aktualniej
    syscall
    chceck_if_returned_negative ; jeżeli SYS_LSEEK zwrócił coś ujemnego, to źle

    movsx   rax, DWORD [rel buffer]   
    add     rax, [rel len]
    add     rax, 6 ; po tych trzech operacjach rax = offset + długość fragmentu + 6

    jnz     .main_loop ; jeżeli to jest 0, to oznacza koniec pliku

    mov     rcx, r9 ; rcx to długość wielomianu z wejścia
    lea     rdi, [rel buffer] ; załaduj do rax adres bufora
.output_loop:
    bt      rbx, 63
    jc      .one ; jeżeli najbardziej znaczący bit jest zapalony skocz do .one
    mov     byte [rdi], '0' 
    jmp     .continue_output
.one:
    mov     byte [rdi], '1'
.continue_output:
    shl     rbx, 1 ; zrób miejsce na następny bit
    inc     rdi ; rdi wskazuje na następny bajt wyjścia
    loop    .output_loop ; powtórz pętle tyle razy, jak dużo bitów ma wielomian z wejścia

    mov     byte [rdi], NEW_LINE

    mov     rax, SYS_WRITE ; przygotowanie do wypisania
    mov     rdi, STDOUT ; na standardowe wyjście
    lea     rsi, [rel buffer] ; zawartosci bufora
    inc r9
    mov     rdx, r9 ; wypisujemy długość wielomianu + 1 znaków, bo trzeba doliczyć znak nowej linii
    syscall
    chceck_if_error r9 ; jeżeli SYS_WRITE zwrócił coś innego niż długość wejścia + 1, to źle

    exit_and_close 0
