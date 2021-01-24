/* ###################################################################
**     Filename    : Events.c
**     Project     : full
**     Processor   : MK64FN1M0VLL12
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2016-05-09, 12:15, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file Events.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         
/* MODULE Events */

#include "Cpu.h"
#include "Events.h"

#ifdef __cplusplus
extern "C" {
#endif 


/* User includes (#include below this line is not maintained by Processor Expert) */

#ifdef pitTimer1_IDX
/*
** ===================================================================
**     Interrupt handler : pitTimer1_IRQHandler
**
**     Description :
**         User interrupt service routine. 
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void pitTimer1_IRQHandler(void)
{
  /* Clear interrupt flag.*/
  PIT_HAL_ClearIntFlag(g_pitBase[pitTimer1_IDX], pitTimer1_CHANNEL);
  /* Write your code here ... */
  int q[7200]; // buffer
    int index=0;
    int buffer_size=7200;

    uint16_t r[7200]; // buffer
    uint16_t label=0;

    int RPM, Frequency;
    				  		  int ESS_Teeth, EPS_Teeth, EPS_Width;
    				  		  int ESS, EPS;
    				  		  int widthEPS;
    				  		  int Missing_Teeth;
    				  		  int Extra_Teeth;
    				  		  char m[50];
    				  		  char t[50];
    				  		  //char N,Y;
    				  		  int a,b,w,v,x,z,e,f,g,h;
    				  		  int i,j,k;
    				  		  int loop_ESS,loop1_ESS, loop2_ESS, loop_EPS,loop1_EPS,loop2_EPS;
    				  		  int pos_EPS;
    				  		  //int y;
    				  		  //int TDC_Width;
    				  		   //dac_buffer;
    				  		  //uint16_t index;

    				  		  int crank = 0 ;

    				  		  int cam = 0;
    				  		  //int r[buffer_size];

    				  		  //int top = 0;
    				  		  //int p[buffer_size];

    				  		  	  	  debug_printf("Enter the RPM:\t");
    				  			      debug_scanf("%d" , &RPM);
    				  			      debug_printf("%d\r\n",RPM);

    				  			        // Calculate Frequency
    				  			      Frequency = RPM*120;
    				  			      debug_printf("Frequency is : %d\n" , Frequency);

    				  			      // FOR ESS
    				  			      debug_printf("\rEnter ESS_Teeth:\t");
    				  			      debug_scanf("%d", &ESS_Teeth);
    				  			      debug_printf("%d\r\n",ESS_Teeth);
    				  			      a = 360/ESS_Teeth;
    				  			      ESS=a*10;
    				  			      debug_printf("ESS : %d\n" , ESS);

    				  			        loop_ESS=buffer_size/ESS;
    				  			        //printf("LOOP_ESS: %d\n", loop_ESS);
    				  			        loop1_ESS=(buffer_size-2*ESS)/ESS;
    				  			        w = (buffer_size-2*ESS)/2;
    				  			        v =(loop1_ESS/2)*ESS;

    				  			        //printf("LOOP1_ESS: %d\n",loop1_ESS);


    				  			        //printf("%d\n",loop1_ESS);
    				  			        loop2_ESS=(buffer_size-4*ESS)/ESS;
    				  			        //printf("LOOP2_ESS: %d\n",loop2_ESS);
    				  			        x = (buffer_size-4*ESS)/2;
    				  			        z = (loop2_ESS/2)*ESS;

    				  			        debug_printf("\rDo you want inverted outputs?(Y/N)");
    				  			        debug_scanf("%s", &m);
    				  			        debug_printf("%s\r\n",m);

    				  			        debug_printf("\n\rEnter Missing_Teeth:");
    				  			        debug_scanf("%d", &Missing_Teeth);
    				  			        debug_printf("%d\r\n",Missing_Teeth);

    				  			        debug_printf("\rEnter Extra_Teeth:");
    				  			        debug_scanf("%d", &Extra_Teeth);
    				  			        debug_printf("%d\r\n",Extra_Teeth);

    				  			        debug_printf("\rThe output for ESS :\t");

    				  			    // If Missing Teeth and Extra Teeth is Zero For Non-inverted output
    				  	//for(;;){


    				  			    if(*m =='N' && Missing_Teeth==0 && Extra_Teeth==0)
    				  			    {
    				  			        for(k=0;k<(loop_ESS);k++)
    				  			        {
    				  			        for(i=0; i<ESS/2 ; i++)
    				  			        {
    				  			            q[crank] = 0x0;
    				  			            crank++;
    				  			            //printf("0");
    				  			        }
    				  			        for(j=0; j<ESS/2 ; j++)
    				  			        {
    				  			            q[crank] = 0xfff;
    				  			            crank++;
    				  			            //printf("1");
    				  			        }
    				  			    }
    				  			    if (a%2 !=0)
    				  			    {
    				  			        for(i=0;i<buffer_size-(loop_ESS*ESS);i++)
    				  			        {
    				  			            q[crank] = 0x0;
    				  			            crank++;
    				  			            //printf("0");
    				  			        }

    				  			    }
    				  			    }
    				  			    //For Non-inverted output
    				  			    if(*m =='N')
    				  			    {
    				  			    // Detect Missing or Extra Teeth in the beginning
    				  			       if (Missing_Teeth ==1)
    				  			        {
    				  			            for(i=0;i<ESS;i++)
    				  			        {
    				  			            q[crank] = 0x0;
    				  			            crank++;
    				  			            //printf("0");
    				  			        }

    				  			    }
    				  			        else if(Missing_Teeth == 2)
    				  			        {
    				  			            for(i=0;i<ESS;i++)
    				  			        {
    				  			            q[crank] = 0x0;
    				  			            crank++;
    				  			            //printf("0");
    				  			        }
    				  			            for(k=0;k<ESS;k++)
    				  			        {
    				  			            q[crank] = 0x0;
    				  			            crank++;
    				  			            //printf("0");
    				  			        }
    				  			        }
    				  			       if(Extra_Teeth==1)
    				  			        {
    				  			            for(i=0;i<ESS;i++)
    				  			        {
    				  			            q[crank] = 0xfff;
    				  			            crank++;
    				  			            //printf("1");
    				  			        }

    				  			    }
    				  			        else
    				  			        {
    				  			            debug_printf("");

    				  			        }

    				  			    // Loop for non-inverted outputs
    				  			    if(Missing_Teeth==1 || Extra_Teeth==1)
    				  			        {
    				  			    for(k=0;k<(loop1_ESS)/2;k++)
    				  			        {
    				  			        for(i=0; i<ESS/2 ; i++)
    				  			        {
    				  			            q[crank] = 0x0;
    				  			            crank++;
    				  			            //printf("0");
    				  			        }
    				  			        for(j=0; j<ESS/2 ; j++)
    				  			        {
    				  			            q[crank] = 0xfff;
    				  			            crank++;
    				  			            //printf("1");
    				  			        }
    				  			    }
    				  			    if (a%2 !=0)
    				  			    {
    				  			        for(i=0;i<(w-v);i++)
    				  			        {
    				  			            q[crank] = 0x0;
    				  			            crank++;
    				  			            //printf("0");
    				  			        }

    				  			    }
    				  			        }

    				  			    if(Missing_Teeth==2)
    				  			        {

    				  			    for(k=0;k<(loop2_ESS)/2;k++)
    				  			        {
    				  			        for(i=0; i<ESS/2 ; i++)
    				  			        {
    				  			            q[crank] = 0x0;
    				  			            crank++;
    				  			            //printf("0");
    				  			        }
    				  			        for(j=0; j<ESS/2 ; j++)
    				  			        {
    				  			            q[crank] = 0xfff;
    				  			            crank++;
    				  			            //printf("1");
    				  			        }
    				  			    }
    				  			    if (a%2 !=0)
    				  			    {
    				  			        for(i=0;i<(x-z);i++)
    				  			        {
    				  			            q[crank] = 0x0;
    				  			            crank++;
    				  			            //printf("0");
    				  			        }

    				  			    }
    				  			        }

    				  			    // Detect Missing or Extra Teeth in the middle
    				  			     if (Missing_Teeth ==1)
    				  			        {
    				  			            for(i=0;i<ESS;i++)
    				  			        {
    				  			            q[crank] = 0x0;
    				  			            crank++;
    				  			            //printf("0");
    				  			        }

    				  			    }
    				  			        else if(Missing_Teeth == 2)
    				  			        {
    				  			            for(i=0;i<ESS;i++)
    				  			        {
    				  			            q[crank] = 0x0;
    				  			            crank++;
    				  			            //printf("0");
    				  			        }
    				  			            for(k=0;k<ESS;k++)
    				  			        {
    				  			            q[crank] = 0x0;
    				  			            crank++;
    				  			            //printf("0");
    				  			        }
    				  			        }
    				  			       if(Extra_Teeth==1)
    				  			        {
    				  			            for(i=0;i<ESS;i++)
    				  			        {
    				  			            q[crank] = 0xfff;
    				  			            crank++;
    				  			            //printf("1");
    				  			        }

    				  			    }

    				  			        else
    				  			        {
    				  			            debug_printf("");

    				  			        }

    				  			    //Loop for non-inverted output
    				  			    if(Missing_Teeth==1 || Extra_Teeth==1)
    				  			    {

    				  			    for(k=0;k<(loop1_ESS)/2;k++)
    				  			        {
    				  			        for(i=0; i<ESS/2 ; i++)
    				  			        {
    				  			            q[crank] = 0x0;
    				  			            crank++;
    				  			            //printf("0");
    				  			        }
    				  			        for(j=0; j<ESS/2 ; j++)
    				  			        {
    				  			            q[crank] = 0xfff;
    				  			            crank++;
    				  			            //printf("1");
    				  			        }
    				  			    }
    				  			    if (a%2 !=0)
    				  			    {
    				  			        for(i=0;i<(w-v);i++)
    				  			        {
    				  			            q[crank] = 0x0;
    				  			            crank++;
    				  			            //printf("0");
    				  			        }

    				  			    }
    				  			        }

    				  			    if(Missing_Teeth==2)
    				  			    {

    				  			    for(k=0;k<(loop2_ESS)/2;k++)
    				  			        {
    				  			        for(i=0; i<ESS/2 ; i++)
    				  			        {
    				  			            q[crank] = 0x0;
    				  			            crank++;
    				  			            //printf("0");
    				  			        }
    				  			        for(j=0; j<ESS/2 ; j++)
    				  			        {
    				  			            q[crank] = 0xfff;
    				  			            crank++;
    				  			            //printf("1");
    				  			        }
    				  			    }
    				  			    if (a%2 !=0)
    				  			    {
    				  			        for(i=0;i<(x-z);i++)
    				  			        {
    				  			            q[crank] = 0x0;
    				  			            crank++;
    				  			            //printf("0");
    				  			        }

    				  			    }
    				  			    }

    				  			    }
    				  			    // For Inverted output-Missing, Extra Teeth is zero
    				  			    if(*m =='Y' && Missing_Teeth==0 && Extra_Teeth==0)
    				  			    {
    				  			        for(k=0;k<(loop_ESS);k++)
    				  			        {
    				  			        for(i=0; i<ESS/2 ; i++)
    				  			        {
    				  			            q[crank] = 0x0;
    				  			            crank++;
    				  			            //printf("0");
    				  			        }
    				  			        for(j=0; j<ESS/2 ; j++)
    				  			        {
    				  			            q[crank] = 0xfff;
    				  			            crank++;
    				  			            //printf("1");
    				  			        }
    				  			    }
    				  			    if (a%2 !=0)
    				  			    {
    				  			        for(i=0;i<buffer_size-(loop_ESS*ESS);i++)
    				  			        {
    				  			            q[crank] = 0x0;
    				  			            crank++;
    				  			            //printf("0");
    				  			        }

    				  			    }
    				  			    }
    				  			    //For inverted output detected Extra Teeth
    				  			    if(*m =='Y' && (Extra_Teeth ==1))
    				  			        {
    				  			    //Detect Extra Teeth in the beginning
    				  			        if(Extra_Teeth==1)
    				  			        {
    				  			            for(i=0;i<ESS;i++)
    				  			        {
    				  			            q[crank] = 0x0;
    				  			            crank++;
    				  			            //printf("0");
    				  			        }

    				  			    }

    				  			        else
    				  			        {
    				  			            debug_printf("");

    				  			        }


    				  			    //Loop for detected Extra Teeth
    				  			    if(Extra_Teeth==1)
    				  			        {
    				  			    for(k=0;k<(loop1_ESS)/2;k++)
    				  			        {
    				  			        for(i=0; i<ESS/2 ; i++)
    				  			        {
    				  			            q[crank] = 0xfff;
    				  			            crank++;
    				  			            //printf("1");
    				  			        }
    				  			        for(j=0; j<ESS/2 ; j++)
    				  			        {
    				  			            q[crank] = 0x0;
    				  			            crank++;
    				  			            //printf("0");
    				  			        }
    				  			    }
    				  			    if (a%2 !=0)
    				  			    {
    				  			        for(i=0;i<(w-v);i++)

    				  			            {
    				  			                q[crank] = 0xfff;
    				  			                crank++;
    				  			                //printf("1");
    				  			            }
    				  			    }
    				  			        }



    				  			    //Detected Extra Teeth in the middle
    				  			      if (Missing_Teeth ==1)
    				  			        {
    				  			            for(i=0;i<ESS;i++)
    				  			        {
    				  			            q[crank] = 0xfff;
    				  			            crank++;
    				  			            //printf("1");
    				  			        }

    				  			    }
    				  			        else if(Missing_Teeth == 2)
    				  			        {
    				  			            for(i=0;i<ESS;i++)
    				  			        {
    				  			            q[crank] = 0xfff;
    				  			            crank++;
    				  			            //printf("1");
    				  			        }
    				  			            for(k=0;k<ESS;k++)
    				  			        {
    				  			            q[crank] = 0xfff;
    				  			            crank++;
    				  			            //printf("1");
    				  			        }
    				  			        }
    				  			       if(Extra_Teeth==1)
    				  			        {
    				  			            for(i=0;i<ESS;i++)
    				  			        {
    				  			            q[crank] = 0x0;
    				  			            crank++;
    				  			            //printf("0");
    				  			        }

    				  			    }

    				  			        else
    				  			        {
    				  			            debug_printf("");

    				  			        }

    				  			    //Loop for Extra Teeth output
    				  			    if(Extra_Teeth==1)
    				  			    {

    				  			    for(k=0;k<(loop1_ESS)/2;k++)
    				  			        {
    				  			        for(i=0; i<ESS/2 ; i++)
    				  			        {
    				  			            q[crank] = 0xfff;
    				  			            crank++;
    				  			            //printf("1");
    				  			        }
    				  			        for(j=0; j<ESS/2 ; j++)
    				  			        {
    				  			            q[crank] = 0x0;
    				  			            crank++;
    				  			            //printf("0");
    				  			        }
    				  			    }
    				  			    if (a%2 !=0)
    				  			    {
    				  			        for(i=0;i<(w-v);i++)
    				  			        {
    				  			            q[crank] = 0xfff;
    				  			            crank++;
    				  			            //printf("1");
    				  			        }

    				  			    }
    				  			    }


    				  			        }
    				  			    // Inverted output for Missing Teeth detected
    				  			    if (*m =='Y' && (Missing_Teeth==1 || Missing_Teeth==2))
    				  			    {
    				  			    //For Inverted Missing Teeth in the beginning
    				  			        if (Missing_Teeth ==1)
    				  			        {
    				  			            for(i=0;i<ESS;i++)
    				  			        {
    				  			            q[crank] = 0xfff;
    				  			            crank++;
    				  			            //printf("1");
    				  			        }

    				  			    }
    				  			        else if(Missing_Teeth == 2)
    				  			        {
    				  			            for(i=0;i<ESS;i++)
    				  			        {
    				  			            q[crank] = 0xfff;
    				  			            crank++;
    				  			            //printf("1");
    				  			        }
    				  			            for(k=0;k<ESS;k++)
    				  			        {
    				  			            q[crank] = 0xfff;
    				  			            crank++;
    				  			            //printf("1");
    				  			        }
    				  			        }

    				  			        else
    				  			        {
    				  			            debug_printf("");

    				  			        }


    				  			    //Loop for Inverted Missing Teeth
    				  			    if(Missing_Teeth==1)
    				  			        {
    				  			    for(k=0;k<(loop1_ESS)/2;k++)
    				  			        {
    				  			        for(i=0; i<ESS/2 ; i++)
    				  			        {
    				  			            q[crank] = 0x0;
    				  			            crank++;
    				  			            //printf("0");
    				  			        }
    				  			        for(j=0; j<ESS/2 ; j++)
    				  			        {
    				  			            q[crank] = 0xfff;
    				  			            crank++;
    				  			            //printf("1");
    				  			        }
    				  			    }
    				  			    if (a%2 !=0)
    				  			    {
    				  			        for(i=0;i<(w-v);i++)
    				  			        {
    				  			            q[crank] = 0x0;
    				  			            crank++;
    				  			            //printf("0");
    				  			        }

    				  			    }
    				  			        }

    				  			        if(Missing_Teeth==2)
    				  			        {

    				  			    for(k=0;k<(loop2_ESS)/2;k++)
    				  			        {
    				  			        for(i=0; i<ESS/2 ; i++)
    				  			        {
    				  			            q[crank] = 0x0;
    				  			            crank++;
    				  			            //printf("0");
    				  			        }
    				  			        for(j=0; j<ESS/2 ; j++)
    				  			        {
    				  			            q[crank] = 0xfff;
    				  			            crank++;
    				  			            //printf("1");
    				  			        }
    				  			    }
    				  			    if (a%2 !=0)
    				  			    {
    				  			        for(i=0;i<(x-z);i++)
    				  			        {
    				  			            q[crank] = 0x0;
    				  			            crank++;
    				  			            //printf("0");
    				  			        }

    				  			    }
    				  			        }
    				  			    //For Inverted Missing Teeth in Middle
    				  			        if (Missing_Teeth ==1)
    				  			        {
    				  			            for(i=0;i<ESS;i++)
    				  			        {
    				  			            q[crank] = 0xfff;
    				  			            crank++;
    				  			            //printf("1");
    				  			        }

    				  			    }
    				  			        else if(Missing_Teeth == 2)
    				  			        {
    				  			            for(i=0;i<ESS;i++)
    				  			        {
    				  			            q[crank] = 0xfff;
    				  			            crank++;
    				  			            //printf("1");
    				  			        }
    				  			            for(k=0;k<ESS;k++)
    				  			        {
    				  			            q[crank] = 0xfff;
    				  			            crank++;
    				  			            //printf("1");
    				  			        }
    				  			        }

    				  			        else
    				  			        {
    				  			            debug_printf("");

    				  			        }

    				  			    //Loop for  Inverted Missing Teeth
    				  			    if(Missing_Teeth==1)
    				  			    {

    				  			    for(k=0;k<(loop1_ESS)/2;k++)
    				  			        {
    				  			        for(i=0; i<ESS/2 ; i++)
    				  			        {
    				  			            q[crank] = 0x0;
    				  			            crank++;
    				  			            //printf("0");
    				  			        }
    				  			        for(j=0; j<ESS/2 ; j++)
    				  			        {
    				  			            q[crank] = 0xfff;
    				  			            crank++;
    				  			            //printf("1");
    				  			        }
    				  			    }
    				  			    if (a%2 !=0)
    				  			    {
    				  			        for(i=0;i<(w-v);i++)
    				  			        {
    				  			            q[crank] = 0;
    				  			            crank++;
    				  			            //printf("0");
    				  			        }

    				  			    }
    				  			    }

    				  			    if(Missing_Teeth==2)
    				  			    {

    				  			    for(k=0;k<(loop2_ESS)/2;k++)
    				  			        {
    				  			        for(i=0; i<ESS/2 ; i++)
    				  			        {
    				  			            q[crank] = 0x0;
    				  			            crank++;
    				  			            //printf("0");
    				  			        }
    				  			        for(j=0; j<ESS/2 ; j++)
    				  			        {
    				  			            q[crank] = 0xfff;
    				  			            crank++;
    				  			            //printf("1");
    				  			        }
    				  			    }
    				  			    if (a%2 !=0)
    				  			    {
    				  			        for(i=0;i<(x-z);i++)
    				  			        {
    				  			            q[crank] = 0x0;
    				  			            crank++;
    				  			            //printf("0");
    				  			        }

    				  			    }
    				  			    }

    				  			    }
    				  		//for(;;)
    				  		//{
    				  			   for(index=0;index<buffer_size;index++)
    				  			      {
    				  				   	debug_printf("%d", q[index]);
    				  				   	DAC_DRV_Output(FSL_DACONV1,q[index]);

    				  			      }
    				  		 //}
    				  	//	}
    				  			 // FOR EPS
    				  			     				  			 debug_printf("\n\rEnter EPS Teeth:");
    				  			     				  			 debug_scanf("%d", &EPS_Teeth);
    				  			     				  			 debug_printf("%d\r\n",EPS_Teeth);
    				  			  		  			             debug_printf("\n\rEnter EPS Width:");
    				  			     				  			 debug_scanf("%d" , &EPS_Width);
    				  			     				  			 debug_printf("%d\r\n",EPS_Width);
    				  			     		  			           b=720/EPS_Teeth;
    				  			     		  			           EPS=b*10;

    				  			     		  			         loop_EPS=7200/EPS;
    				  			     		  			         //printf("loop_EPS : %d" , loop_EPS);

    				  			     		  			         loop1_EPS=(7200-EPS)/EPS;
    				  			       	  			             //printf(" loop1_EPS: %d\n" , loop1_EPS);
    				  			     				  			 loop2_EPS=(7200-2*EPS)/EPS;
    				  			     				  			 //printf("loop2_EPS: %d\n" , loop2_EPS);

    				  			     				  			 debug_printf("EPS : %d\n", EPS);
    				  			     				  			 widthEPS=EPS_Width*10;
    				  			     				  			 //printf("%d\n", widthEPS);

    				  			     				  			 pos_EPS= EPS-(2*widthEPS);
    				  			     				  			 //printf("%d\n" , pos_EPS);
    				  			     				  			 //y=EPS-pos_EPS;
    				  			     				  			 //printf("%d\n", y);

    				  			     				  			 e = 7200-EPS;
    				  			     				  			 f = loop1_EPS*EPS;
    				  			     				  			 g = 7200-2*EPS;
    				  			     				  			 h = loop2_EPS*EPS;

    				  			     				  			 debug_printf("\n\rDo you want inverted outputs?(Y/N)");
    				  			     				  			 debug_scanf("%s", &t);
    				  			     				  			 debug_printf("%s\r\n",t);

    				  			     				  			 debug_printf("\n\rEnter Missing Teeth:\t");
    				  			     				  			 debug_scanf("%d", &Missing_Teeth);
    				  			     				  			 debug_printf("%d\r\n",Missing_Teeth);

    				  			     				  			 debug_printf("\n\rEnter Extra Teeth:\t");
    				  			     				  			 debug_scanf("%d" , &Extra_Teeth);
    				  			     				  			 debug_printf("%d\r\n",Extra_Teeth);

    				  			     				  			 debug_printf("\n\rThe output for EPS is:\t");

    				  			     				  			 if(*t =='N')
    				  			     				  			    {
    				  			     				  			       if(Missing_Teeth==0 && Extra_Teeth==0)
    				  			     				  			         {
    				  			     				  			           for(i=0;i<loop_EPS;i++)
    				  			     				  			              {
    				  			     				  			               for(j=0;j<EPS/2;j++)
    				  			     				  			                {
    				  			     				  			                  r[cam] = 0x0;
    				  			     				  			                  cam++;
    				  			     				  			                  //printf("0");
    				  			     				  			                 }
    				  			     				  			    for(k=0;k<EPS/2;k++)
    				  			     				  			       {
    				  			     				  			         r[cam] = 0xfff;
    				  			     				  			         cam++;
    				  			     				  			         //printf("1");
    				  			     				  			       }
    				  			     				  			                }
    				  			     				  			    if (a%2 !=0)
    				  			     				  			       {
    				  			     				  			         for(i=0;i<7200-(loop_EPS*EPS);i++)
    				  			     				  			           {
    				  			     				  			        	  r[cam] = 0x0;
    				  			     				  			        	  cam++;
    				  			     				  			        	  //printf("0");
    				  			     				  			        	}

    				  			     				  			         }
    				  			     				  			             }

    				  			     				  		         if (Missing_Teeth==1)

    				  			     				  			      {
    				  			     				  			        for(i=0;i<EPS;i++)
    				  			     				  			         {
    				  			     				  			           r[cam] = 0x0;
    				  			     				  			           cam++;
    				  			     				  			           //printf("0");
    				  			     				  			         }
    				  			     				  			        for(k=0;k<(loop1_EPS);k++)
    				  			     				  			          {
    				  			     				  			            for(i=0; i<EPS/2 ; i++)
    				  			     				  			               {
    				  			     				  			                  r[cam] = 0x0;
    				  			     				  			                  cam++;
    				  			     				  			                  //printf("0");
    				  			     				  			               }
    				  			     				  			            for(j=0; j<EPS/2 ; j++)
    				  			     				  			               {
    				  			     				  			                 r[cam] = 0xfff;
    				  			     				  			                 cam++;
    				  			     				  			                 //printf("1");
    				  			     				  			               }
    				  			     				  			            }
    				  			     				  			         if (a%2 !=0)
    				  			     				  			           {
    				  			     				  			             for(i=0;i<(e-f);i++)
    				  			     				  			               {
    				  			     				  			                 r[cam] = 0xfff;
    				  			     				  			                 cam++;
    				  			     				  			                 //printf("1");
    				  			     				  			               }

    				  			     				  			            }

    				  			     				  			          }

    				  			     				  			        if (Missing_Teeth==2)
    				  			     				  			          {
    				  			     				  			            for(i=0;i<EPS;i++)
    				  			     				  			              {
    				  			     				  			                r[cam] = 0x0;
    				  			     				  			                cam++;
    				  			     				  			                //printf("0");
    				  			     				  			              }
    				  			     				  			        for(j=0;j<EPS;j++)
    				  			     				  			          {
    				  			     				  			            r[cam] = 0x0;
    				  			     				  			            cam++;
    				  			     				  			            //printf("0");
    				  			     				  			          }
    				  			     				  			       for(k=0;k<(loop2_EPS);k++)
    				  			     				  			          {
    				  			     				  			           for(i=0; i<EPS/2 ; i++)
    				  			     				  			            {
    				  			     				  			              r[cam] = 0x0;
    				  			     				  			              cam++;
    				  			     				  			              //printf("0");
    				  			     				  			            }
    				  			     				  			       for(j=0; j<EPS/2 ; j++)
    				  			     				  			           {
    				  			     				  			             r[cam] = 0xfff;
    				  			     				  			             cam++;
    				  			     				  			             //printf("1");
    				  			     				  			           }
    				  			     				  			        }
    				  			     				  			       if (a%2 !=0)
    				  			     				  			        {
    				  			     				  			         for(i=0;i<(g-h);i++)
    				  			     				  			          {
    				  			     				  			           r[cam] = 0xfff;
    				  			     				  			           cam++;
    				  			     				  			           //printf("1");
    				  			     				  			          }

    				  			     				  			         }
    				  			     				  			       }

    				  			     				  			     if(Extra_Teeth==1)
    				  			     				  			       {
    				  			     				  			        for(k=0; k<pos_EPS;k++)
    				  			     				  			          {
    				  			     				  			            r[cam] = 0x0;
    				  			     				  			            cam++;
    				  			     				  			            //printf("0");
    				  			     				  			          }
    				  			     				  			        for(i=(EPS-pos_EPS);i<(EPS-pos_EPS)+widthEPS;i++)
    				  			     				  			          {
    				  			     				  			           r[cam] = 0xfff;
    				  			     				  			           cam++;
    				  			     				  			           //printf("1");
    				  			     				  			          }


    				  			     				  			        for(j=pos_EPS+widthEPS;j<EPS ; j++)
    				  			     				  			          {
    				  			     				  			            r[cam] = 0xfff;
    				  			     				  			            cam++;
    				  			     				  			            //printf("1");
    				  			     				  			          }
    				  			     				  			        for(k=0;k<(loop1_EPS);k++)
    				  			     				  			          {
    				  			     				  			            for(i=0; i<EPS/2 ; i++)
    				  			     				  			              {
    				  			     				  			                r[cam] = 0x0;
    				  			     				  			                cam++;
    				  			     				  			                //printf("0");
    				  			     				  			               }
    				  			     				  			             for(j=0; j<EPS/2 ; j++)
    				  			     				  			               {
    				  			     				  			                 r[cam] = 0xfff;
    				  			     				  			                 cam++;
    				  			     				  			                 //printf("1");
    				  			     				  			                }
    				  			     				  			             }
    				  			     				  			        if (a%2 !=0)
    				  			     				  			           {
    				  			     				  			             for(i=0;i<(e-f);i++)
    				  			     				  			                {
    				  			     				  			                  r[cam] = 0xfff;
    				  			     				  			                  cam++;
    				  			     				  			                  //printf("1");
    				  			     				  			                 }

    				  			     				  			        	}
    				  			     				  			          }

    				  			     				  			       }

    				  			     				  			 if(*t =='Y')
    				  			    				  			      {
    				  			     				  			   if(Missing_Teeth==0 && Extra_Teeth==0)
    				  			     				  			    {
    				  			     				  			     for(i=0;i<loop_EPS;i++)
    				  			     				  			       {
    				  			     				  			        for(j=0;j<EPS/2;j++)
    				  			     				  			          {
    				  			     				  			            r[cam] = 0x0;
    				  			     				  			            cam++;
    				  			     				  			            //printf("0");
    				  			     				  			           }
    				  			     				  			     for(k=0;k<EPS/2;k++)
    				  			     				  			        {
    				  			     				  			          r[cam] = 0xfff;
    				  			     				  			          cam++;
    				  			     				  			          //printf("1");
    				  			     				  			        }
    				  			     				  			      }
    				  			     				  			   if (a%2 !=0)
    				  			     				  			    {
    				  			     				  			     for(i=0;i<7200-(loop_EPS*EPS);i++)
    				  			     				  			       {
    				  			     				  			        r[cam] = 0x0;
    				  			     				  			        cam++;
    				  			     				  			        //printf("0");
    				  			     				  			       }

    				  			     				  			      }
    				  			     				  			    }

    				  			     				  			   if (Missing_Teeth==1)

    				  			     				  			    {
    				  			     				  			     for(i=0;i<EPS;i++)
    				  			     				  			      {
    				  			     				  			       r[cam] = 0xfff;
    				  			     				  			       cam++;
    				  			     				  			       //printf("1");
    				  			     				  			      }
    				  			     				  			   for(k=0;k<(loop1_EPS);k++)
    				  			     				  			     {
    				  			     				  			       for(i=0; i<EPS/2 ; i++)
    				  			     				  			        {
    				  			     				  			          r[cam] = 0x0;
    				  			     				  			          cam++;
    				  			     				  			          //printf("0");
    				  			     				  			        }
    				  			     				  			    for(j=0; j<EPS/2 ; j++)
    				  			     				  			        {
    				  			     				  			         r[cam] = 0xfff;
    				  			     				  			         cam++;
    				  			     				  			         //printf("1");
    				  			     				  			        }
    				  			     				  			      }
    				  			     				  			   if (a%2 !=0)
    				  			     				  			    {
    				  			     				  			     for(i=0;i<(e-f);i++)
    				  			     				  			      {
    				  			     				  			       r[cam] = 0x0;
    				  			     				  			       cam++;
    				  			     				  			       //printf("0");
    				  			     				  			      }

    				  			     				  			    }
    				  			     				  			   }

    				  			     				  			   if (Missing_Teeth==2)
    				  			     				  			    {
    				  			     				  			     for(i=0;i<EPS;i++)
    				  			     				  			      {
    				  			     				  			       r[cam] = 0xfff;
    				  			     				  			       cam++;
    				  			     				  			       //printf("1");
    				  			     				  			      }
    				  			     				  			   for(j=0;j<EPS;j++)
    				  			     				  			     {
    				  			     				  			       r[cam] = 0xfff;
    				  			     				  			       cam++;
    				  			     				  			       //printf("1");
    				  			     				  			     }
    				  			     				  			  for(k=0;k<(loop2_EPS);k++)
    				  			     				  			    {
    				  			     				  			      for(i=0; i<EPS/2 ; i++)
    				  			     				  			      {
    				  			     				  			        r[cam] = 0x0;
    				  			     				  			        cam++;
    				  			     				  			        //printf("0");
    				  			     				  			      }
    				  			     				  			  for(j=0; j<EPS/2 ; j++)
    				  			     				  			     {
    				  			     				  			      r[cam] = 0xfff;
    				  			     				  			      cam++;
    				  			     				  			      //printf("1");
    				  			     				  			      }
    				  			     				  			    }
    				  			     				  			 if (a%2 !=0)
    				  			     				  			  {
    				  			     				  			   for(i=0;i<(g-h);i++)
    				  			     				  			    {
    				  			     				  			      r[cam] = 0x0;
    				  			     				  			      cam++;
    				  			     				  			      //printf("0");
    				  			     				  			    }

    				  			         				  	       }

    				  			     				  		      }

    				  			     				  		    if(Extra_Teeth==1)
    				  			     				  			  {
    				  			     				  			   for(k=0; k<pos_EPS;k++)
    				  			     				  			    {
    				  			     				  			     r[cam] = 0xfff;
    				  			     				  			     cam++;
    				  			     				  			     //printf("1");
    				  			     				  			    }
    				  			     				  			for(i=(EPS-pos_EPS);i<(EPS-pos_EPS)+widthEPS;i++)
    				  			     				  			  {
    				  			     				  			    r[cam] = 0x0;
    				  			     				  			    cam++;
    				  			     				  			    //printf("0");
    				  			     				  			  }


    				  			     				  			 for(j=pos_EPS+widthEPS;j<EPS ; j++)
    				  			     				  			   {
    				  			     				  			     r[cam] = 0xfff;
    				  			     				  			     cam++;
    				  			     				  			     //printf("1");
    				  			     				  			   }
    				  			     				  			 for(k=0;k<(loop1_EPS);k++)
    				  			     				  			   {
    				  			     				  			    for(i=0; i<EPS/2 ; i++)
    				  			     				  			     {
    				  			     				  			      r[cam] = 0xfff;
    				  			     				  			      cam++;
    				  			     				  			      //printf("1");
    				  			     				  			     }
    				  			     				  			 for(j=0; j<EPS/2 ; j++)
    				  			     				  			  {
    				  			     				  			    r[cam] = 0x0;
    				  			     				  			    cam++;
    				  			     				  			    //printf("0");
    				  			     				  		      }
    				  			     				  			}
    				  			     				  		 if (a%2 !=0)
    				  			     				  		  {
    				  			     				  		    for(i=0;i<(e-f);i++)
    				  			     				  		     {
    				  			     				  		       r[cam] = 0xfff;
    				  			     				  		       cam++;
    				  			     				  		       //printf("1");
    				  			     				  		     }
    				  			     				  		  }
    				  			     				  	   }
    				  			     				  	}

    				  			     				  	for(label=0;label<buffer_size;label++)
    				  			     				     	{
    				  			     				  		  //index = 0;
    				  			     				  		  //debug_printf("%d", r[label]);
    				  			     				  			DAC_DRV_Output(FSL_DACONV1,r[label++]);
    				  			     				  			//if(index==buffer_size)
    				  			     				  			//  index = 0;
    				  			     				  	 	 }
    				  			     				  	//}



}
#else
  /* This IRQ handler is not used by pitTimer1 component. The purpose may be
   * that the component has been removed or disabled. It is recommended to 
   * remove this handler because Processor Expert cannot modify it according to 
   * possible new request (e.g. in case that another component uses this
   * interrupt vector). */
  #warning This IRQ handler is not used by pitTimer1 component.\
           It is recommended to remove this because Processor Expert cannot\
           modify it according to possible new request.
#endif

/*
** ===================================================================
**     Interrupt handler : DAC0_IRQHandler
**
**     Description :
**         User interrupt service routine. 
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void DAC0_IRQHandler(void)
{
  /* Write your code here ... */
}

/* END Events */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
