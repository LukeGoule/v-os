; Code from windows that needs to be used here
;
;	float newsine(double ang) {
;		double out;
;		__asm {
;			finit			// Clear the FPU registers
;			fld[ang]		// Load the angle into st0
;			fsin			// Perform the cosine on st0
;			fstp[out]		// Place the returned value into eax
;		}
;
;		return out;
;	}
;
;	double newcosine(double ang) {
;		double out;
;		__asm {
;			finit			// Clear the FPU registers
;			fld   [ang]		// Load the angle into st0
;			fcos			// Perform the cosine on st0
;			fstp [out]		// Place the returned value into eax
;		}
;
;		return out;
;	}

bits 32

section .text

global __fsin
global __fcos

; IN: 	(eax) angle
; OUT: 	(eax) sin(angle)
__fsin:
	outfl dd 0

	finit
	
	fld dword [eax]
	fsin
	fstp dword [outfl]
	mov eax, outfl
	
	push eax
	ret


; IN: 	(eax) angle
; OUT: 	(eax) cos(angle)
__fcos:
	finit
	
	fld dword [eax]
	fcos
	fstp dword [eax]
	
	push eax
	ret