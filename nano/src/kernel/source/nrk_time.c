/******************************************************************************
*  Nano-RK, a real-time operating system for sensor networks.
*  Copyright (C) 2007, Real-Time and Multimedia Lab, Carnegie Mellon University
*  All rights reserved.
*
*  This is the Open Source Version of Nano-RK included as part of a Dual
*  Licensing Model. If you are unsure which license to use please refer to:
*  http://www.nanork.org/nano-RK/wiki/Licensing
*
*  This program is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, version 2.0 of the License.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*  Contributing Authors (specific to this file):
*  Anthony Rowe
*  Zane Starr
*  Anand Eswaren
*******************************************************************************/


/******************************************************************************
*                                         GET CURRENT SYSTEM TIME
*
* Description: This function is used by your application to obtain the current 
* value of the 32-bit 
*              counter which keeps track of the number of clock ticks.
*
* Arguments  : none
*
* Returns    : The current value of OSTime
*******************************************************************************/
#include <nrk.h>
#include <nrk_timer.h>
#include <nrk_error.h>

void nrk_time_get(nrk_time_t *t)
{
 t->secs=nrk_system_time.secs;
 t->nano_secs=nrk_system_time.nano_secs;

   t->nano_secs+=((uint32_t)_nrk_os_timer_get()*(uint32_t)NANOS_PER_TICK);
    while(t->nano_secs>=(uint32_t)NANOS_PER_SEC)
    {
    t->nano_secs-=(uint32_t)NANOS_PER_SEC;
    t->secs++;
    }
}

/*
 * result = high-low
 *
 */
int8_t nrk_time_sub(nrk_time_t *result,nrk_time_t high, nrk_time_t low)
{
if(high.secs<low.secs) return NRK_ERROR; 
if(low.secs==high.secs)
	{
	if((uint32_t)low.nano_secs>(uint32_t)high.nano_secs)  return NRK_ERROR;  
	result->nano_secs=(uint32_t)high.nano_secs-(uint32_t)low.nano_secs;
	result->secs=0;
	return NRK_OK;
	}
if(low.nano_secs > high.nano_secs)
{
	high.secs--;
	high.nano_secs+=(uint32_t)NANOS_PER_SEC;
	result->secs=high.secs-low.secs;
	result->nano_secs=high.nano_secs-low.nano_secs;
	return NRK_OK;
}

result->secs=high.secs-low.secs;
result->nano_secs=high.nano_secs-low.nano_secs;
return NRK_OK;
}


/*
 * result = a+b
 *
 */
int8_t nrk_time_add(nrk_time_t *result,nrk_time_t a, nrk_time_t b)
{
result->secs=a.secs+b.secs;
result->nano_secs=a.nano_secs+b.nano_secs;
nrk_time_compact_nanos(result);
return NRK_OK;
}

/*
 * nrk_time_compact_nanos()
 *
 * If a time structure has more than 1 second represented in
 * the nano seconds field, this function will move the nano
 * seconds into the seconds field.
 *
 */
inline void nrk_time_compact_nanos(nrk_time_t *t)
{
  while(t->nano_secs>=NANOS_PER_SEC)
    {
    t->nano_secs-=NANOS_PER_SEC;
    t->secs++;
    }
}



void nrk_time_set(uint32_t secs, uint32_t nano_secs)
{
  nrk_system_time.secs=secs;
  nrk_system_time.nano_secs=nano_secs;
}

uint16_t _nrk_time_to_ticks(nrk_time_t *t)
{
uint16_t ticks;
uint64_t tmp;
uint8_t i;
// FIXME: This will overflow
if(t->secs>0)
{
   tmp=t->nano_secs;
   if(t->secs>65) return 0;
   for(i=0; i<t->secs; i++ ) tmp+=NANOS_PER_SEC; // t->nano_secs+=NANOS_PER_SEC;
   ticks=tmp/(uint32_t)NANOS_PER_TICK;
   //ticks=t->nano_secs/(uint32_t)NANOS_PER_TICK;
}else
{
ticks=t->nano_secs/(uint32_t)NANOS_PER_TICK;
}


/*
if(t.secs>=1)
{
t.nano_secs+=NANOS_PER_SEC;
t.secs--;
ticks=t.nano_secs/(uint32_t)NANOS_PER_TICK;
ticks+=t.secs*TICKS_PER_SEC;
	nrk_kprintf( PSTR("1"));
}else
{
ticks=t.nano_secs/(uint32_t)NANOS_PER_TICK;
	nrk_kprintf( PSTR("2"));
}

tmp=ticks;
while(tmp>TICKS_PER_SEC)tmp-=TICKS_PER_SEC;
t.secs=tmp*NANOS_PER_TICK;

if(t.nano_secs>(t.secs+(NANOS_PER_TICK/2)))
{
	nrk_kprintf( PSTR("3"));
	ticks++;

}
*/
//ticks=t->nano_secs/(uint32_t)NANOS_PER_TICK;
//ticks+=t->secs*(uint32_t)TICKS_PER_SEC;
return ticks;
}

nrk_time_t _nrk_ticks_to_time(uint32_t ticks)
{
nrk_time_t t;

t.secs=ticks/TICKS_PER_SEC;
t.nano_secs=(ticks%TICKS_PER_SEC)*NANOS_PER_TICK;

return t;
}

uint32_t _nrk_time_to_ticks_long(nrk_time_t *t)
{
uint32_t ticks;
uint64_t tmp;
uint8_t i;
// FIXME: This will overflow
if(t->secs>0)
{
   tmp=t->nano_secs;
   for(i=0; i<t->secs; i++ ) tmp+=NANOS_PER_SEC; // t->nano_secs+=NANOS_PER_SEC;
   ticks=tmp/(uint32_t)NANOS_PER_TICK;
   //ticks=t->nano_secs/(uint32_t)NANOS_PER_TICK;
}else
{
ticks=t->nano_secs/(uint32_t)NANOS_PER_TICK;
}
return ticks;
}


/*
uint32_t _nrk_time_to_ticks_long(nrk_time_t t)
{
uint32_t ticks;
uint32_t tmp;

if(t.secs>=1)
{
t.nano_secs+=NANOS_PER_SEC;
t.secs--;
ticks=t.nano_secs/(uint32_t)NANOS_PER_TICK;
ticks+=t.secs*TICKS_PER_SEC;
}else
{
ticks=t.nano_secs/(uint32_t)NANOS_PER_TICK;
}

tmp=ticks;
while(tmp>TICKS_PER_SEC)tmp-=TICKS_PER_SEC;
t.secs=tmp*NANOS_PER_TICK;

if(t.nano_secs>t.secs+(NANOS_PER_TICK/2))ticks++;

//ticks=t->nano_secs/(uint32_t)NANOS_PER_TICK;
//ticks+=t->secs*(uint32_t)TICKS_PER_SEC;
return ticks;
}
*/


