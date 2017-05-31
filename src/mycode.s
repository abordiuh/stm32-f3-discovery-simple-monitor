@@ mycode.s :
@@ Test code for STM32 and linking assembly to C
 

    .code   16
    .text                   @@ - Code section (text -> ROM)

@@ <function block>
    .align  2               @@ - 2^n alignment (n=2)
    .syntax unified
    .global mytest          @@ - Symbol name for function
    .code   16              @@ - 16bit THUMB code (BOTH are required!)
    .thumb_func             @@ /
    .type   mytest, %function   @@ - symbol type (not req)
@@ Declaration : int mytest(int x)
@@ Uses r0 for param 0
@@   r0: x
mytest:
    push {lr}
    push {r1}
    push {r0-r7}
    ldr  r0, =0
    bl   BSP_LED_Toggle           @@ call BSP function
    pop  {r0-r7}
    ldr  r1, =myTickCount
    ldr  r0, [r1]
    pop  {r1} 
    pop  {pc}
    .size   mytest, .-mytest    @@ - symbol size (not req)

@@ *****************************************************************
@@ FUNCTION NAME: ledPatternFlash
@@ PARAMETERS   : void
@@ RETURNS      : void
@@ DESCRIPTION  :
@@  This function makes a the LED3 of the STM32F3DISCOVERY board
@@  to toggle each 1s
@@ *****************************************************************
 
@@ <function block>
    .align  2               @@ - 2^n alignment (n=2)
    .syntax unified
    .global ledPatternFlash          @@ - Symbol name for function
    .code   16              @@ - 16bit THUMB code (BOTH are required!)
    .thumb_func             @@ /
    .type   ledPatternFlash, %function   @@ - symbol type (not req)

@@ Declaration : void ledPatternFlash(void)
@@ Uses r0 for param 0
@@   r0: x
ledPatternFlash:
		push {lr}
		push {r0-r7}
   
		LDR r0, =0
		LDR r3, =7

led:		BL BSP_LED_Toggle
		ADD r0,#1
		CMP r0,r3
		BLE led				@@ toggle all leds until it reaches 7
		
		LDR r4, =myTickCount		@@ get the value of myTickCOunt as a timestamp
		LDR r2,[r4]			@@ assign the value of myTickCount to r2
		ADD r2,r2,#1000			@@ r2 represent 1s elapsed since the timestamp taken
     
delay:		LDR r1, =myTickCount		@@gets another timestamp
		LDR r5,[r1]			@@assign the value of the timestamp to r5
		CMP r5,r2			@@compare the current timestamp with the one got before
		BLE delay			@@if 1s is not elapsed then loop again

		pop	{r0-r7}
		pop	{pc}
	
	.size   mytest, .-mytest    @@ - symbol size (not req)
@@ <function block>
    .align  2               @@ - 2^n alignment (n=2)
    .syntax unified
    .global my_Tick          @@ - Symbol name for function
    .code   16              @@ - 16bit THUMB code (BOTH are required!)
    .thumb_func             @@ /
    .type   my_Tick, %function   @@ - symbol type (not req)
@@ Declaration : void my_Tick( void )
@@ Uses nothing
my_Tick:
    push {lr}
    push {r0-r1}

    ldr  r1, =myTickCount
    ldr  r0, [r1]
    add  r0, r0, #1
    str  r0, [r1]
    pop {r0-r1}
    pop  {pc}
    .size   my_Tick, .-my_Tick    @@ - symbol size (not req)

@@ <function block>
    .align  2               @@ - 2^n alignment (n=2)
    .syntax unified
    .global my_Loop          @@ - Symbol name for function
    .code   16              @@ - 16bit THUMB code (BOTH are required!)
    .thumb_func             @@ /
    .type   my_Loop, %function   @@ - symbol type (not req)
@@ Declaration : void my_Loop( void )
@@ Uses nothing
my_Loop:
    push {lr}
    pop  {pc}
    .size   my_Loop, .-my_Loop    @@ - symbol size (not req)

@@ <function block>
    .align  2               @@ - 2^n alignment (n=2)
    .syntax unified
    .global my_Init          @@ - Symbol name for function
    .code   16              @@ - 16bit THUMB code (BOTH are required!)
    .thumb_func             @@ /
    .type   my_Init, %function   @@ - symbol type (not req)
@@ Declaration : void my_Init( void )
@@ Uses nothing
my_Init:
    push {lr}
    pop  {pc}
    .size   my_Init, .-my_Init    @@ - symbol size (not req)

    .data
    .global myTickCount
myTickCount:
    .word  1         /* A 32-bit variable named myTickCount */


    .end

