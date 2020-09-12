/*
 * value.c
 *
 *  Created on: 12.09.2020
 *      Author: User
 */
#include "value.h"

void conversion(struct value *data)
{
data->calculation_value=10*(data->integer)+((data->decimal)/10);
}

void increase_value(struct value *data)
{
	if(data->decimal<90)
			 {
				 data->decimal+=10;
			 }
			 else
			 {
				 data->decimal=0;
				 data->integer++;
			 }

}

void decrease_value(struct value *data)
{
	 if(data->decimal >0)
	 	 	 	 {
					 data->decimal-=10;
				 }
				 else
				 {
					 if (data->integer<=0 && data->decimal<=0)
					 {
						 data->integer=0;
					 	 data->decimal=0;
					 }
					 else
					 {
					 data->decimal=90;
					 data->integer--;
					 }
				 }
}

