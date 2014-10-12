###############################################################
#   Students' Makefile for Operating Systems Project          #
#                     Use As Is                               #
###############################################################



# The default C compiler 
C_COMP = cc 


#make OSP:  

OSP : osp.o dialog.o cpu.o 
	$(C_COMP) osp.o dialog.o cpu.o -lm -g -o OSP

dialog.o : dialog.c
	$(C_COMP) -w -c -g dialog.c

cpu.o : cpu.c 
	$(C_COMP) -w -c -g cpu.c 

