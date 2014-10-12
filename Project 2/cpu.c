#include <stdio.h>
#include <stdlib.h>
#include <string.h>


 /****************************************************************************/
/*                                                                          */
/* 			     Module CPU                                     */
/* 			External Declarations 				    */
/*                                                                          */
/****************************************************************************/


/* OSP constant      */

#define   MAX_PAGE                   16 /* max size of page tables          */

/* OSP enumeration constants */

typedef enum {
    false, true                         /* the boolean data type            */
} BOOL;

typedef enum {
    running, ready, waiting, done       /* types of status                  */
} STATUS;



/* external type definitions */

typedef struct page_entry_node PAGE_ENTRY;
typedef struct page_tbl_node PAGE_TBL;
typedef struct event_node EVENT;
typedef struct pcb_node PCB;

/*insert in background declaration and head, tail pointers for foreground and background queue*/
void insert_ready(PCB *pcb);
void insert_background(PCB *pcb);
PCB *hf;
PCB *tf;
PCB *hb;
PCB *tb;
PCB *tempf;
PCB *tempb;
PCB *current_pcb;



/* external data structures */

extern struct page_entry_node {
    int    frame_id;    /* frame id holding this page                       */
    BOOL   valid;       /* page in main memory : valid = true; not : false  */
    int    *hook;       /* can hook up anything here                        */
};

extern struct page_tbl_node {
    PCB    *pcb;        /* PCB of the process in question                   */
    PAGE_ENTRY page_entry[MAX_PAGE];
    int    *hook;       /* can hook up anything here                        */
};

extern struct pcb_node {
    int    pcb_id;         /* PCB id                                        */
    int    size;           /* process size in bytes; assigned by SIMCORE    */
    int    creation_time;  /* assigned by SIMCORE                           */
    int    last_dispatch;  /* last time the process was dispatched          */
    int    last_cpuburst;  /* length of the previous cpu burst              */
    int    accumulated_cpu;/* accumulated CPU time                          */
    PAGE_TBL *page_tbl;    /* page table associated with the PCB            */
    STATUS status;         /* status of process                             */
    EVENT  *event;         /* event upon which process may be suspended     */
    int    priority;       /* user-defined priority; used for scheduling    */
    PCB    *next;          /* next pcb in whatever queue                    */
    PCB    *prev;          /* previous pcb in whatever queue                */
    int    *hook;          /* can hook up anything here                     */
};


/* external variables */

extern PAGE_TBL *PTBR;		/* page table base register */

extern int    Quantum;		/* global time quantum; contains the value
                             entered at the beginning or changed
                             at snapshot. Has no effect on timer
                             interrupts, unless passed to set_timer() */



/* external routines */

extern prepage(/* pcb */);
extern int start_cost(/* pcb */);
/*  PCB    *pcb; */

extern set_timer(/* time_quantum */);
/*  int    time_quantum; */

extern int get_clock();





/****************************************************************************/
/*                                                                          */
/*				Module CPU				    */
/*			     Internal Routines				    */
/*                                                                          */
/****************************************************************************/





void cpu_init()
{
    hf = malloc(sizeof(struct pcb_node));
    tf = malloc(sizeof(struct pcb_node));
	tb = malloc(sizeof(struct pcb_node));
    hb = malloc(sizeof(struct pcb_node));
    hf = NULL;
    tf = NULL;
    hb = NULL;
    tb = NULL;
}

void dispatch()
{
    PCB *pcb;
	
    if(PTBR->pcb->status == running)
    {   /*insert process in ready queue*/
		insert_ready(PTBR->pcb);
	}
    //pcb = hf; //if pcb = head here, would it ever go to hb?
    
    if(pcb == hf)
       {
            PTBR = pcb->page_tbl;
            prepage(pcb);
            pcb->status = running;
            pcb->last_dispatch = get_clock();
            set_timer(Quantum);
            insert_background(PTBR->pcb);
        }
        else if (pcb == hb)
        {
            PTBR = pcb->page_tbl;
            prepage(pcb);
            pcb->status = running;
            pcb->last_dispatch = get_clock();
            set_timer(Quantum);
        }

        else
        {
            PTBR = NULL;
        }
}


void insert_ready(PCB *pcb)
{
    if (PTBR->pcb->status != running)
    {   if (hf == NULL)
        {   pcb->status = ready;
            pcb->next = NULL;
            pcb->prev = NULL;
            hf = pcb;
            tf = pcb;
        }
        else
        {   pcb->next = NULL;
            pcb->prev = tf;
            tf->next = pcb;
            tf = pcb;
        }
        pcb = hf; //pcb is equal to head here or in dispatch? if in dispatch would it ever go to background?
    }
    
}

void insert_background(PCB *pcb)
{
    pcb = hb;
    if (PTBR->pcb->status != running)
    {
        if(hb == NULL)
        {
            pcb->status = ready;
            pcb->next = NULL;
            pcb->prev = NULL;
            hb = pcb;
            tb = pcb;
        }
        else
        {
            pcb->next = NULL;
            pcb->prev = tb;
            tb->next = pcb;
            tb = pcb;
        }
    }
}

/* end of module */